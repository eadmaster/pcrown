#!/bin/bash

export CD_PATH=../../cd
export TRANSLATED_SCRIPT_PATH=../../script/eng
export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (patched).bin"

# patch the font
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" KANJI.BIN  > /dev/null
xdelta3 -f -d -s KANJI.BIN KANJI.BIN.xdelta KANJI_ENG.BIN  # apply English font patch
#cp KANJI_ENG_narrow.BIN KANJI_ENG.BIN
#cp KANJI_ENG_couriernew.BIN KANJI_ENG.BIN
#cp KANJI_ENG_geomon5px.BIN KANJI_ENG.BIN

# patch items and names
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" 0.BIN  > /dev/null
sed 's/▼ //g; s/♂ //g; s/◇ //g'  ${TRANSLATED_SCRIPT_PATH}/items_ex.txt > ${TRANSLATED_SCRIPT_PATH}/items.txt  # remove EX icons
iconv -f UTF-8 -t SHIFT-JIS ${TRANSLATED_SCRIPT_PATH}/items.txt -o ${TRANSLATED_SCRIPT_PATH}/items.txt.sjis
wine itemsutil.exe -i ${TRANSLATED_SCRIPT_PATH}/names.txt ${TRANSLATED_SCRIPT_PATH}/items.txt.sjis  0.BIN  KANJI_ENG.BIN  0xC60   # 0xC60 = starting write offset in KANJI_ENG.BIN, ranges are in itemsutils/main.cpp
# TODO: make sure items are not truncated -> there is "FFE88277F1021221121060000203FE02" in KANJI_ENG.BIN -> decrease 0xC60 if needed, not below 0x29A   https://github.com/eadmaster/pcrown/issues/93#issuecomment-2629532940

# make chars spacing smaller  https://github.com/eadmaster/pcrown/issues/1#issuecomment-2439672329
#060729A8    E204
#06072994    E204
#0607297C    E204
#hexcalc '060729A8 - 06004000 + 1' = 6E9A9
#hexcalc '06072994 - 06004000 + 1' = 6E995
#hexcalc '0607297C - 06004000 + 1' = 6E97D
sfk setbytes 0.BIN 0x6E9A9 0x03 -yes
sfk setbytes 0.BIN 0x6E995 0x03 -yes
sfk setbytes 0.BIN 0x6E97D 0x03 -yes

# dialog choice fixes  https://github.com/eadmaster/pcrown/issues/1#issuecomment-2439839754
#06074248    04 = 70248
#060742A2    04 = 702A2
#0607401A    04 = 7001A
#06074020    04 = 70020
#060742EE    88 = 702EE
#060742BA    3A = 702BA
sfk setbytes 0.BIN 0x70249 0x03 -yes
sfk setbytes 0.BIN 0x702A3 0x03 -yes
sfk setbytes 0.BIN 0x7001B 0x03 -yes
sfk setbytes 0.BIN 0x70021 0x03 -yes
sfk setbytes 0.BIN 0x702EF 0x89 -yes
sfk setbytes 0.BIN 0x702BB 0x3A -yes

# OLD 6px font:
#cp KANJI_ENG_Traysia.BIN KANJI_ENG.BIN
#sfk setbytes 0.BIN 0x6E9A9 0x04 -yes
#sfk setbytes 0.BIN 0x6E995 0x04 -yes
#sfk setbytes 0.BIN 0x6E97D 0x04 -yes
#
#sfk setbytes 0.BIN 0x70249 0x04 -yes
#sfk setbytes 0.BIN 0x702A3 0x04 -yes
#sfk setbytes 0.BIN 0x7001B 0x04 -yes
#sfk setbytes 0.BIN 0x70021 0x04 -yes
#sfk setbytes 0.BIN 0x702EF 0x88 -yes
#sfk setbytes 0.BIN 0x702BB 0x3A -yes

# items description fixes / double-line hack  https://github.com/eadmaster/pcrown/issues/57
sfk setbytes 0.BIN 0x70603 0x14 -yes  # X position of the price in the store (max=1A)
# Tile map address (window #1)
sfk setbytes 0.BIN 0x47732 0xB308 -yes
# Tile map address (window #2)
sfk setbytes 0.BIN 0x3BE09 0x0AB308 -yes
# Height of window #1
sfk setbytes 0.BIN 0x476FF 0x05 -yes
# Height of window #2
sfk setbytes 0.BIN 0x3BDD5 0x05 -yes
# Y position of window #1
sfk setbytes 0.BIN 0x47707 0x17 -yes
# Y position of window #2
sfk setbytes 0.BIN 0x3BDDD 0x17 -yes
# Y position of text
sfk setbytes 0.BIN 0x75F7E 0xBC -yes
# Line Scroll Range Fix
sfk setbytes 0.BIN 0x3BA61 0xB8 -yes
# Clearing the description window
sfk setbytes 0.BIN 0x47773 0x05 -yes
sfk setbytes 0.BIN 0x4777B 0x17 -yes

# fix Engrish text in main program (opening, etc) https://github.com/eadmaster/pcrown/issues/94
source _patch_ascii.sh

# statically enable debug mode (press Start on 2nd pad to navigate event files) https://web.archive.org/web/20200918203538/https://github.com/cyberwarriorx/pcrown/wiki/Debugging
#sfk setbytes 0.BIN 0x1EB7F 0x01 -yes

# add splashscreen -> freezes with cheats enabled during boot  https://github.com/eadmaster/pcrown/issues/96
#mv 0.BIN  0_org.BIN
#wine mksplash.exe ../mksplash/splash_loader.bin ../mksplash/splash.bmp 0_org.BIN 0.BIN
#rm 0_org.BIN

# add version number in title screen  https://github.com/eadmaster/pcrown/issues/96
# "@SEGA ENTERPRISES,LTD.& ATLUS,1997" ->  "@SEGA & ATLUS,1997    T-ENG V0.X.Y"
sfk setbytes 0.BIN 0xA4137  "& ATLUS,1997   T-ENG V1.0RC1"  -yes

cd-replace "$PATCHED_IMAGE_FILE" 0.BIN  0.BIN  > /dev/null
cd-replace "$PATCHED_IMAGE_FILE" KANJI.BIN  KANJI_ENG.BIN  > /dev/null
