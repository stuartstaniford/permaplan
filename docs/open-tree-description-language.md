# Open Tree Description Language

## Stuart Staniford

## v 0.0.1 1/5/2021

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
  
  ## Overall Structure of the File

  An OTDL file consists of a single JSON object which represents one particular species (or variant, hybrid, etc).  The OTDL object consists of a series of JSON objects which are the values of subobjects inside, and associated with names of the master OTDL object.  Various of these subobjects are described in the remaining sections of this specification.  

However, the important master principle is that implementations coming across sub-objects with tag names they do not understand should ignore those sub-objects.  This allows experimental or implementation-specific extensions of the file format.  Implementors of such extensions should be aware that there is no guarantee that arbitrary name choices may not be over-ridden by future versions of this specification.  However, the specification commits to never create new sub-objects (or sub-sub-objects) that begin with the sequence 'X-', thus meaning they are safe for proprietary/experimental extensions (except that different proprietary extensions might conflict with each other, and at present this specification does not provide any facilities to prevent such collisions.)

### Overview Example

This gives an example showing the major sections of an OTDL object in a file (details are suppressed with ellipses).  Each of these sections is treated at length in subsequent sections, but the example should serve to make the overall structure clearer.  
  * The `introductoryData` section has metadata about the file and global information required to interpret the file (eg choices of units).  
  * The `landSurface` section describes the shape of the relief of this particular piece of land.
  * The `plants` section describes plants either present on the land, or planned for the future.
  * The `boundaries` section contains the coordinates of an enclosing polygon of the land described by this OLDF object.
  * The `fencing` section describes the geometry of fencing present or planned on the land.

```
// Example OTDL object 
{
  "overviewData":
    {
     "spaceUnits":        "feet",
     "version":           [0, 0, 2],
     "fileTime":          [<seconds>, <microseconds>],
     "baseYear":          2020,
     "software":          "permaplan",
     "softwareVersion":   "1.2.3",
     "author":            "Stuart Staniford"
    },
 "trunk":
  {
   ...
  },
 "branches":
  {
   ...
  },
 "leaves":
  {
   ...
  },  
}
```

### Character sets and internationalization

All JSON names in objects should be expressed in 8 bit ASCII characters, as should all JSON numerical values and JSON structure such as {, }, [, etc.  However, the contents of strings in values are expressed in UTF8 characters and thus may represent any language or alphabet that can be captured via UTF8.  UTF8 is as described in [RFC 3629](https://tools.ietf.org/html/rfc3629).  The OTDL specification provides facilities for internationalization (such as, for example, providing common names of a given tree species in a range of languages and character sets).
