#!/bin/bash

# Doorway signs patching script  https://github.com/eadmaster/pcrown/issues/5
# Made by eadmaster for the Princess Crown Translation GPL Edition project  https://github.com/eadmaster/pcrown/

export SIGNS_PATH=../../signs


# turn a file into an hex string
file2hexstr() { xxd -p -c 10000  "$1" | tr 1 F ; }   

replace_sign() {
    FILEBASENAME=${SIGNS_PATH}/$1
    CORRECT_FILESIZE=$2
	if [ ! -f "${FILEBASENAME}_jap.bin" ] || [ $(stat -c%s "${FILEBASENAME}_jap.bin") -ne $CORRECT_FILESIZE ]; then
		echo "$0: sign ${FILEBASENAME}_jap.bin wrong size or does not exist (skipped)"
		return
	fi
	if [ ! -f "${FILEBASENAME}_eng.bin" ] ||  [ $(stat -c%s "${FILEBASENAME}_eng.bin") -ne $CORRECT_FILESIZE ]; then
		echo "$0: sign ${FILEBASENAME}_eng.bin wrong size or does not exist (skipped)"
		return
	fi
    
    SIGN_JAP_HEX_STR=$(file2hexstr ${FILEBASENAME}_jap.bin)
    SIGN_ENG_HEX_STR=$(file2hexstr ${FILEBASENAME}_eng.bin)
    #echo $SIGN_JAP_HEX_STR
    #echo $SIGN_ENG_HEX_STR
    #return
    
    # binary search-replace  http://stahlworks.com/sfk-rep
    sfk replace $chrfile -binary /$SIGN_JAP_HEX_STR/$SIGN_ENG_HEX_STR/  -yes    
    # TODO? replace with ucon64 -nbak --hreplace="S:R"
}

# extract all files with signs gfx
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" *.CHR

# interate over the extracted files and replace all signs found
find *.CHR | while read chrfile; do 
    # PUB signs (24*11/2 = 132 bytes)
    replace_sign  pub  132
    # NPC signs (24*11/2)
    replace_sign  npc  132
    # shop sign (16*11/2 = 88 bytes)
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
    # witch's (40*11/2=220) -> test in 150-00
    replace_sign  witch  220
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
    # stones shop (40*11/2=220) -> test in 055-00
    replace_sign  stone_shop  220
    # "he" kana (8*5/2=20) -> test in 055-00, cleared
    replace_sign  he_kana2  20
    

    cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso"  $chrfile $chrfile
done
