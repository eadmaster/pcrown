#!/bin/bash

#PRE: install req. patching tools
#sudo apt install iat xdelta3

echo "extract \"Princess Crown (Japan) (1M)\" in this path: $(pwd)"
echo -n "Press any key to continue... "
read -s -n 1

# download the latest patch
curl -kLO https://github.com/eadmaster/pcrown/releases/latest/download/Princess.Crown.Japan.1M.Track.01.iso.xdelta

iat "Princess Crown (Japan) (1M) (Track 01).bin"  "Princess Crown (Japan) (1M) (Track 01).iso" 

xdelta3 -f -d -s  "Princess Crown (Japan) (1M) (Track 01).iso"  "Princess.Crown.Japan.1M.Track.01.iso.xdelta"  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 

rm "Princess Crown (Japan) (1M) (Track 01).iso"

# download the cue sheet
curl -kL 'https://raw.githubusercontent.com/eadmaster/pcrown/refs/heads/master/src/buildcd/Princess%20Crown%20(Japan)%20(1M)%20(English)%20(iso).cue' -o "Princess Crown (Japan) (1M) (English).cue"

echo "use this cue sheet to play the game: \"Princess Crown (Japan) (1M) (English).cue\""
