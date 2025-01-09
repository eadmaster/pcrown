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

# patch font, items and names -> alters 0.BIN, KANJI.BIN
source _patch_items.sh

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
source _patch_signs.sh

# build xdelta patch
xdelta3 -S none -f -e -s "Princess Crown (Japan) (1M) (Track 01).iso"  "Princess Crown (Japan) (1M) (Track 01) (English).iso"  "Princess.Crown.Japan.1M.Track.01.iso.xdelta"

# build xdelta patch (for original bin)
#wine CDmage.exe "Princess Crown (Japan) (1M) (Track 01) (English).iso"  #  TODO: automate cdmage conversion
#xdelta3 -S none -f -e -s "Princess Crown (Japan) (1M) (Track 01).bin"  "Princess Crown (Japan) (1M) (Track 01) (patched).bin"  "Princess.Crown.Japan.1M.Track.01.bin.xdelta"

