#!/bin/bash

typefacename=$1

$(dirname $0)/sfd2ttf.pe ${typefacename%.ttf}.sfd $typefacename
gftools-fix-unwanted-tables.py $typefacename
gftools fix-dsig --autofix $typefacename
