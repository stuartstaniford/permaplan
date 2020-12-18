# Open Landscape Description Format

## Stuart Staniford

## v 0.0.1 12/18/2020

***

## Introduction

This specification describes the Open Landscape Description Format (OLDF).  This is a JSON file format intended for describing a section of the earth's surface that is, or will be, landscaped.  The file format originated as the file format for the Permaplan software for landscape planning with a permaculture flavor, but the OLDF format itself is not restricted and may be used by other software for the same or related purposes.

Files formatted according to this specification typically use a .oldf extension.  Mime type is tbd, but
application/json should work until something more specific is available.  All OLDF files should be fully legal JSON.  For the purposes of this specification, the JSON format is defined by [RFC 8529] (https://tools.ietf.org/html/rfc8259).
