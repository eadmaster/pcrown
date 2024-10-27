
# cleanup
rm *.eng
rm "Princess Crown (Japan) (1M) (Track 01) (English).iso"
rm "Princess Crown (Japan) (1M) (Track 01) (English).iso.bak"
rm "Princess Crown (Japan) (1M) (Track 01) (English).iso.xdelta"

# convert data bin track to iso
iat "Princess Crown (Japan) (1M) (Track 01).bin" "Princess Crown (Japan) (1M) (Track 01) (English).iso"
cp "Princess Crown (Japan) (1M) (Track 01) (English).iso" "Princess Crown (Japan) (1M) (Track 01) (English).iso.bak"

# update font
7z e -y "Princess Crown (Japan) (1M) (Track 01) (English).iso" KANJI.BIN
#xdelta3 -d -s KANJI.BIN KANJI.BIN.xdelta KANJI_ENG.BIN  # apply font patch
#xdelta3 -e -s KANJI.BIN  KANJI_ENG.BIN  KANJI.BIN.xdelta  # create font patch
cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" KANJI.BIN  KANJI_ENG.BIN

TRANSLATED_SCRIPT_PATH=$HOME/github/pcrown/script/eng

# update items names
7z e -y "Princess Crown (Japan) (1M) (Track 01) (English).iso" 0.BIN
wine itemsutil.exe -i ${TRANSLATED_SCRIPT_PATH}/names.txt ${TRANSLATED_SCRIPT_PATH}/items.txt  0.BIN  KANJI.BIN  0x2400
# MEMO: modifies 0.BIN inplace, also alters KANJI.BIN?

# make chars spacing smaller (thanks to paul_met and derek (ateam) for the tips) https://segaxtreme.net/threads/help-me-translate-princess-crown.18555/#post-186226
#060729A8    E204
#06072994    E204
#0607297C    E204
#hexcalc 0x060729A8-0x06004000+1 = 6E9A9
#hexcalc 0x06072994-0x06004000+1 = 6E995
#hexcalc 0x0607297C-0x06004000+1 = 6E97D
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

cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 0.BIN  0.BIN

# update events
# 7z e -y "Princess Crown (Japan) (1M) (Track 01) (English).iso" *.EVN
find *.EVN | while read eventfile; do 
    EVNBASENAME="$(basename "$eventfile" .EVN )"
    if [ -f ${TRANSLATED_SCRIPT_PATH}/events/${EVNBASENAME}.TXT ]; then
        wine eventeditor.exe -i ${eventfile} ${TRANSLATED_SCRIPT_PATH}/events/${EVNBASENAME}.TXT  -o ${eventfile}.eng
        #xdelta3 -e -s ${eventfile}  ${eventfile}.eng ${eventfile}.xdelta
        cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" ${eventfile}  ${eventfile}.eng
    else
        echo "missing translation script: ${TRANSLATED_SCRIPT_PATH}/events/${EVNBASENAME}.TXT"
    fi
done

# build xdelta patch
xdelta3 -e -s "Princess Crown (Japan) (1M) (Track 01) (English).iso.bak"  "Princess Crown (Japan) (1M) (Track 01) (English).iso"  "Princess Crown (Japan) (1M) (Track 01) (English).iso.xdelta"