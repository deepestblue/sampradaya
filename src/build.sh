#!/bin/bash

set -e

catch() {
    if [ "$1" == "0" ]; then
        return
    fi
    rm $typefacename
}

trap 'catch $?' EXIT

typefacename=$1
src=$(dirname $0)

$src/sfd2ttf.pe ${typefacename%.ttf}.sfd $typefacename
$src/add-non-English-names.py --ta சம்பிரதாயம் --sa 𑌸𑌮𑍍𑌪𑍍𑌰𑌦𑌾𑌯𑌃 $typefacename
gftools fix-unwanted-tables --tables FFTM $typefacename
gftools fix-dsig --autofix $typefacename
