#!/bin/bash

# Signs and banners patching script  https://github.com/eadmaster/pcrown/issues/5

export SIGNS_PATH=../../signs
#export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (English).iso"
export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (patched).bin"
export SFK_COLORS=

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
    #echo " ---> $1"
    sfk replace -binary /$SIGN_JAP_HEX_STR/$SIGN_ENG_HEX_STR/  -yes -firsthit -file .CHR -dir .
}

# extract all files with gfx elements
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" '*.CHR'  > /dev/null
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" '*.PAK'  > /dev/null

# TODO: extract original *_jap.bin signs with sfk http://stahlworks.com/sfk-partcopy
# sfk partcopy infile -fromto startoffset  

# doorway signs translation  https://github.com/eadmaster/pcrown/issues/5
# Pub (24*11/2 = 132 bytes)
replace_sign  pub  132
# Home (24*11/2)
replace_sign  home  132
# shop (16*11/2 = 88 bytes)
replace_sign  shop  88
# Potions/Meds (shop) (16*11/2 = 88 bytes) -> test in 002-00, 043-01
replace_sign  potions_shop  88
# Item (shop) (24*11/2 = 132 bytes)
replace_sign  item_shop  132
# Item (shop) (40*11/2=220) -> test in 035-05
replace_sign  item_shop2  220
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
# floor_top (24*11/2=132) -> test in 038-15
replace_sign floor_top 132
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
# truck/track (48*10/2=240) -> test in 055-00
replace_sign  track  240
# chase (64*11/2=352) -> test in 055-00
replace_sign  chase  352
# stones/gems shop (40*11/2=220) -> test in 055-00
replace_sign  gem_shop  220
# (to the) tower (72*11/2=396) -> test in 036-00
replace_sign  tower  396
# (to the) tower2 (72*11/2=396) -> test in 094-00 as Portgus
replace_sign  tower2  396
# out/exit (16*11/2=88) -> test in 038-01
replace_sign  out  88
# Goblin market (112*11/2=616) -> test in 154-00
replace_sign  goblin_market  616
# Goblin market2 (112*11/2=616) -> test in 010-00
replace_sign  goblin_market2  616
# Volga castle (64*11/2=352) -> test in 081-00
replace_sign  volga_castle  352
# gem_shop2 (24*11/2=132) -> test in 081-00
replace_sign  gem_shop2  132
# aud_hall2 (48*12/2=288) -> test in 079-03
replace_sign  aud_hall2  288
# jadis' (72*11/2=396) -> test in 079-03
replace_sign  jadis  396
# Pumpkin House (96*11/2=528) -> test in 022-00
replace_sign  pumpkin_house  528
# cave (24*11/2=132) -> test in 047-00
replace_sign  cave  132
# southwest (40*11/2=220) -> test in 033-00
replace_sign  southwest  220
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
# Portal (To another space) (48*11/2=264) -> test in 000_07_B
replace_sign  portal  264
# Elfaran (88*10/2=440) -> test in 000-04 (old castle, save-dependent)
replace_sign  elfaran  440
# (Guest) Room (24*11/2=132) -> test in 000-04 (old castle, save-dependent)
replace_sign  room_guest  132
# "he" kana (8*5/2=20) -> test in 055-00, cleared
replace_sign  he_kana2  20
# artist (24*11/2=132) -> test in 039-03 (save-dependent)
replace_sign  artist  132
# 's house (24*11/2=132) -> test in 039-03 (save-dependent)
replace_sign  shouse  132
# Grass Meadow (72*11/2=396) -> test in 058-00 (save-dependent)
replace_sign  magic_meadow  396
# Last Tavern/Inn (40*11/2=220) -> test in 083-00
replace_sign  last  220
# Old (Cave) (24*11/2=132) -> test in 087-00 (Edward book)
replace_sign  old2  132
# (Old) Cave (24*11/2=132) -> test in 087-00 (Edward book)
replace_sign  cave2  132
# Lost Forest (48*11/2=264) -> test in 166-00 (Proserpina book)
replace_sign  lost_forest  264

# replace all the chr files with updated signs
find *.CHR -mtime -1 | while read chrfile; do     
    cd-replace  "$PATCHED_IMAGE_FILE"  $chrfile $chrfile  > /dev/null
done

