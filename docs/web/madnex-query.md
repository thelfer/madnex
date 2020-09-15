---
title: madnex-query
author: Thomas Helfer
date: 07/2020
lang: en-EN
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tabPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
---

The `madnex-query` allows querying a `MADNEX` file from the command
line.

# Options

Valid options are :

- `--list-behaviours`: list all available `MFront` behaviours
- `--list-material-properties`: list all available `MFront` material
  properties
- `--list-materials`: list all materials in the `MFront` subgroup
- `--list-models`: list all available `MFront` models
- `--material`: specify the material name. <none> may be used to select
  material properties, behaviours or models not associated with a
  material

# Example of usage

~~~~{.bash}
$ madnex-query --list-materials MFront-test1.madnex 
- UO2
- Zircaloy4
~~~~
