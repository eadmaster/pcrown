#!/bin/bash

# Doorway signs patching script  https://github.com/eadmaster/pcrown/issues/5
# Made by eadmaster for the Princess Crown Translation GPL Edition project  https://github.com/eadmaster/pcrown/

export SIGNS_PATH=../../signs
#export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (English).iso"
export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (patched).bin"

# turn a file into an hex string
#file2hexstr() { xxd -p -c 10000 "$1" ; }   
file2hexstr() { sfk hexdump -raw "$1" ; }   
file2hexstr_sign() { sfk hexdump -raw "$1" | tr 1 F ; }   # fixes pixel values on signs converted via bmp2bin

# binary file search & replace
# using sfk http://stahlworks.com/sfk-rep
file_patch() { sfk replace $1 -binary /$2/$3/ -yes -firsthit ; }

replace_sign() {
    FILEBASENAME=${SIGNS_PATH}/$1
    CORRECT_FILESIZE=$(stat -c%s "${FILEBASENAME}_jap.bin")
	if [ ! -f "${FILEBASENAME}_jap.bin" ]; then
		echo "$0: sign ${FILEBASENAME}_jap.bin does not exist (skipped)"
		return
	fi
	if [ ! -f "${FILEBASENAME}_eng.bin" ] ||  [ $(stat -c%s "${FILEBASENAME}_eng.bin") -ne $CORRECT_FILESIZE ]; then
		echo "$0: sign ${FILEBASENAME}_eng.bin wrong size or does not exist (skipped)"
		return
	fi
    
    SIGN_JAP_HEX_STR=$(file2hexstr_sign ${FILEBASENAME}_jap.bin)
    SIGN_ENG_HEX_STR=$(file2hexstr_sign ${FILEBASENAME}_eng.bin)
    
    # binary search-replace 
    #sfk replace $chrfile -binary /$SIGN_JAP_HEX_STR/$SIGN_ENG_HEX_STR/  -yes -firsthit   
    #file_patch $chrfile $SIGN_JAP_HEX_STR $SIGN_ENG_HEX_STR
    sfk replace -binary /$SIGN_JAP_HEX_STR/$SIGN_ENG_HEX_STR/  -yes -firsthit -file .CHR -dir .
}

# extract all files with gfx elements
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" *.CHR

# TODO: extract original *_jap.bin signs with sfk http://stahlworks.com/sfk-partcopy
# sfk partcopy infile -fromto startoffset  

# doorway signs translation  https://github.com/eadmaster/pcrown/issues/5
# Pub (24*11/2 = 132 bytes)
replace_sign  pub  132
# NPC (24*11/2)
replace_sign  npc  132
# shop (16*11/2 = 88 bytes)
replace_sign  shop  88
# Potions/Meds (shop) (16*11/2 = 88 bytes) -> test in 002-00, 043-01
replace_sign  med_shop  88
# Item (shop) (24*11/2 = 132 bytes)
replace_sign  item_shop  132
# Inn (shop)(16*11/2 = 88 bytes)
replace_sign  inn  88
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
# Valenadine Castle  (96*11/2 = 528) -> test in 002-00
replace_sign  castle  528
# empty / vacant house (40*11/2 = 220)  -> test in 043-00
replace_sign  empty  220
# floor_sub (24*11/2 = 132) -> test in 100-12
replace_sign floor_sub 132
# port (16*11/2 = 88) -> test in 039-00
replace_sign  port  88
# uptown/innertown (40*11/2) -> test in 061-00
replace_sign  uptown  220
# (underground) dungeon (88*12/2) -> test in 000-06
replace_sign  dungeon  528
# witch's (40*11/2=220) -> test in 150-00 as proserpina
replace_sign  witch  220
# old (40*11/2=220) -> test in 153-00 as gradriel
replace_sign  old  220
# hut (24*11/2=132) -> test in 150-00
replace_sign  hut  132
# mysterious' shop  (40*11/2=220) -> test in 152-00
replace_sign  mysterious  220
# inn_room (24*11/2=132) -> test in 055-00
replace_sign  inn_room  132
# mine (24*11/2=132) -> test in 055-00
replace_sign  mine  132
# king's palace (40*11/2=220) -> test in 055-00
replace_sign  kings  220
# track (48*10/2=240) -> test in 055-00
replace_sign  track  240
# chase (64*11/2=352) -> test in 055-00
replace_sign  chase  352
# stones/gems shop (40*11/2=220) -> test in 055-00
replace_sign  stone_shop  220
# (to the) tower (72*11/2=396) -> test in 036-00
replace_sign  tower  396
# (to the) tower2 (72*11/2=396) -> test in 094-00 as Portgus
replace_sign  tower2  396
# out/exit (16*11/2=88) -> test in 038-01
replace_sign  out  88
# Goblin market (112*11/2=616) -> test in 154-00
replace_sign  goblin_market  616
# Volga castle (64*11/2=352) -> test in 081-00
replace_sign  volga_castle  352
# stone_shop2 (24*11/2=132) -> test in 081-00
replace_sign  stone_shop2  132
# aud_hall2 (48*12/2=288) -> test in 079-03
replace_sign  aud_hall2  288
# jadis' (72*11/2=396) -> test in 079-03
replace_sign  jadis  396
# Pumpkin House (96*11/2=528) -> test in 022-00
replace_sign  pumpkin_house  528
# cave (24*11/2=132) -> test in 047-00
replace_sign  cave  132
# southeast (40*11/2=220) -> test in 047-00
replace_sign  southeast  220
# Heindel's (72*11/2=396) -> test in 148-07
replace_sign  heindel  396
# Goat (hut) (40*11/2=220) -> test in 026-00
replace_sign  goat  220
# Farm/Ranch (24*11/2=132) -> test in 026-00
replace_sign  ranch  132
# Desert (64*11/2=352) -> test in 031-00
replace_sign  desert  352
# Dragon's nest (88*11/2=484) -> test in 017-00
replace_sign  dragonnest  484
# portal (To another space) (48*11/2=264) -> test in 000_07_B
replace_sign  portal  264
# "he" kana (8*5/2=20) -> test in 055-00, cleared
replace_sign  he_kana2  20

