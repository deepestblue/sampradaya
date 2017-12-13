#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -o errexit
set -o nounset

clang++ -std=c++1y -Weverything -ferror-limit=1 -ftemplate-backtrace-limit=1 -Wno-missing-prototypes -Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-disabled-macro-expansion -Wno-global-constructors -Wno-exit-time-destructors -Wno-float-equal -I/usr/local/opt/qt/include --system-header-prefix=QtGui/ --system-header-prefix=QtWidgets/ --system-header-prefix=boost/ -F/usr/local/opt/qt/lib -framework QtGui -framework QtCore -framework QtWidgets update_master.cxx -o update_master.out

for i in cases/*.txt; do
    j=${i#cases/}
    output_dir=master_images/${j%.txt}
    touch $i
    mkdir -p $output_dir
    rm -f $output_dir/*.png
    ./update_master.out $i $output_dir
done