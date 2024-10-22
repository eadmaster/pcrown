 

iat "Princess Crown (Japan) (1M) (Track 01).bin" "Princess Crown (Japan) (1M) (Track 01).iso"
cp "Princess Crown (Japan) (1M) (Track 01).iso" "Princess Crown (Japan) (1M) (Track 01).iso.bak"

# update font
7z e "Princess Crown (Japan) (1M) (Track 01).iso" KANJI.BIN
xdelta3 -d -s KANJI.BIN KANJI.BIN.xdelta KANJI_ENG.BIN
cd-replace  "Princess Crown (Japan) (1M) (Track 01).iso" KANJI.BIN  KANJI_ENG.BIN


# TODO: update items names
#7z e "Princess Crown (Japan) (1M) (Track 01).iso" 0.BIN
#...

TRANSLATED_SCRIPT_PATH=$HOME/github/pcrown/script/eng/events

find *.EVN | while read eventfile; do 
    EVNBASENAME="$(basename "$eventfile" .EVN )"
    
    wine eventeditor.exe -i $eventfile $TRANSLATED_SCRIPT_PATH/${EVNBASENAME}.txt  -o ${eventfile}.eng
    #xdelta3 -e -s ${eventfile}  ${eventfile}.eng ${eventfile}.xdelta
    cd-replace  "Princess Crown (Japan) (1M) (Track 01).iso" ${eventfile}  ${eventfile}.eng

done

xdelta3 -e -s "Princess Crown (Japan) (1M) (Track 01).iso.bak"  "Princess Crown (Japan) (1M) (Track 01).iso"  "Princess Crown (Japan) (1M) (Track 01).iso.xdelta"