#!/bin/bash

# Main patching script
# Made by eadmaster for the Princess Crown Translation GPL Edition project  https://github.com/eadmaster/pcrown/

# Requirements:
# - apt install iat  p7zip-full  xdelta3  wine  python3
# - cd-replace (manual install) https://github.com/mistydemeo/cd-replace
# - ucon64 (manual install)  https://ucon64.sourceforge.io/
# - skf (manual install)  https://sourceforge.net/projects/swissfileknife/
# - eventeditor.exe -> build via ../eventeditor/_build.sh
# - itemsutil.exe -> build via ../itemsutil/_build.sh

# cleanup
rm *.eng
rm "Princess Crown (Japan) (1M) (Track 01) (English).iso"
rm "Princess Crown (Japan) (1M) (Track 01) (English).iso.bak"
rm "Princess Crown (Japan) (1M) (Track 01) (English).iso.xdelta"
rm KANJI_ENG.BIN

# convert data bin track to iso
iat "Princess Crown (Japan) (1M) (Track 01).bin" "Princess Crown (Japan) (1M) (Track 01) (English).iso"
cp "Princess Crown (Japan) (1M) (Track 01) (English).iso" "Princess Crown (Japan) (1M) (Track 01) (English).iso.bak"

# update font
7z e -y "Princess Crown (Japan) (1M) (Track 01) (English).iso" KANJI.BIN
xdelta3 -f -d -s KANJI.BIN KANJI.BIN.xdelta KANJI_ENG.BIN  # apply English font patch
#xdelta3 -e -s KANJI.BIN  KANJI_ENG.BIN  KANJI.BIN.xdelta  # create new font patch

TRANSLATED_SCRIPT_PATH=../../script/eng

# update items and names
7z e -y "Princess Crown (Japan) (1M) (Track 01) (English).iso" 0.BIN
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

# dialog fixes (thanks to paul_met)
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
#hexcalc '06074602 - 06004000 + 1' =70603
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
# cook menu fix
#05E6D56B-    FFEAFFFFEB00FFF3F700FFF7CFFFE9FFE8FF (old value)
#05E6D57C     FFEAFFFFF7FFE8FFFFE9FFF7FFFFFFFFFFFF (new value)
#05E6D57D-    FFEAFFEBFF00FFF7003CFFECC6FFFFE8FFF7FFFFE9 (old value)
#05E6D591     FFEB00FFFFF7FFECC6FFFFEB04FFFFE8FFF7FFFFE9 (new value)
#05E6D592-    FFEAFFFFEB00FFF7CF00FFECC63FFFE9FFE8FF  (old value)
#05E6D5A4     FFEAFFFFF7FFECC6FFFFEB05FFFFE8FFF7FFFFF9 (new value)
sfk replace KANJI_ENG.BIN -binary /FFEAFFFFEB00FFF3F700FFF7CFFFE9FFE8FF/FFEAFFFFF7FFE8FFFFE9FFF7FFFFFFFFFFFF/  -yes
sfk replace KANJI_ENG.BIN -binary /FFEAFFEBFF00FFF7003CFFECC6FFFFE8FFF7FFFFE9/FFEB00FFFFF7FFECC6FFFFEB04FFFFE8FFF7FFFFE9/  -yes
#NO? optional (for selling items - adding descriptions line for items). -> breaks the game
#sfk replace KANJI_ENG.BIN -binary /FFEAFFFFEB00FFF7CF00FFECC63FFFE9FFE8FF/FFEAFFFFF7FFECC6FFFFEB05FFFFE8FFF7FFFFF9/  -yes

cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 0.BIN  0.BIN
cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" KANJI.BIN  KANJI_ENG.BIN

