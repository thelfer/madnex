---
title: Storage of `MTest` tests in `MADNEX` files and usage
author: Thomas Helfer
date: 2022
lang: en-EN
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
pandoc -f markdow+tex_math_single_backslash -F pandoc-crossref mtest-tests-storage.md -o mtest-tests-storage.md
-->

This document specifies how an `MTest` test is stored in a `madnex` file
and discusses how to execute this test. It extends the [description of
the `madnex` file regarding how to store material
knowledge](mfront-implementations-storage.html).

![Storage of an `MTest` test in a `madnex` file](img/MTestTestStorage.svg "Storage of an `MTest` test in a `madnex` file"){#fig:madnex:MTestTestStorage width=100%}

Figure @fig:madnex:MTestTestStorage shows how an `MTest` file is stored
in a `madnex` file. Note that an `MTest` test is always associated with
a behaviour.

# Group associated with a test

## The `test` data set (required)

The group associated with a behaviour shall contains a data set called
`test` storing the `MTest` test.

## The `metadata` data set (optional)

The `metadata` data set is a compound which may alter the meta data of
the `MFront` implementation. The following entries are allowed, but not
required:

- `author`: name of the person in charge of the identification
- `date`: date at which the identification of the behaviour took place
- `description`: description of this new behaviour
- `interfaces`: list of compatible interfaces