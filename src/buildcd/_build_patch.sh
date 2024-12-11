#!/bin/bash

# Main patching script
# Made by eadmaster for the Princess Crown Translation GPL Edition project  https://github.com/eadmaster/pcrown/

# Requirements:
# - apt install iat  p7zip-full  xdelta3  wine  python3  xxd
# - cd-replace (manual install) https://github.com/mistydemeo/cd-replace
# - ucon64 (manual install)  https://ucon64.sourceforge.io/
# - skf (manual install)  https://sourceforge.net/projects/swissfileknife/
# - eventeditor.exe -> build via ../eventeditor/_build.sh
# - itemsutil.exe -> build via ../itemsutil/_build.sh

export CD_PATH=../../cd
export TRANSLATED_SCRIPT_PATH=../../script/eng
export SIGNS_PATH=../../signs

# cleanup
rm *.eng
rm "Princess Crown (Japan) (1M) (Track 01) (English).iso"
rm "Princess Crown (Japan) (1M) (Track 01).iso"
rm "Princess Crown (Japan) (1M) (Track 01) (patched).bin"
rm "Princess.Crown.Japan.1M.Track.01.iso.xdelta"
rm "Princess.Crown.Japan.1M.Track.01.bin.xdelta"

# convert data bin track to iso
iat "Princess Crown (Japan) (1M) (Track 01).bin" "Princess Crown (Japan) (1M) (Track 01).iso"
cp "Princess Crown (Japan) (1M) (Track 01).iso" "Princess Crown (Japan) (1M) (Track 01) (English).iso" 

# update the font
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" KANJI.BIN
xdelta3 -f -d -s KANJI.BIN KANJI.BIN.xdelta KANJI_ENG.BIN  # apply English font patch

# update items and names
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

# enable debug mode (press Start on 2nd pad to navigate event files) https://web.archive.org/web/20200918203538/https://github.com/cyberwarriorx/pcrown/wiki/Debugging
#ucon64 --nbak --poke=1EB7F:01 0.BIN

# add splashscreen  -> crash in Mednafen?
#mv 0.BIN  0_org.BIN
#wine mksplash.exe ../mksplash/splash_loader.bin ../mksplash/splash.bmp 0_org.BIN 0.BIN
#rm 0_org.BIN

cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 0.BIN  0.BIN
cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" KANJI.BIN  KANJI_ENG.BIN

