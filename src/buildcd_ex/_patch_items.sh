#!/bin/bash

export TRANSLATED_SCRIPT_PATH=../../script/eng
export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (patched).bin"

# patch the font
7z e -y "../buildcd/Princess Crown (Japan) (1M) (Track 01).iso" KANJI.BIN
xdelta3 -f -d -s KANJI.BIN ../buildcd/KANJI.BIN.xdelta KANJI_ENG.BIN  # apply English font patch

# patch items and names
iconv -f UTF-8 -t SHIFT-JIS ${TRANSLATED_SCRIPT_PATH}/items_ex.txt -o ${TRANSLATED_SCRIPT_PATH}/items_ex.txt.sjis
wine ../buildcd/itemsutil.exe -i ${TRANSLATED_SCRIPT_PATH}/names.txt ${TRANSLATED_SCRIPT_PATH}/items_ex.txt.sjis  0.BIN  KANJI_ENG.BIN  0xEA0   # 0xEA0 = starting write offset in KANJI_ENG.BIN, ranges are in itemsutils/main.cpp

# make chars spacing smaller (thanks to paul_met and derek (ateam) for the tips) https://segaxtreme.net/threads/help-me-translate-princess-crown.18555/#post-186226
#060729A8    E204
#06072994    E204
#0607297C    E204
#hexcalc '060729A8 - 06004000 + 1' = 6E9A9
#hexcalc '06072994 - 06004000 + 1' = 6E995
#hexcalc '0607297C - 06004000 + 1' = 6E97D
sfk setbytes 0.BIN 0x6E9A9 0x04 -yes
sfk setbytes 0.BIN 0x6E995 0x04 -yes
sfk setbytes 0.BIN 0x6E97D 0x04 -yes

# dialog fixes (thanks to paul_met)  https://github.com/eadmaster/pcrown/issues/1
#06074248    04 = 70248
#060742A2    04 = 702A2
#0607401A    04 = 7001A
#06074020    04 = 70020
#060742EE    88 = 702EE
#060742BA    3A = 702BA
sfk setbytes 0.BIN 0x70249 0x04 -yes
sfk setbytes 0.BIN 0x702A3 0x04 -yes
sfk setbytes 0.BIN 0x7001B 0x04 -yes
sfk setbytes 0.BIN 0x70021 0x04 -yes
sfk setbytes 0.BIN 0x702EF 0x88 -yes
sfk setbytes 0.BIN 0x702BB 0x3A -yes

# items description fixes (thanks to paul_met) https://github.com/eadmaster/pcrown/issues/57
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
# "A LONG LONG AGO..." -> "LONG LONG AGO..."
sfk setbytes 0.BIN 0xACC85  "LONG LONG AGO...  "  -yes
# PROSERPINA RUN A WAY AT TOP SPEED -> PROSERPINA RAN AWAY AT FULL SPEED
sfk setbytes 0.BIN 0xACDFD  "RAN AWAY AT FULL"  -yes
# "PARSONS HAPPENED TO BE REAL PORTGUS" -> "PARSON HAPPENED TO BE PORTGUS      "
sfk setbytes 0.BIN 0xACE35  "PARSON HAPPENED TO BE " 0x04 "PORTGUS      "  -yes
# "BUT THIS CAUSED THE GATE TO UNDERWORLD" -> "AND THIS OPENED THE DEMON REALM'S GATE"
sfk setbytes 0.BIN 0xACE91  "AND"  -yes
sfk setbytes 0.BIN 0xACE9A  "OPEN"  -yes
sfk setbytes 0.BIN 0xACEA5  0x04  -yes
sfk setbytes 0.BIN 0xACEA6  "DEMON REALM'S GATE"  -yes
# "GRADRIEL WENT TO FACE THE GREATERDEMONS" -> "GRADRIEL WENT TO FACE THE DEMON KING"
sfk setbytes 0.BIN 0xACED8  "DEMON KING   "  -yes
# "FALLDOWN FROM THE BOOKWORLD" (gameover screen)  ->  "FALLEN FROM THE BOOKWORLD"
sfk setbytes 0.BIN 0xACC3A  "E"  -yes
sfk setbytes 0.BIN 0xACC3C  "N"  -yes
sfk setbytes 0.BIN 0xACC3E  " "  -yes
sfk setbytes 0.BIN 0xACC40  " "  -yes

# fix Engrish in the save manager https://github.com/eadmaster/pcrown/issues/94
# "The SAVE-FILE has crashed." -> "The SaveFile is corrupted"
sfk setbytes 0.BIN 0xA4099  "FILE is corrupt"  -yes
# "BACK-UP RAM is lacking." -> "BACK-UP RAM is full.   "
sfk setbytes 0.BIN 0xA40E0  "full.   "  -yes

# statically enable debug mode (press Start on 2nd pad to navigate event files) https://web.archive.org/web/20200918203538/https://github.com/cyberwarriorx/pcrown/wiki/Debugging
sfk setbytes 0.BIN 0x1EB7F 0x01 -yes

# add splashscreen -> freezes with cheats enabled during boot  https://github.com/eadmaster/pcrown/issues/96
#mv 0.BIN  0_org.BIN
#wine mksplash.exe ../mksplash/splash_loader.bin ../mksplash/splash.bmp 0_org.BIN 0.BIN
#rm 0_org.BIN

# add version number in title screen  https://github.com/eadmaster/pcrown/issues/96
# "@SEGA ENTERPRISES,LTD.& ATLUS,1997" -> "@SEGA & ATLUS,1997 + T-ENG 0.8 +RH"
sfk setbytes 0.BIN 0xA4137  "& ATLUS,1997 T-ENG V0.8.1+EX"  -yes

cd-replace "$PATCHED_IMAGE_FILE" 0.BIN  0.BIN
cd-replace "$PATCHED_IMAGE_FILE" KANJI.BIN  KANJI_ENG.BIN
