# How to make a single mp3, which contains all songs
This method requires GNU/Linux OS.

## 1. Link approved compositions to single directory
Run bash script
```
bash make_single_mp3_part1.sh
```

## 2. Convert .tg files to .mid in TuxGuitar
1. Open latest TuxGuitar version.
2. From menubar, open Tools -> File format batch converter.
3. To ```Output File Format```, set newly created ```Midi (*.mid)```
4. To ```Input folder```, set newly created ```tmp/all_tg``` folder
5. To ```Input folder```, set newly created ```tmp/convert``` folder

## 3. Convert .mid files to .mp3, and finally join then to a single mp3 file
Run bash script
```
bash make_single_mp3_part2.sh
```
