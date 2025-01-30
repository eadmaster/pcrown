 #!/bin/bash
 
 
# Requirements:
# - pre-patched "../buildcd/Princess Crown (Japan) (1M) (Track 01) (patched).bin"
# - Princess Crown-Hack_(v1.1).7z  downloaded from https://meduza-team.ucoz.net/publ/romkhaking/modifikacii/modifikacija_igr_dlja_sega_saturn/3-1-0-5

export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (patched).bin"

rm "$PATCHED_IMAGE_FILE"
cp "../buildcd/$PATCHED_IMAGE_FILE" .

# extract prepatched files for the widescreen hack
7z e -y "Princess Crown-Hack_(v1.1).7z" "Patch"

# replace all pre-patched MCB files
find *.MCB | while read mcbfile; do     
    cd-replace  "$PATCHED_IMAGE_FILE"  $mcbfile $mcbfile  > /dev/null
done

# replace other pre-patched files
cd-replace  "$PATCHED_IMAGE_FILE"  STG_DAT2.PRG  STG_DAT2.PRG  > /dev/null
cd-replace  "$PATCHED_IMAGE_FILE"  COCKPIT.CLB  COCKPIT.CLB  > /dev/null

# replace BEGIN text in load save dialog  https://github.com/eadmaster/pcrown/issues/90
# TODO: repatch
#file_patch COCKPIT.CHB $(file2hexstr ${SIGNS_PATH}/save_begin_jap.bin) $(file2hexstr ${SIGNS_PATH}/save_begin_eng.bin)
xdelta3 -f -d -s COCKPIT.CHB COCKPIT.CHB.xdelta COCKPIT.CHB.eng
cd-replace  "$PATCHED_IMAGE_FILE" COCKPIT.CHB COCKPIT.CHB.eng > /dev/null

# TODO: repatch the signs
# source _patch_signs.sh
xdelta3 -f -d -s KD2T.CHR KD2T.CHR.xdelta KD2T.CHR.eng
cd-replace  "$PATCHED_IMAGE_FILE" KD2T.CHR KD2T.CHR.eng  > /dev/null

# repatch 0.BIN, KANJI.BIN
source _patch_items.sh

# repatch a single event  (not sure why is needed)
#wine ../buildcd/eventeditor.exe -i 031_01_0.EVN ${TRANSLATED_SCRIPT_PATH}/events_splitted_35chars/031_01_0.TXT  -o 031_01_0.EVN.eng
cp ../buildcd/031_01_0.EVN.eng .
sfk setbytes 031_01_0.EVN.eng 0x1aa 0x2c -yes
cd-replace "$PATCHED_IMAGE_FILE" 031_01_0.EVN  031_01_0.EVN.eng  > /dev/null

# build xdelta patch (bin)
xdelta3 -S none -f -e -s "../buildcd/Princess Crown (Japan) (1M) (Track 01).bin" "$PATCHED_IMAGE_FILE"  "Princess.Crown.Japan.1M.Track.01.EX.bin.xdelta"
