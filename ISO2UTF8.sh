#!/bin/bash

for f in $(find . | xargs file | grep "ISO-8859 text" | awk '{ printf $1"\n" }' | tr -d ":") ; do
    echo $f
    temp=$(mktemp)
    iconv -f ISO-8859-15 -t UTF-8 $f > $temp
    mv -f $temp $f
done