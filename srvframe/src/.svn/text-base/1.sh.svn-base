#!/bin/bash
filelist=`find . -name "*.*"`
for file in $filelist
do
     if [ $file != "./1.sh" ] 
     then
     sed -e "s///g" $file > tmp.txt
     mv tmp.txt $file
     fi
done
