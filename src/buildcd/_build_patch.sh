 
rm *.eng
rm "Princess Crown (Japan) (1M) (Track 01).iso"
rm "Princess Crown (Japan) (1M) (Track 01).iso.bak"

iat "Princess Crown (Japan) (1M) (Track 01).bin" "Princess Crown (Japan) (1M) (Track 01).iso"
cp "Princess Crown (Japan) (1M) (Track 01).iso" "Princess Crown (Japan) (1M) (Track 01).iso.bak"

# update font
#7z e "Princess Crown (Japan) (1M) (Track 01).iso" KANJI.BIN
#xdelta3 -d -s KANJI.BIN KANJI.BIN.xdelta KANJI_ENG.BIN
cd-replace  "Princess Crown (Japan) (1M) (Track 01).iso" KANJI.BIN  KANJI_ENG.BIN

TRANSLATED_SCRIPT_PATH=$HOME/github/pcrown/script/eng

# update items names
7z e "Princess Crown (Japan) (1M) (Track 01).iso" 0.BIN
wine itemsutil.exe -i ${TRANSLATED_SCRIPT_PATH}/names.txt ${TRANSLATED_SCRIPT_PATH}/items.txt  0.BIN  KANJI.BIN  0x2400
# MEMO: modifies 0.BIN inplace, also alters KANJI.BIN?
cd-replace  "Princess Crown (Japan) (1M) (Track 01).iso" 0.BIN  0.BIN

find *.EVN | while read eventfile; do 
    EVNBASENAME="$(basename "$eventfile" .EVN )"
    
    wine eventeditor.exe -i ${eventfile} ${TRANSLATED_SCRIPT_PATH}/events/${EVNBASENAME}.txt  -o ${eventfile}.eng
    #xdelta3 -e -s ${eventfile}  ${eventfile}.eng ${eventfile}.xdelta
    cd-replace  "Princess Crown (Japan) (1M) (Track 01).iso" ${eventfile}  ${eventfile}.eng
done

xdelta3 -e -s "Princess Crown (Japan) (1M) (Track 01).iso.bak"  "Princess Crown (Japan) (1M) (Track 01).iso"  "Princess Crown (Japan) (1M) (Track 01).iso.xdelta"