# update events
if [ "$1" == "new" ] || [ "$1" == "update_script" ]; then
    7z e -y "Princess Crown (Japan) (1M) (Track 01) (English).iso" *.EVN
    # enforce correct line splitting
    for txt in  ${TRANSLATED_SCRIPT_PATH}/events/*.txt ; do
        echo "splitting text in $txt"
        python3 _split_long_lines.py "$txt"  ${TRANSLATED_SCRIPT_PATH}/events_splitted_35chars/$(basename $txt)
    done
fi
find *.EVN | while read eventfile; do 
    EVNBASENAME="$(basename "$eventfile" .EVN )"
    if [ -f ${TRANSLATED_SCRIPT_PATH}/events_splitted_35chars/${EVNBASENAME}.TXT ]; then
        echo "updating ${eventfile}"
        wine eventeditor.exe -i ${eventfile} ${TRANSLATED_SCRIPT_PATH}/events_splitted_35chars/${EVNBASENAME}.TXT  -o ${eventfile}.eng
        #xdelta3 -e -s ${eventfile}  ${eventfile}.eng ${eventfile}.xdelta
        cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" ${eventfile}  ${eventfile}.eng  > /dev/null
    else
        echo "missing translation script: ${TRANSLATED_SCRIPT_PATH}/events_splitted_35chars/${EVNBASENAME}.TXT"
    fi
done

# temp. fix for dragon fight softlock (restore jap ver)  https://github.com/eadmaster/pcrown/issues/30
cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 015_00_1.EVN 015_00_1.EVN
# temp. fix for softlock at Larva boss (restore jap ver)  https://github.com/eadmaster/pcrown/issues/88
cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 176_00_0.EVN 176_00_0.EVN

# doorway signs translation  https://github.com/eadmaster/pcrown/issues/5
# TODO: replace with ucon64 --hreplace="S:R"
7z e -y "Princess Crown (Japan) (1M) (Track 01) (English).iso" *.CHR
find *.CHR | while read chrfile; do 
    # PUB signs (24*11/2 = 132 bytes)
    sfk replace $chrfile -binary /0F00FFFFFFF000F00FFFFF0000F000F0F00000F00F000F00000000F0F00000F00FFFFF00F000FFFFFFF0FFFF0F000F000F00F0F0F0F000F0FFFFFFF00000F0F0F0F000F000F000000000FF00FFF000F00FFFFFF00F00F00000F000F0F0F0F0F00F00FFFFFFF0FF000F00F0F0F000F00000F00000F00F00F0F000FFFFFFF0000000F00F00/0FFFFF000F00000F0FFFFF000F0000F00F00000F00F000F00F00000F0F00000F00F0000F0F00000F0F00000F00F0000F0F00000F0F00000F00F000F00F0000F00F00000F00FFFF000FFFFF000F00000F00F000F00F0000000F00000F00F0000F0F0000000F00000F00F0000F0F00000000F000F000F000F00F000000000FFF000FFFFF00/  -yes
    
    # NPC signs (24*11/2)
    sfk replace $chrfile -binary /0FFFFFFFFF0000000F0000000F0000000F00FFFFFFFFFFF00F0000000F00F000000000F00FFFFFFFFF0000FFFFFFF0000F0000F000000000F0F000000F0000F000000FFF0F0F0F000FFFFFFFFFF00000FFF0F0000F00000F00000FFF00F0F0000F00000F00000000FFF00F000F0FF000F0F00FFF00F000F0FFF000000FF00000FF000000/0F00000F0FFFFF00000FFF000FF0000F0F0000F000F000F00F0F000F0F00000F0F00000F0F0F000F0F00000F0F00000F0F00F00F0F00000F0F0000000F00F00F0F0000F00F0000000F000F0F0FFFFF000F0000000F000F0F0F0000000F00000F0F0000FF0F0000000F00000F0F0000FF0F00000000F000F00F00000F0F000000000FFF00/  -yes
    
    # shop sign (16*11/2 = 88 bytes)
    # "$" sign (centered):
    sfk replace $chrfile -binary /0FFFFFFFFF0000000F0000000F0000000FFFFFFFFF0000000F000000000000000FFFFFFFFFF000000F00F000F00000000F0FFFFF0F0000000F0000F0000000000F0FFFFFFF000000F00000F000000000F0FFFFFFFFF00000/0000000F0000000000000FFFFF0000000000F00F00F000000000F00F000000000000F00F0000000000000FFFFF0000000000000F00F000000000000F00F000000000F00F00F0000000000FFFFF0000000000000F00000000/  -yes
 
    # Med (shop) (16*11/2 = 88 bytes)
    sfk replace $chrfile -binary /000F000F00000000FFFFFFFFFFF0000000000F0000000000F00FFFFF00F000000FFF000FFF0000000F0FFFFF0F000000F0000F0000F00000FFFFFFFFFFF000000000FFF00000000000FF0F0FF0000000FF000F000FF00000/F000F0FFFF0FFF00FF0FF0F00F0F00F0F0F0F0F00F0F000FF0F0F0F0000F000FF000F0F0000F000FF000F0FFF00F000FF000F0F0000F000FF000F0F0000F000FF000F0F00F0F000FF000F0F00F0F00F0F000F0FFFF0FFF00/  -yes

    # Item (shop) (24*11/2 = 132 bytes)
    sfk replace $chrfile -binary /0F0000F0F00000FFFFFFF00000F0FFFFFFF000F00000F0000000000F000000FFFFFFF00000000FFFFF0000F00000F000FFF00F000F0000FFFFFFF00000F00FFFFF0000F00000F00000F00F000F0000FFFFFFF00000F00F000F0000000000000000F00FFFFF00FFFFFFFFFFF00F0F0000000000F00000F000F000FFFFFFF00F0000000F00/FFF0FFFFF0FFFFFF0F00000F0F0000F0000F000F0FF000FF0F0000F0000F000F0F0F0F0F0F0000F0000F00000F00F00F0F0000F0000F00000F00F00F0F0000F0000FFF000F00000F0F0000F0000F00000F00000F0F0000F0000F00000F00000F0F0000F0000F000F0F00000F0F0000F0000F000F0F00000FFFF000F000FFFFFF0F00000F/  -yes
    
    # Inn (shop)(16*11/2 = 88 bytes)
    sfk replace $chrfile -binary /00000F0000000000FFFFFFFFFFF00000F000000000F0000000F000000000000000F0FFFFFF0000000F0000F000000000FF00FFFFFF0000000F00F0000F0000000F00FFFFFF0000000F00F0000F0000000F00FFFFFF000000/FFFF0F000F0F000F0F000FF00F0FF00F0F000F0F0F0F0F0F0F000F0F0F0F0F0F0F000F00FF0F00FF0F000F00FF0F00FF0F000F000F0F000F0F000F000F0F000F0F000F000F0F000F0F000F000F0F000FFFFF0F000F0F000F/  -yes
    
    # BAKE (shop) (24*10/2=120)
    sfk replace $chrfile -binary /0000000000F0000000000000000F000F0F0F00F000000000000F000F00F0000FF00000F000F00000F00000000F0000F000F00000F000000000000F0000F000000F00000000000F0000F000000F0000000000F0000F0000000F000000000F00000F00000000F000000FF00000F000000000F000FFF0000000/FFF00000FF000F000F0FFFFFF00F000F00F00F000F0F0000F000F00F00F00F00F00F0000F000F00F00F00F0F000F0000F00F00F0000F0FF0000F0000FFF000F0000F0F00000FFFF0F00F00F0000F0FF0000F0000F000F0FFFFFF0F0F000F0000F000F0F0000F0F00F00F0000FFFF00F0000F0F000F0FFFFF/  -yes
        
    cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso"  $chrfile $chrfile
done


# build xdelta patch
xdelta3 -f -e -s "Princess Crown (Japan) (1M) (Track 01) (English).iso.bak"  "Princess Crown (Japan) (1M) (Track 01) (English).iso"  "Princess.Crown.Japan.1M.Track.01.iso.xdelta"
