#!/bin/bash

mkdir tmp/wav
mkdir tmp/mp3

cd tmp
rm filelist.txt
touch filelist.txt

cd converted

for f in *.mid
do
        fn=${f%.mid}
        wav=`echo ${fn##*/}.wav`
        mp3=`echo ${fn##*/}.mp3`
        echo Wav file: $wav
        echo MP3 file: $mp3
        timidity "$f" -Ow
        cp "$wav" ../wav/
        rm "$wav"
        ffmpeg -i "../wav/$wav" -c:a mp3 "../mp3/$mp3"
        echo file \'mp3/${mp3}\' >> ../filelist.txt
done

cd ..
cat filelist.txt

ffmpeg -f concat -safe 0 -i filelist.txt -c copy all.mp3

