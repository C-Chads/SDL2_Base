#!/bin/bash
IFS=$'\n' JPGFILE=$(ls -1|grep '.\.jpg')
for filename in $JPGFILE
do
 echo "Found file $filename"
 convert $filename -resize 512x512 512x512/$filename
 #convert $filename -resize 1024x1024 1024x1024/$filename
 convert $filename -resize 256x256 256x256/$filename
done