# hide "$" part in the "Inn" signs only
sfk replace -binary /0205110511050205000A/FFFFFFFFFFFFFFFFFFFF/  -yes -dir . -file WN1C.PAK
cd-replace  "$PATCHED_IMAGE_FILE"  WN1C.PAK  WN1C.PAK  > /dev/null
#cd-replace  "$PATCHED_IMAGE_FILE"  EE1C.PAK  EE1C.PAK  > /dev/null
#cd-replace  "$PATCHED_IMAGE_FILE"  KG1C.PAK  KG1C.PAK  > /dev/null
sfk replace -binary /0305120512050305000A/FFFFFFFFFFFFFFFFFFFF/  -yes -dir . -file DC1T.PAK GB1T.PAK KD2T.PAK KD3T.PAK LL1T.PAK NB1T.PAK VG1T.PAK
#cd-replace  "$PATCHED_IMAGE_FILE"  DC1T.PAK  DC1T.PAK  > /dev/null
cd-replace  "$PATCHED_IMAGE_FILE"  GB1T.PAK  GB1T.PAK  > /dev/null
cd-replace  "$PATCHED_IMAGE_FILE"  KD2T.PAK  KD2T.PAK  > /dev/null
cd-replace  "$PATCHED_IMAGE_FILE"  KD3T.PAK  KD3T.PAK  > /dev/null
cd-replace  "$PATCHED_IMAGE_FILE"  LL1T.PAK  LL1T.PAK  > /dev/null
cd-replace  "$PATCHED_IMAGE_FILE"  NB1T.PAK  NB1T.PAK  > /dev/null
cd-replace  "$PATCHED_IMAGE_FILE"  VG1T.PAK  VG1T.PAK  > /dev/null

# hide "$" part in the "Baker" signs only
#sfk replace -binary /0705160516050705000A/00000000000000000000/  -yes -dir . -file DC1T.PAK LL1T.PAK  # NB1T.PAK VG1T.PAK -> multiple matches
#cd-replace  "$PATCHED_IMAGE_FILE"  DC1T.PAK  DC1T.PAK  > /dev/null
sfk setbytes LL1T.PAK  0x172  0xFFFFFFFFFFFFFFFFFFFF -yes  # test in 026-00
cd-replace  "$PATCHED_IMAGE_FILE"  LL1T.PAK  LL1T.PAK  > /dev/null
sfk setbytes NB1T.PAK  0x182  0xFFFFFFFFFFFFFFFFFFFF -yes  # test in 004_00
cd-replace  "$PATCHED_IMAGE_FILE"  NB1T.PAK  NB1T.PAK  > /dev/null
sfk setbytes VG1T.PAK  0x1AA  0xFFFFFFFFFFFFFFFFFFFF -yes  # test in 081-00
cd-replace  "$PATCHED_IMAGE_FILE"  VG1T.PAK  VG1T.PAK  > /dev/null
# TODO: resize the frames

# castle signs repositioning (test in 000-04)
#WIP: python _edit_texture.py VD14.PAK 13C -2 -1
sfk setbytes VD14.PAK  0x13E 0xFFFFFFFFFFFFFFFF -yes  # hide "'s" in Gradriel's room
sfk setbytes VD14.PAK  0x1CE 0xFFFFFFFFFFFFFFFF -yes  # hide "'s" in Eriel's room
sfk setbytes VD14.PAK  0x186 0xFFFFFFFFFFFFFFFF -yes  # hide "'s" in Sidrael's room
sfk setbytes VD14.PAK  0x25E 0xFFFFFFFFFFFFFFFF -yes  # hide "'s" in Kwein's room
# TODO: resize the frames too:
#sfk setbytes EE1C.PAK  0x252  0x14062b062b041404 -yes  # Kwein's room - room
#sfk setbytes EE1C.PAK  0x252  0x200d2e0d2e0c200c -yes  # Kwein's room - center frame
#sfk setbytes EE1C.PAK  0x25E  0x280d210d210c280c -yes  # Kwein's room - left frame
sfk setbytes VD14.PAK  0x216 0xFFFFFFFFFFFFFFFF -yes  # move "'s" in Jestonai's room
cd-replace  "$PATCHED_IMAGE_FILE" VD14.PAK VD14.PAK