# update events
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" *.EVN
# enforce correct line splitting
for txt in  ${TRANSLATED_SCRIPT_PATH}/events/*.txt ; do
    echo "splitting text in $txt"
    python3 _split_long_lines.py "$txt"  ${TRANSLATED_SCRIPT_PATH}/events_splitted_35chars/$(basename $txt)
done

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

# temp. fix for dragon fight softlock (overwrite jap text)  https://github.com/eadmaster/pcrown/issues/30
xdelta3 -f -d -s 015_00_1.EVN 015_00_1.EVN.xdelta 015_00_1.EVN.fixed
cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 015_00_1.EVN 015_00_1.EVN.fixed
# temp. fix for softlock at Larva boss (overwrite jap text)  https://github.com/eadmaster/pcrown/issues/88
xdelta3 -f -d -s 176_00_0.EVN 176_00_0.EVN.xdelta 176_00_0.EVN.fixed
cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 176_00_0.EVN 176_00_0.EVN.fixed
# temp. fix for PEOPLE FULL bug in Cado Bado (add missing cmds)  https://github.com/eadmaster/pcrown/issues/71
xdelta3 -f -d -s 041_00_1.EVN 041_00_1.EVN.xdelta 041_00_1.EVN.fixed
cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 041_00_1.EVN 041_00_1.EVN.fixed
# temp. fix for softlock at dialog with the wizard turned into a frog () https://github.com/eadmaster/pcrown/issues/89
ucon64 --nbak --poke=1002:01 061_00_2.EVN.eng
ucon64 --nbak --poke=1003:90 061_00_2.EVN.eng
ucon64 --nbak --poke=1002:01 061_00_1.EVN.eng  # not sure if really needed
ucon64 --nbak --poke=1003:90 061_00_1.EVN.eng  # not sure if really needed
cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 061_00_2.EVN  061_00_2.EVN.eng
cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 061_00_1.EVN  061_00_1.EVN.eng


# doorway signs translation  https://github.com/eadmaster/pcrown/issues/5
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" *.CHR
file2hexstr() { xxd -p -c 10000  "$1" | tr 1 F ; }
replace_sign() {
    FILEBASENAME=${SIGNS_PATH}/$1
    CORRECT_FILESIZE=$2
	if [ $(stat -c%s "${FILEBASENAME}_jap.bin") -ne $CORRECT_FILESIZE ]; then
		echo "$0: sign ${FILEBASENAME}_jap.bin wrong size or does not exist (skipped)"
		return
	fi
	if [ $(stat -c%s "${FILEBASENAME}_eng.bin") -ne $CORRECT_FILESIZE ]; then
		echo "$0: sign ${FILEBASENAME}_eng.bin wrong size or does not exist (skipped)"
		return
	fi
    
    SIGN_JAP_HEX_STR=$(file2hexstr ${FILEBASENAME}_jap.bin)
    SIGN_ENG_HEX_STR=$(file2hexstr ${FILEBASENAME}_eng.bin)
    #echo $SIGN_JAP_HEX_STR
    #echo $SIGN_ENG_HEX_STR
    #return
    sfk replace $chrfile -binary /$SIGN_JAP_HEX_STR/$SIGN_ENG_HEX_STR/  -yes    
    # TODO? replace with ucon64 -nbak --hreplace="S:R"
}

find *.CHR | while read chrfile; do 
    # PUB signs (24*11/2 = 132 bytes)
    replace_sign  pub  132
    # NPC signs (24*11/2)
    replace_sign  npc  132
    # shop sign (16*11/2 = 88 bytes)
    replace_sign  shop  88
    # Pot/Med (shop) (16*11/2 = 88 bytes) -> test in 002-00, 043-01
    replace_sign  med_shop  88
    # Item (shop) (24*11/2 = 132 bytes)
    replace_sign  item_shop  132
    # Inn (shop)(16*11/2 = 88 bytes)
    replace_sign  inn  88
    # Castle  (96*11/2 = 528) -> test in 002-00
    replace_sign  castle  528
    # Baker (shop) (24*10/2=120) -> test in 004-00
    replace_sign  baker_shop  120
    # Magic shop (24*11/2=132) -> test in 045-00
    replace_sign  magic_shop  132
    # School (24*11/2=132) -> test in 045-00
    replace_sign  school  132
    # Warp/Teleportation Magic (24*11/2 = 132)  -> test in 045-00
    replace_sign warp  132
    # courtyard (inside castle) (24*11/2 = 132)-> test in 000-01
    replace_sign  courtyard  132
    # Room (inside castle) (24*11/2 = 132) -> test in 000-04
    replace_sign  room  132
    # Audience Hall (inside castle) (48*11/2 = 264) -> test in 000-03
    replace_sign  aud_hall  264
    # Floor (16*11/2 = 88) -> "F"
    replace_sign  floor  88
    # 2nd/3rd Floor (24*11/2 = 132) -> "3 F"
    replace_sign  floor3  132
    replace_sign  floor2  132
    # "he" kana (16*5/2) -> cleared
    replace_sign  he_kana  40
    # "spell kanji (16*11/2 = 88) -> cleared
    replace_sign  spell_kanji 88
    # "no" kana (16*8/2=64) -> 's
    replace_sign  no_kana  64
    # Jestonai (72*12/2=432) -> test in 000-04 (sx)
    replace_sign  jetsonai  432
    # Kwein (48*10/2=240) -> test in 000-04 (sx)
    replace_sign  kwein  240
    # Sidrael (64*11/2=352) -> test in 000-04  (rx)
    replace_sign  sidrael  352
    # Eriel (48*10/2=240) -> test in 000-04  (rx)
    replace_sign  eriel  240
    # Gradriel (72*11/2=396) -> test in 000-04 (rx)
    replace_sign  gradriel  396
    # empty / vacant house (40*11/2 = 220)  -> test in 043-00
    replace_sign  empty  220
    # floor_sub (24*11/2 = 132) -> test in 100-12
    replace_sign floor_sub 132
    # port (16*11/2 = 88) -> test in 039-00
    replace_sign  port  88
        
    cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso"  $chrfile $chrfile
done

# replace BEGIN text in load save dialog  https://github.com/eadmaster/pcrown/issues/90
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" COCKPIT.CHB
sfk replace COCKPIT.CHB -binary /$( xxd -p -c 10000 ${SIGNS_PATH}/save_begin_jap.bin)/$( xxd -p -c 10000 ${SIGNS_PATH}/save_begin_eng.bin)/  -yes    
cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" COCKPIT.CHB COCKPIT.CHB

# build xdelta patch
xdelta3 -S none -f -e -s "Princess Crown (Japan) (1M) (Track 01).iso"  "Princess Crown (Japan) (1M) (Track 01) (English).iso"  "Princess.Crown.Japan.1M.Track.01.iso.xdelta"

# build xdelta patch (for original bin)
#wine CDmage.exe "Princess Crown (Japan) (1M) (Track 01) (English).iso"  #  TODO: automate cdmage conversion
#xdelta3 -S none -f -e -s "Princess Crown (Japan) (1M) (Track 01).bin"  "Princess Crown (Japan) (1M) (Track 01) (English).bin"  "Princess.Crown.Japan.1M.Track.01.bin.xdelta"

