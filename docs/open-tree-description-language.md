# Open Tree Description Language

## Stuart Staniford

## v 0.0.1 1/5/2021

***

## Introduction

This specification describes the Open Tree Description Language (OTDL).  This is a JSON file format intended for describing a particular species of tree (or other woody plant, but the design focus is
on trees).  

Files formatted according to this specification typically use a .otdl extension.  Mime type is tbd, but
application/json should work until something more specific is available.  All OTDL files should be fully legal JSON.  For the purposes of this specification, the JSON format is defined by [RFC 8529] (https://tools.ietf.org/html/rfc8259).  The one exception is that OTDL explicitly allows the use of C++
style comments (ie beginning with //).  OTDL itself does not specificy any particular comments, and
OTDL parsers should ignore all comments - they are simply for human-readable annotations to the 
file.

The goal of an OTDL format file is to describe the way in which a species of tree may grow in order to support:
  * Visualization of the tree at varying points in its life.
  * Distinguishing individuals of this species of tree from individuals of other species.
  * Estimation of the carbon uptake or release of the tree.
  * Estimation of the useful products of the tree (fruit, nuts, timber).
  * Simulation of the interaction of this tree with other neighboring trees (eg shading leading to death or deformation, risk of fire spread, changes in the form of the tree from more spreading park trees vs taller forest trees).
  
  ## Overall Structure of the File

  An OTDL file consists of a single JSON object which represents one particular species (or variant, hybrid, etc).  


