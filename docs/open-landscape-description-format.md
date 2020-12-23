# Open Landscape Description Format

## Stuart Staniford

## v 0.0.2 12/21/2020

***

## Introduction

This specification describes the Open Landscape Description Format (OLDF).  This is a JSON file format intended for describing a section of the earth's surface that is, or will be, landscaped.  The file format originated as the file format for the Permaplan software for landscape planning with a permaculture flavor, but the OLDF format itself is not restricted and may be used by other software for the same or related purposes.

Files formatted according to this specification typically use a .oldf extension.  Mime type is tbd, but
application/json should work until something more specific is available.  All OLDF files should be fully legal JSON.  For the purposes of this specification, the JSON format is defined by [RFC 8529] (https://tools.ietf.org/html/rfc8259).  The one exception is that OLDF explicitly allows the use of C++
style comments (ie beginning with //).  OLDF itself does not specificy any particular comments, and
OLDF parsers should ignore all comments - they are simply for human-readable annotations to the 
file.

The goal of an OLDF format file is to describe a single contiguous parcel of land, or a small number of related parcels, for purposes of assessing or planning the management of the landscape.  Tasks of particular importance are 
  * To be able to visualize the landscape at particular points in its development.
  * To be able to plan possible changes to the landscape, and visualize the effect of those changes.
  * To be able to assess the sustainability of the landscape, with particular focus on
      * The carbon uptake or release of the landscape.
      * The albedo of the landscape
      * Other important biophysical impacts of the landscape, such as moisture retention and release.
      * The fire resistance of the landscape under different climate conditions.
  * To be able to assess the biological or horticultural productivity of the landscape, and plan possible treatments that would have an impact on it's productivity.

## Overall Structure of the File

An OLDF file consists of a series of objects, each of which is a self-contained represention of a particular contiguous parcel of land.  An OLDF file should have at least one object, but may have an arbitrary number.  These master objects are referred to as "OLDF objects".  A single OLDF object must refer to a contiguous (ie connected) portion of land presumed to be under common management.  Successive objects might be adjacent, or disconnected.  However, implementations may assume that the objects in a file are not overlapping in their geographical extent.  

Separate OLDF objects in a file otherwise share no relationship, and n the remainder of the document, we describe the structure of a single OLDF object.  Multiple OLDF objects are processed in parallel to each other without reference to the others.

Each OLDF object consists of a series of JSON objects which are the values of subobjects inside, and associated with names of the master OLDF object.  Various of these subobjects are described in the remaining sections of this specification.  

However, the important master principle is that implementations coming across sub-objects with tag names they do not understand should ignore those sub-objects.  This allows experimental or implementation-specific extensions of the file format.  Implementors of such extensions should be aware that there is no guarantee that arbitrary name choices may not be over-ridden by future versions of this specification.  However, the specification commits to never create new sub-objects (or sub-sub-objects) that begin with the sequence 'X-', thus meaning they are safe for proprietary/experimental extensions (except that different proprietary extensions might conflict with each other, and at present this specification does not provide any facilities to prevent such collisions.)

### Overview Example

This gives an example showing the major sections of an OLDF object in a file (details are suppressed with ellipses).  Each of these sections is treated at length in subsequent sections, but the example should serve to make the overall structure clearer.  
  * The `introductoryData` section has metadata about the file and global information required to interpret the file (eg choices of units).  
  * The `landSurface` section describes the shape of the relief of this particular piece of land.
  * The `plants` section describes plants either present on the land, or planned for the future.
  * The `boundaries` section contains the coordinates of an enclosing polygon of the land described by this OLDF object.
  * The `fencing` section describes the geometry of fencing present or planned on the land.

```
// Example OLDF file with one OLDF object
{
  "introductoryData":
    {
     "spaceUnits":        "feet",
     "version":           [0, 0, 2],
     "fileTime":          [<seconds>, <microseconds>],
     "baseYear":          2020,
     "software":          "permaplan",
     "softwareVersion":   "1.2.3",
     "author":            "Stuart Staniford"
    },
 "landSurface":
  {
   ...
  },
 "plants":
  {
   ...
  },
 "boundaries":
  {
   ...
  },  
 "fencing":
  {
   ...
  }  
}
```

### Character sets and internationalization

All JSON names in objects should be expressed in 8 bit ASCII characters, as should all JSON numerical values and JSON structure such as {, }, [, etc.  However, the contents of strings in values are expressed in UTF8 characters and thus may represent any language or alphabet that can be captured via UTF8.  UTF8 is as described in [RFC 3629](https://tools.ietf.org/html/rfc3629).

## Introductory Data

The `introductoryData` section has metadata about the file and global information required to interpret the file.  It has certain mandatory name/value pairs that must be present, and then others that are optional.  We here take them in turn.

### spaceUnits (mandatory)

Syntax: `"spaceUnits":  "<units>",`

where `<units>` is a JSON string allowed only to be one of either `feet` or `meters`, reflecting either Imperial/SAE or SI units of measurement.  A file must pick exactly one such unit and use it throughout for coordinates of geometry in a variety of other sections.  There is no provision for direct use of other sub-units such as inches, yards, or centimeters.  Generally geometric co-ordinates will be specified as JSON numbers, and thus decimal fractions of the measurements are allowed.  Thus a length of twelve feet, three inches would be expressed as 12.25.

### fileTime (mandatory)

Syntax: `"fileTime":  [<seconds>, <microseconds>]`

The fileTime value is a JSON array of exactly two integers which denote the time at which the file was written.  The first integer is the number of seconds since the beginning of 1970, UTC (aka "Unix time") and the second integer is a number of microseconds (expressing the fractional part of a second).

### baseYear (mandatory)

Syntax: `"baseYear":  <year>,`

The baseYear value is an integer value for the year that is considered "base" for simulation and extrapolation purposes.  That is, plants and the like present before the baseYear are considered existing, whereas ones planted after that are projected (by default).  This may or may not be the year associated with the fileTime (eg a plan might be created which cannot be executed until some years in the future).  The possible range of valid baseYears is presently restricted to the years 1500 to 2500 AD (to
allow some error checking for unreasonable values).

### version (mandatory)

Syntax: `"version":  [<majorversion>, <minorversion1>, <minorversion2>]`

The version value is a JSON array of exactly three integers which denote the version of this specification that governs the format of the file.  The current version is `[0,0,1]`.  Version numbers will be incremented as the file format continues to evolve over time.  A general convention is that changes in minor version numbers will be expected to preserve backward compatability with older versions of OLDF parsing software.  However, this may not be the case with changes in major version number.  Major version 0 is used for versions of the specification prior to general release, while it is in early development and testing.

### software (mandatory)

Syntax: `"software":  "<progname>",`

The software value is a JSON string to denote the software that wrote out the file.  The special value `manual edit` can be used to denote a file created in a text editor or similar.


### softwareVersion (optional)

Syntax: `"softwareVersion":  "<version-info>",`

The software-version is a JSON string to denote the version of the particular software that wrote out the file (as distinct from the version of this OLDF specification itself).  This specification allows OLDF writing software to use any legal JSON string for their version information.

### author (optional)

Syntax: `"author":  "First Last",`

The author value is a JSON string to denote the human author of a particular file.  The specification allows any UTF8 string and does not constrain to any particular convention of naming humans or organizations.

## Land Surface


## Boundaries

The boundaries object is used to define the edge of the contiguous parcel described by this particular OLDF object.  Generally the boundaries will be the legally defined bounds of the parcel ownership.  Boundaries are defined by means of a single reference point absolutely defined by latitude and longtitude, and then a series of straight arcs expressed in spaceUnits (either feet or meters as defined in the introductoryData).  Non straight boundary arcs must be approximated by a series of short straight segments. The boundary arcs must vector sum to zero (ie the boundary forms a closed polygon), and the boundary must not intersect itself otherwise.  The detailed formats of the two sections are as follows. T

### referencePoint (mandatory)

Syntax: `"referencePoint":  [<latitude>,<longtitude>],`

The referentPoint object value is a JSON arry with exactly two numeric members, the first representing the latitude and the second of the longtitude of the reference point in degrees.  Minutes and seconds are not represented directly by as decimal parts of the latitude and longtitude (of course parsing applications may display these values to users however they like, but the representation in the file is via decimal parts of a degree).  Longtitudes east of the Greenwich meridian are positive, whereas longtitudes west of it are negative.

### arcs (mandatory)

Syntax: `"arcs":  [ [<x1>, <y1>], [<x2>, <y2>], ...,`

The arcs object is a JSON array of arbitrary length (but at least three members).  Each member in this array is a two dimension array representing a two-dimensional vector in the horizontal plane, expressed in spaceUnits (as defined in the introductoryData to be either feet or meters).  The series of vectors are interpreted as a successive series of movements starting from the reference point which form the boundary of the landscape described by a particular OLDF object.  The boundary arcs must vector sum to zero (ie the boundary forms a closed polygon), and the boundary must not intersect itself otherwise.  When seen from overhead, the boundary arcs must go in a counter-clockwise direction.


### Boundaries Example
The example below shows a simple boundaries object

```
// Example OLDF boundaries object
 "boundaries":
  {
   "referencePoint": [40.7831, -73.9712],
   "arcs": [
            [300, 0],
            [0, 300],
            [-300, 0],
            [0, -300]
           ]
  },
```
This example is a square plot, three hundred feet on each side, in Manhattan, NY.

## Plants

This section covers plants in the landscape.  The plants value is a JSON array of an arbitrary number of individual plants.  Each plant is described by an object, with various named attributes about that particular plant.

### Plants Example
The example below shows the beginning of a plants object, with the first plant in the array laid
out in detail.  Individual attributes of the plant are described below.

```
// Example OLDF plants object
 "plants":
  {
    [
      {
       "location": [92, 48],
       "yearPlanted": 2022,
       "timePlanted":
       "genus": "Acer",
       "species": "rubrum",
       "taxonomyLink": 
    },
   ...
   ]
 },
```
### location (mandatory)

Syntax: `"location": [<unitsEast>, <unitsNorth>],`

The location gives the planting location (actual or planned) of this particular plant.

### yearPlanted (mandatory)

### timePlanted (optional)

### genus (mandatory)

### species (mandatory)

### taxonomyLink (optional)

## Fencing



