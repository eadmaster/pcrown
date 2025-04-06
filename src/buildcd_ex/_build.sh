 #!/bin/bash
 
# Main patching script for the EX build  https://github.com/eadmaster/pcrown/issues/18

# Requirements:
# - pre-patched "../buildcd/Princess Crown (Japan) (1M) (Track 01) (patched).bin"
# - Princess Crown-Hack_(v1.1).7z  downloaded from https://meduza-team.ucoz.net/publ/romkhaking/modifikacii/modifikacija_igr_dlja_sega_saturn/3-1-0-5

export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (patched).bin"
export SIGNS_PATH=../../signs

rm "$PATCHED_IMAGE_FILE"
cp "../buildcd/$PATCHED_IMAGE_FILE" .

# extract prepatched files for the widescreen hack
7z e -y "PC_patch_v1,3.7z" "Patch"

# replace all pre-patched MCB files
find *.MCB | while read mcbfile; do     
    cd-replace  "$PATCHED_IMAGE_FILE"  $mcbfile $mcbfile  > /dev/null
done

# replace other pre-patched files
cd-replace  "$PATCHED_IMAGE_FILE"  STG_DAT2.PRG  STG_DAT2.PRG  > /dev/null
cd-replace  "$PATCHED_IMAGE_FILE"  COCKPIT.CLB  COCKPIT.CLB  > /dev/null

# repatch COCKPIT.CHB - replace BEGIN text in load save dialog  https://github.com/eadmaster/pcrown/issues/90
file2hexstr() { sfk hexdump -raw "$1" ; }
sfk replace COCKPIT.CHB -binary /$(file2hexstr ${SIGNS_PATH}/save_begin_jap.bin)/$(file2hexstr ${SIGNS_PATH}/save_begin_eng.bin)/ -yes -firsthit
cd-replace  "$PATCHED_IMAGE_FILE" COCKPIT.CHB COCKPIT.CHB > /dev/null

# repatch KD2T.CHR
cp "../buildcd/KD2T.CHR" KD2T.CHR.eng
sfk replace KD2T.CHR.eng -binary /11111111111111111111111111111111111111111111111111111111111111112222222222222222222222222222222222222222222222222222222222222222/44444444444444444444444444444444444444444444444444444444444444445555555555555555555555555555555555555555555555555555555555555555/ -yes -firsthit
sfk replace KD2T.CHR.eng -binary /3333333333333333333333333333333333333333333333333333333333333333/6666666666666666666666666666666666666666666666666666666666666666/ -yes -firsthit                                  
cd-replace  "$PATCHED_IMAGE_FILE" KD2T.CHR KD2T.CHR.eng  > /dev/null

# remove transparency on Amberstep villagers turned into stones
sfk setbytes 0.BIN 0xF36E 0x0000 -yes

# repatch 0.BIN, KANJI.BIN
source _patch_items.sh

# repatch some event files to fix the room size
#wine ../buildcd/eventeditor.exe -i 031_01_0.EVN ${TRANSLATED_SCRIPT_PATH}/events_splitted_35chars/031_01_0.TXT  -o 031_01_0.EVN.eng
cp ../buildcd/031_01_0.EVN.eng .
sfk setbytes 031_01_0.EVN.eng 0x1aa 0x2c -yes
cd-replace "$PATCHED_IMAGE_FILE" 031_01_0.EVN  031_01_0.EVN.eng  > /dev/null
# Dwarf King cutscenes fix https://github.com/eadmaster/pcrown/issues/105
cp ../buildcd/055_01_1.EVN.eng .
sfk setbytes 055_01_1.EVN.eng 0xDF 0x2C -yes
cd-replace "$PATCHED_IMAGE_FILE" 055_01_1.EVN  055_01_1.EVN.eng  > /dev/null
cp ../buildcd/055_02_3.EVN.eng .
sfk setbytes 055_02_3.EVN.eng 0x134 0x2C -yes
sfk setbytes 055_02_3.EVN.eng 0x1BF 0x2C -yes
cd-replace "$PATCHED_IMAGE_FILE" 055_02_3.EVN  055_02_3.EVN.eng  > /dev/null


# build xdelta patch (bin)
xdelta3 -S none -f -e -s "../buildcd/Princess Crown (Japan) (1M) (Track 01).bin" "$PATCHED_IMAGE_FILE"  "Princess.Crown.Japan.1M.Track.01.EX.bin.xdelta"
