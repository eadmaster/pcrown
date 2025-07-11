#!/bin/bash

export CD_PATH=../../cd
export TRANSLATED_SCRIPT_PATH=../../script/eng
export PATCHED_IMAGE_FILE="Princess Crown (Japan) (Rev A) (11M) (Track 01) (patched).bin"

# patch the font
7z e -y "Princess Crown (Japan) (Rev A) (11M) (Track 01).iso" KANJI.BIN  > /dev/null
xdelta3 -f -d -s KANJI.BIN ../buildcd/KANJI.BIN.xdelta KANJI_ENG.BIN  # apply English font patch
#cp KANJI_ENG_narrow.BIN KANJI_ENG.BIN
#cp KANJI_ENG_couriernew.BIN KANJI_ENG.BIN
#cp KANJI_ENG_geomon5px.BIN KANJI_ENG.BIN

7z e -y "Princess Crown (Japan) (Rev A) (11M) (Track 01).iso" 0.BIN  > /dev/null

# patch items and names
# standard build:
#sed 's/▼ //g; s/♂ //g; s/◇ //g'  ${TRANSLATED_SCRIPT_PATH}/items_ex.txt > ${TRANSLATED_SCRIPT_PATH}/items.txt  # remove EX icons
#iconv -f UTF-8 -t SHIFT-JIS ${TRANSLATED_SCRIPT_PATH}/items.txt -o ${TRANSLATED_SCRIPT_PATH}/items.txt.sjis
#wine itemsutil.exe -i ${TRANSLATED_SCRIPT_PATH}/names.txt ${TRANSLATED_SCRIPT_PATH}/items.txt.sjis  0.BIN  KANJI_ENG.BIN  0xC60   # 0xC60 = starting write offset in KANJI_ENG.BIN, ranges are in itemsutils/main.cpp
# TODO: make sure items are not truncated -> there is "FFE88277F1021221121060000203FE02" in KANJI_ENG.BIN -> decrease 0xC60 if needed, not below 0x29A   https://github.com/eadmaster/pcrown/issues/93#issuecomment-2629532940

# LT build:
iconv -f UTF-8 -t SHIFT-JIS ${TRANSLATED_SCRIPT_PATH}/items_lt.txt -o ${TRANSLATED_SCRIPT_PATH}/items_lt.txt.sjis
wine itemsutil.exe -i ${TRANSLATED_SCRIPT_PATH}/names.txt ${TRANSLATED_SCRIPT_PATH}/items_lt.txt.sjis  0.BIN  KANJI_ENG.BIN  0xEA0

# fix for RevA shifted addresses
mv 0.BIN 0.BIN.bak
sfk partcopy 0.BIN.bak -allfrom 0xB0 0.BIN -yes

# make chars spacing smaller  https://github.com/eadmaster/pcrown/issues/1#issuecomment-2439672329
#060729A8    E204
#06072994    E204
#0607297C    E204
#hexcalc '060729A8 - 06004000 + 1' = 6E9A9
#hexcalc '06072994 - 06004000 + 1' = 6E995
#hexcalc '0607297C - 06004000 + 1' = 6E97D
sfk setbytes 0.BIN $((0x6E9A9+0x8)) 0x03 -yes
sfk setbytes 0.BIN $((0x6E995+0x8)) 0x03 -yes
sfk setbytes 0.BIN $((0x6E97D+0x8)) 0x03 -yes
#shifted:
#sfk setbytes 0.BIN 0x6EA61 0x03 -yes
#sfk setbytes 0.BIN 0x6EA35 0x03 -yes
#sfk setbytes 0.BIN 0x6EA4D 0x03 -yes

# dialog choice fixes  https://github.com/eadmaster/pcrown/issues/1#issuecomment-2439839754
#06074248    04 = 70248
#060742A2    04 = 702A2
#0607401A    04 = 7001A
#06074020    04 = 70020
#060742EE    88 = 702EE
#060742BA    3A = 702BA
sfk setbytes 0.BIN $((0x70249+0x8))  0x03 -yes
sfk setbytes 0.BIN $((0x702A3+0x8))  0x03 -yes
sfk setbytes 0.BIN $((0x7001B+0x8))  0x03 -yes
sfk setbytes 0.BIN $((0x70021+0x8))  0x03 -yes
sfk setbytes 0.BIN $((0x702EF+0x8))  0x89 -yes
sfk setbytes 0.BIN $((0x702BB+0x8))  0x2F -yes  # bottom padding on the selection block

# items description fixes / double-line hack  https://github.com/eadmaster/pcrown/issues/57
sfk setbytes 0.BIN $((0x70603+0x8))  0x14 -yes  # X position of the price in the store (max=1A)
# Tile map address (window #1)
#sfk setbytes 0.BIN $((0x47732+0x8))  0xB308 -yes
# Tile map address (window #2)
#sfk setbytes 0.BIN $((0x3BE09+0x8))  0x0AB308 -yes
# Height of window #1
#sfk setbytes 0.BIN $((0x476FF+0x8))  0x05 -yes
# Height of window #2
#sfk setbytes 0.BIN $((0x3BDD5+0x8))  0x05 -yes
# Y position of window #1
#sfk setbytes 0.BIN $((0x47707+0x8))  0x17 -yes
# Y position of window #2
#sfk setbytes 0.BIN $((0x3BDDD+0x8))  0x17 -yes
# Y position of text
#sfk setbytes 0.BIN $((0x75F7E+0x8))  0xBC -yes
# Line Scroll Range Fix
#sfk setbytes 0.BIN $((0x3BA61+0x8))  0xB8 -yes
# Clearing the description window
#sfk setbytes 0.BIN $((0x47773+0x8))  0x05 -yes
#sfk setbytes 0.BIN $((0x4777B+0x8))  0x17 -yes

# fix Engrish text in main program (opening, etc) https://github.com/eadmaster/pcrown/issues/94
#source ../buildcd/_patch_ascii.sh

# statically enable debug mode (press Start on 2nd pad to navigate event files) https://web.archive.org/web/20200918203538/https://github.com/cyberwarriorx/pcrown/wiki/Debugging
#sfk setbytes 0.BIN 0x1EB7F 0x01 -yes

# add splashscreen -> freezes with cheats enabled during boot  https://github.com/eadmaster/pcrown/issues/96
#mv 0.BIN  0_org.BIN
#wine mksplash.exe ../mksplash/splash_loader.bin ../mksplash/splash.bmp 0_org.BIN 0.BIN
#rm 0_org.BIN

# add version number in title screen  https://github.com/eadmaster/pcrown/issues/96
# "@SEGA ENTERPRISES,LTD.& ATLUS,1997" ->  "@SEGA & ATLUS,1997    T-ENG V0.X.Y"
sfk setbytes 0.BIN 0xA4137  "& ATLUS,1997      T-ENG V1.1"  -yes

# fix for RevA shifted addresses
sfk partcopy 0.BIN -allfrom 0x0 0.BIN.bak 0xB0 -yes

cd-replace "$PATCHED_IMAGE_FILE" 0.BIN  0.BIN.bak  > /dev/null
cd-replace "$PATCHED_IMAGE_FILE" KANJI.BIN  KANJI_ENG.BIN  > /dev/null