# cleanup "Top Room" sign (test in 038-20) https://github.com/eadmaster/pcrown/issues/5#issuecomment-2629013480
sfk setbytes EE1C.PAK  0x5e8  0x00141b05040504051b05005F  -yes  # reposition Top 10R
sfk setbytes EE1C.PAK  0x5f4  0x00100005170517050005000A  -yes  # reposition Room 12L
sfk setbytes EE1C.PAK  0x5c4  0x40221b0C220C220C1b0C000A  -yes  # reposition frame right 11L
sfk setbytes EE1C.PAK  0x5d0  0x40211B0C1B0C1B0C1B0C004B  -yes  # reposition frame center -11L & -11R
sfk setbytes EE1C.PAK  0x5DC  0x4020230C1C0C1C0C230C005F  -yes  # reposition frame left 11R
sfk setbytes EE1C.PAK  0x602  0xFFFFFFFFFFFFFFFF000A -yes       # hide "F" part
sfk setbytes EE1C.PAK  0x60E  0xFFFFFFFFFFFFFFFF000A -yes       # hide "'s" part
cd-replace  "$PATCHED_IMAGE_FILE"  EE1C.PAK  EE1C.PAK  > /dev/null

# replace BEGIN text in load save dialog  https://github.com/eadmaster/pcrown/issues/90
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" COCKPIT.CHB  
file_patch COCKPIT.CHB $(file2hexstr ${SIGNS_PATH}/save_begin_jap.bin) $(file2hexstr ${SIGNS_PATH}/save_begin_eng.bin)
cd-replace  "$PATCHED_IMAGE_FILE" COCKPIT.CHB COCKPIT.CHB

# fix Engrish in names https://github.com/eadmaster/pcrown/issues/93
# PORTGAS->PORTGUS banner
#ALREADY EXTRACTED: 7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" COMM.CHR
file_patch COMM.CHR $(file2hexstr ${SIGNS_PATH}/portgus_jap.bin) $(file2hexstr ${SIGNS_PATH}/portgus_eng.bin)
cd-replace  "$PATCHED_IMAGE_FILE" COMM.CHR COMM.CHR

# fix Engrish in ending scrolls
# "SCENARIO WRITING & VILLAGERS SET" (4bpp 232x16 1856B)
file_patch ROLL.CHR $(file2hexstr ${SIGNS_PATH}/credits_1_jap.bin) $(file2hexstr ${SIGNS_PATH}/credits_1_eng.bin)
# sfk setbytes ROLL.CHR  0x1140  0x$(file2hexstr ${SIGNS_PATH}/credits_1_eng.bin) -yes 
# "SCENARIO WRITING & EVENT DESIGN" (4bpp 224x16 1792B)
file_patch ROLL.CHR $(file2hexstr ${SIGNS_PATH}/credits_2_jap.bin) $(file2hexstr ${SIGNS_PATH}/credits_2_eng.bin)
# sfk setbytes ROLL.CHR  0x1880 0x$(file2hexstr ${SIGNS_PATH}/credits_2_eng.bin) -yes 
cd-replace  "$PATCHED_IMAGE_FILE" ROLL.CHR ROLL.CHR

# fix Engrish in enemy banner names https://github.com/eadmaster/pcrown/issues/93
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" '*.PRG'  > /dev/null
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
sfk replace POLT.PRG -text '/SLAVE WOOD/SLAVEWOOD /'  -yes
cd-replace "$PATCHED_IMAGE_FILE"  POLT.PRG  POLT.PRG
sfk replace PUPPET.PRG -text '/SLAVE WOOD/SLAVEWOOD /'  -yes
cd-replace "$PATCHED_IMAGE_FILE"  PUPPET.PRG  PUPPET.PRG
sfk replace VORG.PRG -text '/VORGLODO/VOLGROD /'  -yes
cd-replace "$PATCHED_IMAGE_FILE"  VORG.PRG  VORG.PRG
sfk replace KUMO.PRG -text '/TARANTURA/TARANTULA/'  -yes
cd-replace "$PATCHED_IMAGE_FILE"  KUMO.PRG  KUMO.PRG

