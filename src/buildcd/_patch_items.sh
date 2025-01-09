#!/bin/bash

export CD_PATH=../../cd
export TRANSLATED_SCRIPT_PATH=../../script/eng


# patch the font
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" KANJI.BIN
xdelta3 -f -d -s KANJI.BIN KANJI.BIN.xdelta KANJI_ENG.BIN  # apply English font patch

# patch items and names
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" 0.BIN
wine itemsutil.exe -i ${TRANSLATED_SCRIPT_PATH}/names.txt ${TRANSLATED_SCRIPT_PATH}/items.txt  0.BIN  KANJI_ENG.BIN  0xEA0   # 0xEA0 = starting write offset in KANJI_ENG.BIN, ranges are in itemsutils/main.cpp

# make chars spacing smaller (thanks to paul_met and derek (ateam) for the tips) https://segaxtreme.net/threads/help-me-translate-princess-crown.18555/#post-186226
#060729A8    E204
#06072994    E204
#0607297C    E204
#hexcalc '060729A8 - 06004000 + 1' = 6E9A9
#hexcalc '06072994 - 06004000 + 1' = 6E995
#hexcalc '0607297C - 06004000 + 1' = 6E97D
# TODO: replace ucon64 -> sfk setbytes  http://stahlworks.com/sfk-setbytes
ucon64 --nbak --poke=6E9A9:04 0.BIN
ucon64 --nbak --poke=6E995:04 0.BIN
ucon64 --nbak --poke=6E97D:04 0.BIN

# dialog fixes (thanks to paul_met)  https://github.com/eadmaster/pcrown/issues/1
#06074248    04 = 70248
#060742A2    04 = 702A2
#0607401A    04 = 7001A
#06074020    04 = 70020
#060742EE    88 = 702EE
#060742BA    3A = 702BA
ucon64 --nbak --poke=70249:04 0.BIN
ucon64 --nbak --poke=702A3:04 0.BIN
ucon64 --nbak --poke=7001B:04 0.BIN
ucon64 --nbak --poke=70021:04 0.BIN
ucon64 --nbak --poke=702EF:88 0.BIN
ucon64 --nbak --poke=702BB:3A 0.BIN

# items description fixes (thanks to paul_met) https://github.com/eadmaster/pcrown/issues/57
ucon64 --nbak --poke=70603:14 0.BIN  # X position of the price in the store (max=1A)
# Tile map address (window #1)
ucon64 --nbak --poke=47732:B3 0.BIN
ucon64 --nbak --poke=47733:08 0.BIN
# Tile map address (window #2)
ucon64 --nbak --poke=3BE09:0A 0.BIN
ucon64 --nbak --poke=3BE0A:B3 0.BIN
ucon64 --nbak --poke=3BE0B:08 0.BIN
# Height of window #1
ucon64 --nbak --poke=476FF:05 0.BIN
# Height of window #2
ucon64 --nbak --poke=3BDD5:05 0.BIN
# Y position of window #1
ucon64 --nbak --poke=47707:17 0.BIN
# Y position of window #2
ucon64 --nbak --poke=3BDDD:17 0.BIN
# Y position of text
ucon64 --nbak --poke=75F7E:BC 0.BIN
# Line Scroll Range Fix
ucon64 --nbak --poke=3BA61:B8 0.BIN
# Clearing the description window
ucon64 --nbak --poke=47773:05 0.BIN
ucon64 --nbak --poke=4777B:17 0.BIN

# fix Engrish in the opening https://github.com/eadmaster/pcrown/issues/94
#  using http://stahlworks.com/sfk-setbytes
# "A LONG LONG AGO..." -> "A LONG TIME AGO..."
#sfk setbytes 0.BIN 0xACC8C  "TIME"  -yes
# "A LONG LONG AGO..." -> "LONG LONG AGO..."
sfk setbytes 0.BIN 0xACC85  "LONG LONG AGO...  "  -yes
# PROSERPINA RUN A WAY AT TOP SPEED -> PROSERPINA RUN AWAY AT TOP SPEED
sfk setbytes 0.BIN 0xACE01  " A"  -yes
# "PARSONS HAPPENED TO BE REAL PORTGUS" -> "PARSON  HAPPENED TO BE REAL PORTGUS"
sfk setbytes 0.BIN 0xACE3B  " "  -yes
# "BUT THIS CAUSED THE GATE TO UNDERWORLD" -> "AND THIS OPENED THE DEMON REALM'S GATE"
sfk setbytes 0.BIN 0xACE91  "AND"  -yes
sfk setbytes 0.BIN 0xACE9A  "OPEN"  -yes
sfk setbytes 0.BIN 0xACE9A  "OPEN"  -yes
sfk setbytes 0.BIN 0xACEA5  0x04  -yes
sfk setbytes 0.BIN 0xACEA6  "DEMON REALM'S GATE"  -yes
# "GRADRIEL WENT TO FACE THE GREATERDEMONS" -> "GRADRIEL WENT TO FACE THE DEMON KING"
sfk setbytes 0.BIN 0xACED8  "DEMON KING   "  -yes

# fix Engrish in the save manager https://github.com/eadmaster/pcrown/issues/94
# "The SAVE-FILE has crashed." -> "The SaveFile is corrupted"
sfk setbytes 0.BIN 0xA4099  "FILE is corrupt"  -yes
# "BACK-UP RAM is lacking." -> BACKUP RAM is insuff..
#sfk setbytes 0.BIN 0xA40D5  "FILE is corrupt"  -yes

# enable debug mode (press Start on 2nd pad to navigate event files) https://web.archive.org/web/20200918203538/https://github.com/cyberwarriorx/pcrown/wiki/Debugging
#ucon64 --nbak --poke=1EB7F:01 0.BIN

# add splashscreen -> freezes with cheats enabled on startup
#mv 0.BIN  0_org.BIN
#wine mksplash.exe ../mksplash/splash_loader.bin ../mksplash/splash.bmp 0_org.BIN 0.BIN
#rm 0_org.BIN

cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 0.BIN  0.BIN
cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" KANJI.BIN  KANJI_ENG.BIN
