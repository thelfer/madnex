---
title: Storage of material knowledge implementation written in `MFront` in `MADNEX` files and its usage
author: Thomas Helfer, François Curtit, Jean-François Rit, Shun Huang, Charles Toulemonde
date: 2020
lang: en-EN
numbersections: true
toc: true
documentclass: article
from: markdown+tex_math_single_backslash
geometry:
  - margin=2cm
papersize: a4
link-citations: true
colorlinks: true
figPrefixTemplate: "$$i$$"
tabPrefixTemplate: "$$i$$"
secPrefixTemplate: "$$i$$"
eqnPrefixTemplate: "($$i$$)"
bibliography: bibliography.bib
csl: iso690-numeric-en.csl
---

<!--
pandoc -f markdow+tex_math_single_backslash -F pandoc-crossref mfront-behaviours-storage.md -o mfront-behaviours-storage.md
-->

This document specifies how an `MFront` behaviour is stored in a
`MADNEX` file and discusses how information about such an `MFront`
behaviour can be queried using `mfront-query` and compiled using
`mfront`.

# Storage of material knowledge in `madnex` files

![Hierarchy of a `MADNEX` file](img/MFrontBehaviourStorage.pdf "Hierarchy of a `MADNEX` file"){#fig:madnex:MFrontBehaviourStorage}

Figure @fig:madnex:MFrontBehaviourStorage shows how material knowledge
implementation are stored in a `madnex` file. Three kinds of material
knowledge implementations are supported:

- Material properties.
- Behaviours.
- Models.

For the sake of simplicity, only behaviours are considered in the rest
of this document.

This hierarchy is the same of the one adopted in the `MFrontMaterials`
project.

## About material identifiers

As depicted on Figure @fig:madnex:MFrontBehaviourStorage, `MFront`
implementations are stored under the `MFront` group. Then the
implementations are sorted using a material identifier. This material
identifier is optional and behaviours may be directly stored in a
`Behaviours` group directly under the `MFront` group[^1].

Material identifiers must conform the restrictions imposed by `MFront`
[^2].

[^1]: This implies that `Behaviours`, `Material Properties` and `MOdels`
  can't be used as material identifiers.
[^2]: See the documentation of the `@Material` keyword. Those
  restrictions mostly states that the material' name must be a valid
  `C++` identifier.

## About behaviour identifiers

Behaviour identifiers must be unique and follow the restrictions of the
behaviour names imposed by `MFront`[^3].

[^3]: See the documentation of the `@Behaviour` keyword. Those
  restrictions mostly states that the behaviour' name must be a valid
  `C++` identifier.

## Global identifier

The combination of material identifier and behaviour identifier shall
result in a unique identifier. A `madnex` file containing two material
knowledge with the same identifiers is ill-formed.

## Group associated with a behaviour

The identification a behaviour starts from an existing implementation
file whose parameters or material properties are fitted to adjust
experimental data. The structure of the group associated with a
behaviour reflects this.

### The `source` data set (required)

The group associated with a behaviour shall contains a data set called
`source` storing an `MFront` implementation. This implementation serves
as the basis for building the final behaviour, which means that the
information it contains may be overridden by information retrieved in
the other data set of the group.

### The `metadata` data set (optional)

The `metadata` data set is a compound which may alter the meta data of
the `MFront` implementation. The following entries are allowed, but not
required:

- `author`: name of the person in charge of the identification
- `date`: date at which the identification of the behaviour took place
- `description`: description of this new behaviour

### The `parameters` data set (optional)

The `parameters` data set allows:

- Redefining material properties as parameters.
- Assigning new default values to parameters.

The parameters names must be a subset of the parameters or material
properties defined in the `source` data set.

### The `bounds` data  (optional)

The bounds data shall reflect the domain over which the identification
is assumed to be valid.

# `mfront-query` usage



~~~~{.bash}
$ `mfront-query` --list-materials file.madnex
~~~~

~~~~{.bash}
$ `mfront-query` --material=<material_id> --list-behaviours file.madnex
~~~~

~~~~{.bash}
$ `mfront-query` --list-behaviours file.madnex
~~~~


# `mfront` usage

~~~~{.bash}
$ mfront --obuild --interface=generic --material=<material_id> --behaviour=<behaviour_id> file.madnex
~~~~

The '--behaviour' option is not mandatory when only one behaviour is
stored.

~~~~{.bash}
$ mfront --obuild --interface=generic --material=<material_id> --all-behaviours file.madnex
~~~~

~~~~{.bash}
$ mfront --obuild --interface=generic --all-behaviours file.madnex
~~~~

# `mfront` python module

<http://tfel.sourceforge.net/mfront-python.html>

~~~~{.python}
import mfront

dsl = mfront.getDSL("file.madnex",<material_id>,<behaviour_id>)
dsl.analyseFile("file.madnex",<material_id>,<behaviour_id>)
~~~~

~~~~{.python}
import mfront

dsl = mfront.getDSL("file.madnex",<behaviour_id>)
dsl.analyseFile("file.madnex",<behaviour_id>)
~~~~

# Discussion

This document describes only the part of the data-model relating to the
material behavior gathered in the first level group named `MFront`.

A second 1st level group named `Experimental data` will contain the
experimental test data used to identify or validate the constitutive
laws. Each test will be structured according to the current data model
of the `CADEEX` pivot format. A third one named `Identification Study`
will be able to capitalize on the different identification scenarios
carried out with the `LC0D` component. (hypotheses, tests used, ...).

The source code hosted in the source group can contain parameters with
values different from those capitalized in the `Parameters` group. In
standard use with `MFrontMaterials` or `LC0D`, the compiled law will
automatically integrate the values from the `Parameters` group. It will
however be necessary to think of a means to avoid any ambiguity in other
case (for example, edition of the file with hdfview)

This data model corresponds to `MFront`’s needs and makes it possible to
start the first phase of development. It may be extended in the future,
in particular with context data:

- Status of the law (under development, validated for industrial use,
  etc.)
- Workflow elements (validator, dates, etc.)
- etc.