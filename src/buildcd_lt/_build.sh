 #!/bin/bash
 
 
# Requirements:
# - pre-patched "../buildcd/Princess Crown (Japan) (1M) (Track 01) (patched).bin"

export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (patched).bin"

rm "$PATCHED_IMAGE_FILE"
cp "../buildcd/$PATCHED_IMAGE_FILE" .

# repatch 0.BIN, KANJI.BIN
source _patch_items.sh

# build xdelta patch (bin)
xdelta3 -S none -f -e -s "../buildcd/Princess Crown (Japan) (1M) (Track 01).bin" "$PATCHED_IMAGE_FILE"  "Princess.Crown.Japan.1M.Track.01.LT.bin.xdelta"
