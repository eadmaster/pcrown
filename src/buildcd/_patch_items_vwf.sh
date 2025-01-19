#!/bin/bash

export CD_PATH=../../cd
export TRANSLATED_SCRIPT_PATH=../../script/eng
#export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (English).iso"
export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (patched).bin"

7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" KANJI.BIN
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" 0.BIN

# unnecessary:
# fontgen -2 -b ${BMFONTGEN} ${FONTGEN_DIR}/pcrown.txt -o KANJI.BIN
# fontgen [options] [filename].txt -o [filename].bin
#wine prgutil.exe [options] [list of prg or text files]

# inject VWF code  https://github.com/eadmaster/pcrown/issues/99
# satcode_insert.exe compiled_code.bin(patch file)  target_file.bin
wine satcode_insert.exe ../satcode.old/satcode_patch.bin 0.BIN

# patch the font
xdelta3 -f -d -s KANJI.BIN KANJI.BIN.xdelta KANJI_ENG.BIN  # apply English font patch

# patch items and names
iconv -f UTF-8 -t SHIFT-JIS ${TRANSLATED_SCRIPT_PATH}/items.txt -o ${TRANSLATED_SCRIPT_PATH}/items.txt.sjis
wine itemsutil.exe -i ${TRANSLATED_SCRIPT_PATH}/names.txt ${TRANSLATED_SCRIPT_PATH}/items.txt.sjis  0.BIN  KANJI_ENG.BIN  0xEA0   # 0xEA0 = starting write offset in KANJI_ENG.BIN, ranges are in itemsutils/main.cpp

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
#sfk setbytes 0.BIN 0x1EB7F 0x01 -yes

# add splashscreen -> freezes with cheats enabled during boot  https://github.com/eadmaster/pcrown/issues/96
#mv 0.BIN  0_org.BIN
#wine mksplash.exe ../mksplash/splash_loader.bin ../mksplash/splash.bmp 0_org.BIN 0.BIN
#rm 0_org.BIN

# add version number in title screen  https://github.com/eadmaster/pcrown/issues/96
# "@SEGA ENTERPRISES,LTD.& ATLUS,1997" ->  "@SEGA & ATLUS,1997    T-ENG V0.8.1"
sfk setbytes 0.BIN 0xA4137  "& ATLUS,1997    T-ENG V0.8.2"  -yes

cd-replace "$PATCHED_IMAGE_FILE" 0.BIN  0.BIN
cd-replace "$PATCHED_IMAGE_FILE" KANJI.BIN  KANJI_ENG.BIN
