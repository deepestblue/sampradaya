#!/bin/bash

set -o errexit
set -o nounset

PROGRAM=$(basename $0)
TMPDIR=$(mktemp -d -t $PROGRAM)

./update_master.sh $TMPDIR

mkdir -p diffs
# Workaround bizarre issue, where the single command below doesn't work, in that it copies all files over.
# rsync -rc --compare-dest=master_images/ $TMPDIR/ diffs/
rsync -rc --compare-dest=$TMPDIR/ master_images/ diffs/
rsync -rc --existing $TMPDIR/ diffs/
find diffs/ -type d -depth -empty -exec rmdir "{}" \;
rm -r $TMPDIR