# fix Engrish in town/place names https://github.com/eadmaster/pcrown/issues/87
#7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" COMM.PAK  > /dev/null
# "DWALF LAND" -> "DWARF LAND"
sfk setbytes COMM.PAK 0x6351 0x05 -yes
# "YGGDRASILL" -> "YGGDRASIL " & moved 2px right to recenter
sfk setbytes COMM.PAK 0x5E64 0x\
410016081D081D0F160F0000\
40FD11081808180F110F0000\
410000000000000000000000\
41060A081108110F0A0F0000\
40F504080B080B0F040F0000\
410503080408040F030F0041\
40FB17081008100F170F0055\
40F809080208020F090F0055\
40FB10080908090F100F0055\
410B1E081708170F1E0F0055  -yes
# NO? "CADHO BADHO" -> "CADO  BADO"
#sfk setbytes COMM.PAK 0x6200 0x4103 -yes             # H->O
#sfk setbytes COMM.PAK ... 0x0x0000000000000000 -yes  # O->invisible
#sfk setbytes COMM.PAK 0x623C 0x4103 -yes             # H->O
#sfk setbytes COMM.PAK ... 0x0x0000000000000000 -yes  # O->invisible
# "EARTH ON TRUSE TERA"-> "ARSONTULA" 
sfk setbytes COMM.PAK 0x650E 0x0000000000000000 -yes          # E->invisible
sfk setbytes COMM.PAK 0x6502 0x0000000000000000 -yes          # A->invisible
sfk setbytes COMM.PAK 0x64F4 0x40f51f081808180f1f0f0055 -yes  # R->A, moved right 11
sfk setbytes COMM.PAK 0x64E8 0x410518081108110f180f0055 -yes  # T->R, moved right 11
sfk setbytes COMM.PAK 0x64DC 0x410611080a080a0f110f0055 -yes  # H->S, moved right 12
sfk setbytes COMM.PAK 0x64D0 0x41030a080308030f0a0f0055 -yes  # O unchanged, moved right 9
sfk setbytes COMM.PAK 0x64C4 0x410104080308030f040f0041 -yes  # N unchanged, moved right 9
sfk setbytes COMM.PAK 0x64B8 0x410703080a080a0f030f0000 -yes  # T unchanged, moved right 7
sfk setbytes COMM.PAK 0x64AC 0x410809081008100f090f0000 -yes  # R->U, moved right 7
sfk setbytes COMM.PAK 0x6458 0x410010081708170f100f0000 -yes  # U->L, moved right 7
sfk setbytes COMM.PAK 0x64A0 0x40f517081e081e0f170f0000 -yes  # S->A, moved right 7
sfk setbytes COMM.PAK 0x6496 0x0000000000000000 -yes          # E->invisible
sfk setbytes COMM.PAK 0x648A 0x0000000000000000 -yes          # T->invisible
sfk setbytes COMM.PAK 0x647E 0x0000000000000000 -yes          # E->invisible
sfk setbytes COMM.PAK 0x6472 0x0000000000000000 -yes          # R->invisible
sfk setbytes COMM.PAK 0x6466 0x0000000000000000 -yes          # A->invisible

# fix Engrish in Notice Drop enemies banners  https://github.com/eadmaster/pcrown/issues/93#issuecomment-2614060584
# "DOH DOH" -> "DODO" (found in 047-00)
# D unchanged
# H->invisible
# O->invisible
# D->O, moved left 9
# H->D
# O unchanged
sfk setbytes COMM.PAK 0x70AC 0x\
40F8020809080901020100AA\
40FC000000000000000000AA\
4103000000000000000000AA\
410314081b081b01140100AA\
40F80E08150815010E0100AA\
410308080F080F01080100AA  -yes
# "ORCHRE JELLY" -> " OCHRE JELLY" (found in 047-00)
sfk setbytes COMM.PAK 0x710E 0x0000000000000000 -yes # O->invisible
sfk setbytes COMM.PAK 0x7100 0x4103 -yes             # R->O
# "SIRENE" -> "SIREN " (found in 047-00)
sfk setbytes COMM.PAK 0x78A6 0x0000000000000000 -yes # E->invisible
# "CHAOTHIC EYE" -> "CHAOTIC EYE" (found in 025-00)
# H unchanged
# C unchanged
# E unchanged, moved left 7
# Y unchanged, moved left 7
# E unchanged, moved left 7
# C unchanged, moved left 7
# H->I, moved right 1
# T unchanged
# O unchanged
# I->invisible
# A unchanged
sfk setbytes COMM.PAK 0x7bc8 0x\
40FC06080D080D01060100AA\
40F7000807080701000100AA\
40F93c08430843013c0100AA\
410B35083c083c01350100AA\
40F92e08350835012e0100AA\
40F726082d082d01260100AA\
40FD210828082801210100AA\
41071a08210821011a0100AA\
410314081B081B01140100AA\
40FD000000000000000000AA\
40F51E08250825011E0100AA -yes


cd-replace "$PATCHED_IMAGE_FILE" COMM.PAK  COMM.PAK  > /dev/null
