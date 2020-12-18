# Open Landscape Description Format

## Stuart Staniford

## v 0.0.1 12/18/2020

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
     "spaceUnits":  "feet",
     "baseYear":    2020,
     "author":      "Stuart Staniford",
     "version":     [0.0.1]
    }
 "landSurface":
  {
   ...
  }
 "plants":
  {
   ...
  }  
 "boundaries":
  {
   ...
  }  
 "fencing":
  {
   ...
  }  
}
```
## Introductory Data

The `introductoryData` section has metadata about the file and global information required to interpret the file.  It has certain mandatory name/value pairs that must be present, and then others that are optional.  We here take them in turn.

### spaceUnits (mandatory)

Syntax: `"spaceUnits":  "<units>",

where `<units>` is a JSON string allowed only to be one of either `feet` or `meters`, reflecting either Imperial/SAE or SI units of measurement.  A file must pick exactly one such unit and use it throughout for coordinates of geometry in a variety of other sections.  There is no provision for direct use of other sub-units such as inches, yards, or centimeters.  Generally geometric co-ordinates will be specified as JSON numbers, and thus decimal fractions of the measurements are allowed.  Thus a length of twelve feet, three inches would be expressed as 12.25.

## Land Surface


## Boundaries


## Fencing


## Plants


