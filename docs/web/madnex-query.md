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

# Command line arguments

Valid command line arguments are :

- `--list-behaviours`: list all available `MFront` behaviours.
- `--list-material-properties`: list all available `MFront` material
  properties.
- `--list-materials`: list all materials in the `MFront` subgroup.
- `--list-models`: list all available `MFront` models.
- `--material`: specify the material name. <none> may be used to select
  material properties, behaviours or models not associated with a
  material.
- `--behaviour`: specify a behaviour.
- `--list-mtest-tests`: list all the tests associated with a behaviour
  (specified using the `--behaviour` command line argument) associated
  with a material (specified using the `--material` command line
  argument).

# Examples of usage

## List of materials

~~~~{.bash}
$ madnex-query --list-materials MFront-test1.madnex 
- UO2
- Zircaloy4
~~~~

## List of tests associated with a behaviour

~~~~{.bash}
$ madnex-query --material="<none>" --behaviour=Plasticity --list-mtest-tests Plasticity.mdnx 
- tests associated with behaviour 'Plasticity':
    - UniaxialTensileTest
~~~~