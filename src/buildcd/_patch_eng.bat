@echo off

echo extract "Princess Crown (Japan) (1M).zip" in this path: "%CD%"
pause

iat.exe -i "Princess Crown (Japan) (1M) (Track 01).bin" -o "Princess Crown (Japan) (1M) (Track 01).iso"  --iso

xdelta3.exe -d -s  "Princess Crown (Japan) (1M) (Track 01).iso"  "Princess.Crown.Japan.1M.Track.01.iso.xdelta"  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 

echo use this cue sheet to play the game: "Princess Crown (Japan) (1M) (English).cue"

pause