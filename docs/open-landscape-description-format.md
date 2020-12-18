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


