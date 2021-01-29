#!bin/bash

target_Format='.jpg'

for unsrt_File in $( ls *$target_Format )
do
    sorted_File=$( date '+%Y_%m_%dT%H_%M_%M' -d @$( stat -c %Y $unsrt_File ) )
    mv $unsrt_File $sorted_File$target_Format
    echo "$unsrt_File -> $sorted_File$target_Format"
done
