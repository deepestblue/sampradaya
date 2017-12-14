#!/bin/bash

set -o errexit
set -o nounset

clang++ -std=c++1y -Weverything -ferror-limit=1 -ftemplate-backtrace-limit=1 -Wno-missing-prototypes -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-disabled-macro-expansion -Wno-global-constructors -Wno-exit-time-destructors -Wno-float-equal -I/usr/local/opt/qt/include --system-header-prefix=QtGui/ --system-header-prefix=QtWidgets/ --system-header-prefix=boost/ -F/usr/local/opt/qt/lib -framework QtGui -framework QtCore -framework QtWidgets rasterise_text.cxx -o rasterise_text.out

for i in cases/*.txt; do
    j=${i#cases/}
    output_dir=master_images/${j%.txt}
    mkdir -p $output_dir
    rm -f $output_dir/*.png
    ./rasterise_text.out $i $output_dir
done