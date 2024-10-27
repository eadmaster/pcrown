@echo off

echo extract "Princess Crown (Japan) (1M).zip" in this path: "%CD%"
pause

REM download patching tools
curl.exe -kLO "https://raw.githubusercontent.com/eadmaster/pcrown/refs/heads/master/src/buildcd/iat.exe"
curl.exe -kLO "https://raw.githubusercontent.com/eadmaster/pcrown/refs/heads/master/src/buildcd/cygwin1.dll"
curl.exe -kLO "https://raw.githubusercontent.com/eadmaster/pcrown/refs/heads/master/src/buildcd/xdelta3.exe"

iat.exe -i "Princess Crown (Japan) (1M) (Track 01).bin" -o "Princess Crown (Japan) (1M) (Track 01).iso"  --iso

curl.exe -kLO "https://github.com/eadmaster/pcrown/releases/latest/download/Princess.Crown.Japan.1M.Track.01.iso.xdelta"

xdelta3.exe -d -s  "Princess Crown (Japan) (1M) (Track 01).iso"  "Princess.Crown.Japan.1M.Track.01.iso.xdelta"  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 

del /F /Q "Princess Crown (Japan) (1M) (Track 01).iso"

curl.exe -kL "https://raw.githubusercontent.com/eadmaster/pcrown/refs/heads/master/src/buildcd/Princess%20Crown%20(Japan)%20(1M)%20(English).cue" -o "Princess Crown (Japan) (1M) (English).cue"

echo use this cue sheet to play the game: "Princess Crown (Japan) (1M) (English).cue"

pause
