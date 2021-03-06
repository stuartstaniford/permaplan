# Open Tree Description Language

## Stuart Staniford

## v 0.0.1 1/26/2021

***

## Introduction

This specification describes the Open Tree Description Language (OTDL).  This is a JSON file format intended for describing a particular species of tree (or other woody plant, but the design focus is on trees).  

Files formatted according to this specification typically use a .otdl extension.  Mime type is tbd, but application/json should work until something more specific is available.  All OTDL files should be fully legal JSON.  For the purposes of this specification, the JSON format is defined by [RFC 8529] (https://tools.ietf.org/html/rfc8259).  The one exception is that OTDL explicitly allows the use of C++ style comments (ie beginning with //).  OTDL itself does not specificy any particular comments, and OTDL parsers should ignore all comments - they are simply for human-readable annotations to the file.

The goal of an OTDL format file is to describe the way in which a species of tree may grow in order to support:
  * Visualization of the tree at varying points in its life.
  * Distinguishing individuals of this species of tree from individuals of other species.
  * Estimation of the carbon uptake or release of the tree.
  * Estimation of the useful products of the tree (fruit, nuts, timber).
  * Simulation of the interaction of this tree with other neighboring trees (eg shading leading to death or deformation, risk of fire spread, changes in the form of the tree from more spreading park trees vs taller forest trees).

### Conceptual Model

The simulation model of plant growth implied in this specification is what is known in botany/forestry as a Functional-Structural Plant Model (see [Sievanen et al](https://core.ac.uk/download/pdf/52626911.pdf) for a review).  This kind of model builds a detail model of the components of the tree (trunk, branches, leaves, sapwood vs heartwood, etc) and is capable of generating random instances of different kinds of trees with the characteristic behavior and growth form of that particular species or variety.

This kind of detailed model allows for detailed simulation of interaction effects between plants - for example, in a food forest with numerous plants of different sizes, will all plants receive enough light, water, etc?  Being able to simulate the growth of branches and roots in detail allows the possibility of modeling these kinds of interactions.  Likewise, the model is detailed enough to potentially answer questions like "if this tree is exposed to a two month drought, is it likely to survive", and "under what conditions can a fire spread through this collection of trees"

The model also allows for 3D visual rendering of landscapes of trees (and other plants).  The goal here is not to be photo-realistic, but rather have trees that are recognizable as to type and allow a general sense of what a particular landscape will look like in ten years or fifty years.

However, detailed low level simulation of large landscapes is typically inconsistent with real-time rendering of a scene (if every leaf in a 100 acres of forest or parkland must be individually modeled and rendered, that is not feasible at present).  For this reason, OTDL also has facilities for generating approximations that can be used at increasing distance.  We compute once what a "sugar maple in a forest at 1000 feet away" looks like, and then can use that approximation many times over when it's the right level of detail for drawing a scene.

For this purpose, OTDL uses a distributed computation scheme in which there is a central repository which stores intermediate representations, but computation of them is done by (many) clients.  The overall goal is to have a repository of many small(ish) objects, most of which change very rarely and are thus cache friendly, and to make the repository computation load small because clients do the work when each encounters a new need.  Thus the first time someone needs a maple at a thousand feet, that user will suffer some delay while his client computes a visual represention.  All subsequent users simply need to download and apply that object (which ideally will already be cached somewhere).

The repository may get very large, but individual landscapes are only likely to involve a few hundred, or in extreme cases a few thousand, species, and thus local storage needs will be more manageble.  

Thus this specification defines an abstract file tree structure which all OTDL implementations must share and use to manage data.

  ## Overall Structure of an OTDL object

  An OTDL file consists of a single JSON object which represents one particular species (or variant, hybrid, etc).  The OTDL object consists of a series of JSON objects which are the values of subobjects inside, and associated with names of the master OTDL object.  Various of these subobjects are described in the remaining sections of this specification.  

However, the important master principle is that implementations coming across sub-objects with tag names they do not understand should ignore those sub-objects.  This allows experimental or implementation-specific extensions of the file format.  Implementors of such extensions should be aware that there is no guarantee that arbitrary name choices may not be over-ridden by future versions of this specification.  However, the specification commits to never create new sub-objects (or sub-sub-objects) that begin with the sequence 'X-', thus meaning they are safe for proprietary/experimental extensions (except that different proprietary extensions might conflict with each other, and at present this specification does not provide any facilities to prevent such collisions.)

### Overview Example

This gives an example showing the major sections of an OTDL object in a file (details are suppressed with ellipses).  Each of these sections is treated at length in subsequent sections, but the example should serve to make the overall structure clearer.  
  * The `overviewData` section has metadata about the object and global information about this particular species or variety of plant (eg names, file version, etc).  
  * The `trunk` section describes information associated with the main trunk of the plant.
  * The `branches` section describes information about branching.
  * The `foliage` section describes the foliage of this particular species.

```
// Example OTDL object 
{
  "overviewData":
    {
     "genus":       "Pinus",
     "species":     "strobus",
     "maxHeight":   61000,
     "commonNames":  
       {
        "en-US": "Eastern White Pine",
        "en-GB": "Weymouth Pine",
        ...
       }
     "version":     [0, 0, 1],
     "fileTime":    [<seconds>, <microseconds>],
     "authors":     ["Stuart Staniford", ...]
    },
   "wood":
    {
    },
   "foliage":
    {
    },
   "parent": "<path>"
}
```

### Character sets and internationalization

All JSON names in objects should be expressed in 8 bit ASCII characters, as should all JSON numerical values and JSON structure such as {, }, [, etc.  However, the contents of strings in values are expressed in UTF8 characters and thus may represent any language or alphabet that can be captured via UTF8.  UTF8 is as described in [RFC 3629](https://tools.ietf.org/html/rfc3629).  The OTDL specification provides facilities for internationalization (such as, for example, providing common names of a given tree species in a range of languages and character sets).

## Overview Data (mandatory, non-heritable)

The `overviewData` section has metadata about the file and global information required to interpret the file.  It has certain mandatory name/value pairs that must be present, and then others that are optional.  We here take them in turn.

### fileTime (mandatory, non-heritable)

Syntax: `"fileTime":  [<seconds>, <microseconds>]`

The fileTime value is a JSON array of exactly two integers which denote the time at which the file was written.  The first integer is the number of seconds since the beginning of 1970, UTC (aka "Unix time") and the second integer is a number of microseconds (expressing the fractional part of a second).

### version (mandatory, heritable)

Syntax: `"version":  [<majorversion>, <minorversion1>, <minorversion2>]`

The version value is a JSON array of exactly three integers which denote the version of this specification that governs the format of the file.  The current version is `[0,0,1]`.  Version numbers will be incremented as the file format continues to evolve over time.  A general convention is that changes in minor version numbers will be expected to preserve backward compatability with older versions of OLDF parsing software.  However, this may not be the case with changes in major version number.  Major version 0 is used for versions of the specification prior to general release, while it is in early development and testing.

### genus (mandatory, heritable)

Syntax: `"genus": "<genus-name>",`

This is the scientific genus-name of the plant.  It should consist of a capital first letter, followed by one or more lower case letters (no characters outsided of `a-z` are allowed).  Species and Genus name are expected to be in ASCII latin.

### species (mandatory, heritable)

Syntax: `"species": "<species-name>",`

This is the scientific species-name of the plant.  It should consist of two or more lower case letters (no characters outsided of `a-z` are allowed).  Species name are expected to be in ASCII latin.

### var (optional, non-heritable)

Syntax: `"var": "<var-name>",`

This is a varietal, subspecies, or hybrid name of the plant.  It should consist of two or more characters which must be alphabetical (upper or lower case) or space, or the punctuation character '-'.

### commonNames (optional, heritable)

Syntax:  `"commonNames": { "en-US": "American Beech", "en-GB": "American Beech"},`

This is an object which has a number of language tags (drawn from RFC 5646), together with a common name for the tree in that particular language community.

### maxHeight (mandatory, heritable)

Syntax: `"maxHeight":   61000,`

The maximum height that this tree species is capable of growing to under ideal circumstances.

### maxGirth (mandatory, heritable)

Syntax: `"maxGirth":   1000,`

The maximum girth that this tree species is capable of growing to under ideal circumstances.  Generally taken to be the girth at chest height (4.5 feet or 1.4m above the ground level).  Note that the girth is the circumference, not the diameter (but is the most widely cited measurement)

### maxWidth (mandatory, heritable)

Syntax: `"maxWidth":   0.7,`

The maximum width (diameter) of the canopy that this tree species is capable of growing to under ideal circumstances (ie in an open field with excellent soil etc - trees growing in a forest stand will generally be much narrower).  This is expressed as a fraction of the maxHeight and this fraction is used to make sure trees are of roughly the right macro-shape.

### maxAge (mandatory, heritable)

Syntax: `"maxAge":   150,`

The maximum age in years that this tree species is capable of growing to under ideal circumstances.  

This is the specific scientific variety name of the plant if known. 

### taxonomyLink (optional, non-heritable)

Syntax: `"taxonomyLink": "<url>",`

or  `"taxonomyLink": ["<url1>", "<url2>", ...],`

This is a url to a formal taxonomic description of the exact species of plant being referenced in this particular plant object.  Eg this might be a link to the online Interagency Taxonomic Information System (itis.gov)

### authors (optional, heritable)

Syntax: `"authors":  "First Last",`
or
Syntax: `"authors":  {"F1 L1", "F2 L2", ...]`

The authors value is a JSON string to denote the human author of a particular OTDL object.  
The specification allows any UTF8 string and does not constrain to any particular convention of naming humans or organizations.  It's also legitimate to have authors go to an array of 
names, giving credit to multiple human authors who have worked on a particular file.


## Wood (mandatory, heritable)

The wood sub-object encomasses all information required to describe the woody parts of the plant.  The following tags are defined within the wood object.

### stemRate (mandatory, heritable)

Syntax: `"stemRate":  <sRate>,`

The stemRate is the maximum rate at which stems grow in length (ie the rate when all is well).  It is expressed in mm/year.

### branchSpacing (mandatory, heritable)

Syntax: `"branchSpacing":  <bspace>,`

The branchSpacing is the rate at which branches form along an existing branch (when all is well).  It is expressed in mm.

### branchFactor (mandatory, heritable)

Syntax: `"branchFactor":  <bfac>,`

### branchAngle (mandatory, heritable)

Syntax: `"branchAngle":  <angleDegrees>,`

The branchAngle is the angle at which a new branch initially heads away from the axis of the trunk or branch it is starting from.  It is expressed in degrees.

### branchSpiralAngle (mandatory, heritable)

Syntax: `"branchSpiralAngle":  <angleDegrees>,`

The branchSpiralAngle is the angle around the tree at which one set of branches will be offset from the last set of branches.  It is expressed in degrees.

### initSapThickness (mandatory, heritable)

Syntax: `"initSapThickness":  <iST>,`

The initSapThickness (initial sapwood thickness) is the thickness (radius) of the sapwood in a new stem (ie one in which there is no heartwood yet.  It is expressed in mm.

### initBarkThickness (mandatory, heritable)

Syntax: `"initBarkThickness":  <iBT>,`

The initBarkThickness (initial bark thickness) is the thickness of the bark in a new stem (ie one in which there is no heartwood yet.  It is expressed in mm.  Bark includes phloem for this purpose.

### barkColors (mandatory, heritable)

Syntax:
```
"barkColors":
 [
   {
    "ages": [0.0, 2.0],
    "rgb": [200, 200, 200]
   },
   {
    "ages": [2.0, 10000.0],
    "rgb": [230, 230, 230]
   }
 ]
```
barkColors specifies the colors of bark for woody stems, branches, etc for this species.  This information is used in situations where bark texture data is not available or else not advised to use
(eg from sufficient distance).

barkColors is an array of objects each of which specify the color based on the ages of the stems/branches in question.  Each object has an "ages" tag which has an array of two values, the age at which this entry becomes operative, and the age at which it becomes inoperative.  The second tag in each object is "rgb" which species an array of three values giving the red, green, and blue bytes of the color.

Note that
* In the barkColors array, the first entry must start at age 0.0.
* The intervals of successive entries in the array must touch (ie the next starts at the same place the last leaves off with no gap or overlap.
* The last array must end at 10000.0 (taken to be infinite age of the tree trunk).
* As a corollory of the above, the array must have at least one member (if there was exactly one member it should be [0,10000]).

## Foliage (mandatory, heritable)

### leafColors (mandatory, heritable)

Syntax:
```
"leafColors":
 {
    "spring": [40, 200, 50],
    "summer": [10, 200, 50],
    "fall": [200, 100, 10],
    "winter": "none"
  }
```
leafColors specifies the color of foliage (leaves, needles, etc).  It does so by the four separate seasons, which are tags in an object as above, each followed by an integer RGB array, or the special value "none" indicating that the tree has no leaves in that season (usually winter in temperate places, but sometimes a a dry season in other climates)

## Parent (optional)

Syntax: `"parent":  "<path>",`

OTDL has a simple inheritance structure implemented by the `parent` option.  If parent is present, it must have a string value which is a path to another OTDL file.  See the section below for the standard OTDL file structure.  If parent is present, and if an OTDL file can be found at `<path>`, then the current OTDL file is considered to inherit from that one.  What this means is that any parts of the OTDL DOM structure which are present in the parent file, but not present in the current file, the parent values will be used.  If values are present in the current OTDL object, they will override the parent ones.  This allows OTDL files for similar species to be more compact because a parent file can carry a lot of the similarities and the child file can only note things that are different in the more specific case.

For example, if Acer rubrum x Autumn Glory inherits from Acer rubrum, and Acer Rubrum has a maxAge of 100 years, but x Autumn Glory doesn't specify a maxAge, the maxAge from Acer rubrum without a varietal will be applied.


## File Structure for OTDL.

This file structure is described with respect to <ROOT>, which may be different in different places- for example, it may be a local subdirectory for an instance of OTDL software or it may be relative to some URL over HTTP(S).  This specification does not specify where <ROOT> is, but rather specifies a subdirectory tree underneath it.  Subdirectory names must be ASCII alpha-numeric plus `._-`.  The `/` character is used as a path divider, with the understanding that on file systems using a different path divider the correct one for local circumstances will be used (eg `\` on Microsoft Windows).

The first two levels of the file structure represent the genus and species of the plant in question, thus 

`<ROOT>/Acer/saccharum/`

is the subdirectory for OTDL resources for the sugar maple.