# replace all chr files with updated signs
find *.CHR -mtime -1 | while read chrfile; do     
    cd-replace  "$PATCHED_IMAGE_FILE"  $chrfile $chrfile  > /dev/null
done

# replace BEGIN text in load save dialog  https://github.com/eadmaster/pcrown/issues/90
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" COCKPIT.CHB  
file_patch COCKPIT.CHB $(file2hexstr ${SIGNS_PATH}/save_begin_jap.bin) $(file2hexstr ${SIGNS_PATH}/save_begin_eng.bin)
cd-replace  "$PATCHED_IMAGE_FILE" COCKPIT.CHB COCKPIT.CHB

# fix Engrish in names https://github.com/eadmaster/pcrown/issues/93
# PORTGAS->PORTGUS banner
#ALREADY EXTRACTED: 7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" COMM.CHR
file_patch COMM.CHR $(file2hexstr ${SIGNS_PATH}/portgus_jap.bin) $(file2hexstr ${SIGNS_PATH}/portgus_eng.bin)
cd-replace  "$PATCHED_IMAGE_FILE" COMM.CHR COMM.CHR

# fix Engrish in enemy banner names https://github.com/eadmaster/pcrown/issues/93
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" *.PRG
sfk replace DOHDOH.PRG -text '/DOHDOH/ DODO /' -yes
cd-replace "$PATCHED_IMAGE_FILE"  DOHDOH.PRG  DOHDOH.PRG
sfk replace CEYE.PRG -text '/CHAOTHIC EYE/CHAOTIC EYE /'  -yes
cd-replace "$PATCHED_IMAGE_FILE" CEYE.PRG  CEYE.PRG
sfk replace NECRO.PRG -text '/NECRO SAMANSA/NECROSAMANTHA/'  -yes
#ALT. sfk replace NECRO.PRG -text '/NECRO SAMANSA/  SAMANTHA   /'  -yes
cd-replace "$PATCHED_IMAGE_FILE"  NECRO.PRG  NECRO.PRG
sfk replace RYON.PRG -text '/RYON/LEON/'  -yes
cd-replace "$PATCHED_IMAGE_FILE"  RYON.PRG  RYON.PRG
sfk replace SIRENE.PRG -text '/SIRENE/SIREN /'  -yes
cd-replace "$PATCHED_IMAGE_FILE"  SIRENE.PRG  SIRENE.PRG
sfk replace WGOD.PRG -text '/EVIL GOLGODA/EVILGOLGOTHA/'  -yes
#ALT. sfk replace WGOD.PRG -text '/EVIL GOLGODA/  GOLGOTHA  /'  -yes
cd-replace "$PATCHED_IMAGE_FILE"  WGOD.PRG  WGOD.PRG
sfk replace NISE.PRG -text '/CAPTAIN BIGFOOK/CAPTAIN BIGHOOK/'  -yes
cd-replace "$PATCHED_IMAGE_FILE"  NISE.PRG  NISE.PRG

# fix Engrish in town/place names https://github.com/eadmaster/pcrown/issues/87
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" COMM.PAK
# "DWALF LAND" -> "DWARF LAND"
sfk setbytes COMM.PAK 0x6351 0x05 -yes
# "YGGDRASILL" -> "YGGDRASIL "
sfk setbytes COMM.PAK 0x5E7E 0x0000000000000000 -yes
# NO? "CADHO BADHO" -> "CADO  BADO"
#sfk setbytes COMM.PAK 0x6200 0x4103 -yes             # H->O
#sfk setbytes COMM.PAK ... 0x0x0000000000000000 -yes  # O->invisible
#sfk setbytes COMM.PAK 0x623C 0x4103 -yes             # H->O
#sfk setbytes COMM.PAK ... 0x0x0000000000000000 -yes  # O->invisible
# "EARTH ON TRUSE TERA"-> "  ARS ON TULA      " 
sfk setbytes COMM.PAK 0x650E 0x0000000000000000 -yes  # E->invisible
sfk setbytes COMM.PAK 0x6502 0x0000000000000000 -yes  # A->invisible
sfk setbytes COMM.PAK 0x64F4 0x40F5 -yes              # R->A
sfk setbytes COMM.PAK 0x64E8 0x4105 -yes              # T->R
sfk setbytes COMM.PAK 0x64DC 0x4106 -yes              # H->S
#                                                       O unchanged
#                                                       N unchanged
#                                                       T unchanged
sfk setbytes COMM.PAK 0x64AC 0x4108 -yes              # R->U
sfk setbytes COMM.PAK 0x6458 0x4100 -yes              # U->L
sfk setbytes COMM.PAK 0x64A0 0x40F5 -yes              # S->A
sfk setbytes COMM.PAK 0x6496 0x0000000000000000 -yes  # E->invisible
sfk setbytes COMM.PAK 0x6484 0x0000000000000000 -yes  # T->invisible
sfk setbytes COMM.PAK 0x647E 0x0000000000000000 -yes  # E->invisible
sfk setbytes COMM.PAK 0x6472 0x0000000000000000 -yes  # R->invisible
sfk setbytes COMM.PAK 0x6466 0x0000000000000000 -yes  # A->invisible
cd-replace "$PATCHED_IMAGE_FILE" COMM.PAK  COMM.PAK