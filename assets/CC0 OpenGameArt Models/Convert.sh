#!/bin/bash
#~ MP4FILE=$(ls ./ |grep .zip)
#~ for filename in $MP4FILE
#~ do 
 #~ name=`echo "$filename" | sed -e "s/.zip$//g"`
 #~ mkdir ./$name/
 #~ unzip $filename -d ./$name/
#~ done
MP4FILE=$(ls ./ |grep .7z)
for filename in $MP4FILE
do 
 name=`echo "$filename" | sed -e "s/.7z$//g"`
 7za e $filename
done
