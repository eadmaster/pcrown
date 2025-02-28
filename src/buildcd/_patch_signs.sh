#!/bin/bash

# Signs and banners patching script  https://github.com/eadmaster/pcrown/issues/5

export SIGNS_PATH=../../signs
#export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (English).iso"
export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (patched).bin"
export SFK_COLORS=


# turn a file into an hex string
#file2hexstr() { xxd -p -c 10000 "$1" ; } 
file2hexstr() { sfk hexdump -raw "$1" ; }

# binary file search & replace
# using sfk http://stahlworks.com/sfk-rep
file_patch() { sfk replace $1 -binary /$2/$3/ -yes -firsthit ; }

replace_sign() {
	FILEBASENAME=${SIGNS_PATH}/$1
	
	if [ -z "$2" ]; then
		TARGET=".CHR"
	else
		TARGET="$2 $3 $4 $5 $6 $7 $8 $9"
	fi
	
	echo "$0: patching sign $1 in $TARGET..."

	if [ ! -f "${FILEBASENAME}_jap.bin" ] || [ ! -f "${FILEBASENAME}_eng.bin" ]; then
		echo "$0: sign ${FILEBASENAME} missing (skipped)"
		return
	fi

	SIGN_JAP_HEX_STR=$(file2hexstr ${FILEBASENAME}_jap.bin)
	SIGN_ENG_HEX_STR=$(file2hexstr ${FILEBASENAME}_eng.bin)
	
	JAP_SIGN_FILESIZE=$(stat -c%s "${FILEBASENAME}_jap.bin")
	ENG_SIGN_FILESIZE=$(stat -c%s "${FILEBASENAME}_eng.bin")
	
	# check if resizing
	if [ $JAP_SIGN_FILESIZE -ne $ENG_SIGN_FILESIZE ]; then
		echo "$0: sign ${FILEBASENAME}_eng.bin resized"
		# compute and add padding
		JAP_SIGN_PADDED_SIZE=$(( ($JAP_SIGN_FILESIZE + 31) / 32 * 32 ))  # closest bigger multiple of 32
		ENG_SIGN_PADDED_SIZE=$(( ($ENG_SIGN_FILESIZE + 31) / 32 * 32 ))  # closest bigger multiple of 32
		ENG_SIGN_PAD_NEEDED_SIZE=$(( $ENG_SIGN_PADDED_SIZE - ( $JAP_SIGN_PADDED_SIZE - $JAP_SIGN_FILESIZE + $ENG_SIGN_FILESIZE ) ))
		echo "ENG_SIGN_PAD_NEEDED_SIZE: $ENG_SIGN_PAD_NEEDED_SIZE"
		#echo "ENG_SIGN_PADDED_SIZE: $ENG_SIGN_PADDED_SIZE"
		#echo "JAP_SIGN_PADDED_SIZE: $JAP_SIGN_PADDED_SIZE"
		#echo "JAP_SIGN_FILESIZE: $JAP_SIGN_FILESIZE"
		#echo "ENG_SIGN_FILESIZE: $ENG_SIGN_FILESIZE"
		if [ $ENG_SIGN_PAD_NEEDED_SIZE -gt 0 ]; then
			# add extra padding bytes
			SIGN_ENG_HEX_STR=$(file2hexstr ${FILEBASENAME}_eng.bin ; printf '00%.0s' $(seq 1 $ENG_SIGN_PAD_NEEDED_SIZE))
		elif [ $ENG_SIGN_PAD_NEEDED_SIZE -lt 0 ] ; then
			# alt patching method using http://stahlworks.com/sfk-xrep
			SIGN_JAP_HEX_STR=$(file2hexstr ${FILEBASENAME}_jap.bin | sed 's/\(..\)/\\x\1/g')
			SIGN_ENG_HEX_STR=$(file2hexstr ${FILEBASENAME}_eng.bin | sed 's/\(..\)/\\x\1/g')
			ENG_SIGN_PAD_NEEDED_SIZE=${ENG_SIGN_PAD_NEEDED_SIZE#-}  # absolute value 
			#echo "sfk xreplace -text /$SIGN_JAP_HEX_STR[$ENG_SIGN_PAD_NEEDED_SIZE bytes]/$SIGN_ENG_HEX_STR/  -yes -firsthit -file .CHR -dir ."
			sfk xreplace -pat "/$SIGN_JAP_HEX_STR[$ENG_SIGN_PAD_NEEDED_SIZE bytes]/$SIGN_ENG_HEX_STR/"  -yes -firsthit -dir . -file $TARGET
			return
		fi
	fi
	
	# binary search-replace 
	sfk replace -binary /$SIGN_JAP_HEX_STR/$SIGN_ENG_HEX_STR/  -yes -firsthit -dir . -file $TARGET
}


# extract all files with gfx elements
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" '*.CHR'  > /dev/null
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" '*.PAK'  > /dev/null

# TODO: extract original *_jap.bin signs with sfk http://stahlworks.com/sfk-partcopy
# sfk partcopy infile -fromto startoffset  


## doorway signs translation  https://github.com/eadmaster/pcrown/issues/5

# Pub (24*11/2 = 132 bytes)
replace_sign  pub
# Home (24*11/2)
replace_sign  home
# shop (16*11/2 = 88 bytes)
replace_sign  shop
# Potions/Meds (shop) (16*11/2 = 88 bytes) -> test in 002-00, 043-01
replace_sign  potions_shop  VD1T.CHR  YA1C.CHR
# Item (shop) (24*11/2 = 132 bytes)
replace_sign  item_shop
# Item (shop) (40*11/2=220) -> test in 035-05
replace_sign  item_shop2
# Inn (shop)(16*11/2 = 88 bytes)
replace_sign  inn
# Bakery (24*10/2=120) -> test in 004-00
replace_sign  baker_shop
# Magic shop (24*11/2=132) -> test in 045-00
#replace_sign  magic_shop  KD3T.CHR
# School (24*11/2=132) -> test in 045-00
replace_sign  school
# Warp Magic (24*11/2 = 132)  -> test in 045-00
replace_sign  warp  KD3T.CHR  NE1T.CHR  VD1T.CHR  VG1T.CHR
# Courtyard (inside castle) (24*11/2 = 132) -> test in 000-01
replace_sign  courtyard  VD11.CHR
# Room (inside castle) (24*11/2 = 132) -> test in 000-04
replace_sign  room
# Audience Hall (inside castle) (48*11/2 = 264) -> test in 000-03
replace_sign  aud_hall
# Floor (16*11/2 = 88) -> "F"
replace_sign  floor
# 2nd/3rd Floor (24*11/2 = 132) -> "3 F"
replace_sign  floor3
replace_sign  floor2
# "he" kana (16*5/2) -> cleared
replace_sign  he_kana
# "spell" kanji (16*11/2 = 88) -> cleared
replace_sign  spell_kanji
# "no" kana (16*8/2=64) -> 's
replace_sign  no_kana
# Jestonai (72*12/2=432) -> test in 000-04 (sx)
replace_sign  jetsonai
# Kwein (48*10/2=240) -> test in 000-04 (sx)
replace_sign  kwein
# Sidrael (64*11/2=352) -> test in 000-04  (rx)
replace_sign  sidrael
# Eriel (48*10/2=240) -> test in 000-04  (rx)
replace_sign  eriel
# Gradriel (72*11/2=396) -> test in 000-04 (rx)
replace_sign  gradriel
# Valenadine Castle  (96*11/2 = 528) -> test in 002-00
replace_sign  castle
# empty/vacant (home) (40*11/2 = 220)  -> test in 043-00
replace_sign  empty
# floor_sub (24*11/2 = 132) -> test in 100-12
replace_sign floor_sub
# floor_top (24*11/2=132) -> test in 038-15
replace_sign floor_top
# port (16*11/2 = 88) -> test in 039-00
replace_sign  port
# uptown/innertown (40*11/2) -> test in 061-00
replace_sign  uptown
# (underground) dungeon (88*12/2) -> test in 000-06
replace_sign  dungeon
# witch's (40*11/2=220) -> test in 150-00 as proserpina
replace_sign  witch
# old (40*11/2=220) -> test in 153-00 as gradriel
replace_sign  old
# hut (24*11/2=132) -> test in 150-00
replace_sign  hut
# mysterious' shop  (40*11/2=220) -> test in 152-00
replace_sign  mysterious
# inn_room (24*11/2=132) -> test in 055-00
replace_sign  inn_room
# mine (24*11/2=132) -> test in 055-00
replace_sign  mine
# King's Palace (40*11/2=220) -> test in 055-00
replace_sign  kings
# truck/track (48*10/2=240) -> test in 055-00
replace_sign  track
# chase (64*11/2=352) -> test in 055-00
replace_sign  chase
# stones/gems shop (40*11/2=220) -> test in 055-00
replace_sign  gem_shop   DC1T.CHR
# (to the) tower (72*11/2=396) -> test in 036-00
replace_sign  tower
# (to the) tower2 (72*11/2=396) -> test in 094-00 as Portgus
replace_sign  tower2
# out/exit (16*11/2=88) -> test in 038-01
replace_sign  out
# Goblin market (112*11/2=616) -> test in 154-00
replace_sign  goblin_market
# Goblin market2 (112*11/2=616) -> test in 010-00
replace_sign  goblin_market2
# Volga castle (64*11/2=352) -> test in 081-00
replace_sign  volga_castle
# gem_shop2 (24*11/2=132) -> test in 081-00
replace_sign  gem_shop2  VG1T.CHR 
# aud_hall2 (48*12/2=288) -> test in 079-03
replace_sign  aud_hall2
# jadis' (72*11/2=396) -> test in 079-03
replace_sign  jadis
# Pumpkin House (96*11/2=528) -> test in 022-00
replace_sign  pumpkin_house
# cave (24*11/2=132) -> test in 047-00
replace_sign  cave  MR1C.CHR
# southwest (40*11/2=220) -> test in 033-00
replace_sign  southwest  MR1C.CHR
# southeast (40*11/2=220) -> test in 047-00
replace_sign  southeast  MR1C.CHR
# Heindel's (72*11/2=396) -> test in 148-07
replace_sign  heindel
# Goat (hut) (40*11/2=220) -> test in 026-00
replace_sign  goat
# Ranch (24*11/2=132) -> test in 026-00
replace_sign  ranch
# Desert (64*11/2=352) -> test in 031-00
replace_sign  desert
# Dragon's nest (88*11/2=484) -> test in 017-00
replace_sign  dragonnest
# Portal (To another space) (48*11/2=264) -> test in 000_07_B
replace_sign  portal
# Elfaran (88*10/2=440) -> test in 000-04 (old castle, save-dependent)
replace_sign  elfaran
# (Guest) Room (24*11/2=132) -> test in 000-04 (old castle, save-dependent)
replace_sign  room_guest
# "he" kana (8*5/2=20) -> test in 055-00, cleared
replace_sign  he_kana2
# artist (24*11/2=132) -> test in 039-03 (save-dependent)
replace_sign  artist
# 's house (24*11/2=132) -> test in 039-03 (save-dependent)
replace_sign  shouse
# Grass Meadow (72*11/2=396) -> test in 058-00 (save-dependent)
replace_sign  magic_meadow
# Last (Tavern/Inn) (40*11/2=220) -> test in 083-00
replace_sign  last
# Old (Cave) (24*11/2=132) -> test in 087-00 (Edward book)
replace_sign  old2
# (Old) Cave (24*11/2=132) -> test in 087-00 (Edward book)
replace_sign  cave2
# (Lost) Forest (48*11/2=264) -> test in 166-00 (Proserpina book)
replace_sign  lost_forest  LL1T.CHR


# castle signs cleanups -> test in 000-04
#WIP: python _edit_texture.py VD14.PAK 13C -2 -1
sfk setbytes VD14.PAK  0x13E 0xFFFFFFFFFFFFFFFF -yes  # hide "'s" in Gradriel's room
sfk setbytes VD14.PAK  0x1CE 0xFFFFFFFFFFFFFFFF -yes  # hide "'s" in Eriel's room
sfk setbytes VD14.PAK  0x186 0xFFFFFFFFFFFFFFFF -yes  # hide "'s" in Sidrael's room
sfk setbytes VD14.PAK  0x25E 0xFFFFFFFFFFFFFFFF -yes  # hide "'s" in Kwein's room
# TODO: resize the frames too:
#sfk setbytes EE1C.PAK  0x252  0x14062b062b041404 -yes  # Kwein's room - room
#sfk setbytes EE1C.PAK  0x252  0x200d2e0d2e0c200c -yes  # Kwein's room - center frame
#sfk setbytes EE1C.PAK  0x25E  0x280d210d210c280c -yes  # Kwein's room - left frame
sfk setbytes VD14.PAK  0x216 0xFFFFFFFFFFFFFFFF -yes    # hide "'s" in Jestonai's room

# Jestonai's Room recentering
#TODO: sfk setbytes VD14.PAK 0x1fc 0x00032b06080608052b05004b -yes  # move Jestonai left 10px
sfk setbytes VD14.PAK 0x208 0x000814062b062b041404000a -yes  # move Room left 10px

# TODO: recenter Jadis' Room -> test in 079-03
# sfk setbytes VG11.PAK ...
 
# TODO: recenter Mine   -> test in 055-00
 
# TODO: recenter Inn  

# cleanup "Top Room" sign  -> test in 038-20  https://github.com/eadmaster/pcrown/issues/5#issuecomment-2629013480
sfk setbytes EE1C.PAK  0x5e8  0x00141b05040504051b05005F  -yes  # reposition Top 10R
sfk setbytes EE1C.PAK  0x5f4  0x00100005170517050005000A  -yes  # reposition Room 12L
sfk setbytes EE1C.PAK  0x5c4  0x40221b0C220C220C1b0C000A  -yes  # reposition frame right 11L
sfk setbytes EE1C.PAK  0x5d0  0x40211B0C1B0C1B0C1B0C004B  -yes  # reposition frame center -11L & -11R
sfk setbytes EE1C.PAK  0x5DC  0x4020230C1C0C1C0C230C005F  -yes  # reposition frame left 11R
sfk setbytes EE1C.PAK  0x602  0xFFFFFFFFFFFFFFFF000A -yes       # hide "F" part
sfk setbytes EE1C.PAK  0x60E  0xFFFFFFFFFFFFFFFF000A -yes       # hide "'s" part


## resize some signs  https://github.com/eadmaster/pcrown/issues/102

# Port -> test in 039-00
sfk setbytes RL1T.PAK 0x6c 0x18 -yes  # width to 24px
sfk setbytes RL1T.PAK 0x2f4 0x0A050A -yes  # change texture size
#sfk setbytes RL1T.PAK 0x2fe 0x0000000000000000 -yes  # null "he" part

# Ranch -> test in 026-00
sfk setbytes LL1T.PAK 0x74 0x20 -yes  # width to 32px
sfk setbytes LL1T.PAK 0x2a0 0x0D050D  -yes  # change texture size 

# Bakery -> test in 004-00, 026-00, 081-00
sfk setbytes VG1T.PAK 0x58 0x00206C98280c000E -yes  #Volga Bakery segment entry
sfk setbytes LL1T.PAK 0x5c 0x280c -yes  # width to 32px x 12px
sfk setbytes NB1T.PAK 0x64 0x280c -yes  # width to 32px x 12px
sfk replace -binary /1205050505041204004B/1406130613051405004B/  -yes -firsthit -dir . -file VG1T.PAK LL1T.PAK NB1T.PAK

# item_shop
sfk setbytes RL1T.PAK 0x5c 0x300c -yes  # size 24x11->48x12
sfk setbytes VD1T.PAK 0x5c 0x300c -yes  # size 24x11->48x12
sfk setbytes DR1T.PAK 0x74 0x300c -yes  # size 24x11->48x12
sfk setbytes GB1T.PAK 0x54 0x300c -yes  # size 24x11->48x12
sfk setbytes KD2T.PAK 0x54 0x300c -yes  # size 24x11->48x12
sfk setbytes NB1T.PAK 0x5c 0x300c -yes  # size 24x11->48x12
sfk setbytes NE1T.PAK 0x74 0x300c -yes  # size 24x11->48x12
sfk setbytes VG1T.PAK 0x54 0x300c -yes  # size 24x11->48x12
sfk setbytes YA1C.PAK 0x9C 0x300c -yes  # size 24x11->48x12
#sfk setbytes RL1T.PAK 0x224 0x00031705180518061706 -yes  # change texture size
sfk replace -binary /1105060506051105004B/1705180518061706004B/ -yes -firsthit -dir . -file RL1T.PAK DR1T.PAK GB1T.PAK KD2T.PAK NB1T.PAK  NE1T.PAK RL1T.PAK VD1T.PAK VG1T.PAK YA1C.PAK
# TODO: adjust frames via a sfk xreplace pattern like "/[l frame coords][4 bytes][c frame coords][4 bytes][r frame coords][4 bytes][item segment coords][4 bytes][shop segment coords]/[new l frame coords][part 1].../"

# potions_shop -> test in 002-00, 043-01, ...
sfk setbytes VD1T.PAK 0xa8  0x0020EEC8400c0070 -yes  # size 16x11->64x12
sfk setbytes YA1C.PAK 0xf8  0x00205F4C400c0044 -yes  # size 16x11->64x12
#sfk setbytes VD1T.PAK 0x5b4 0x000D20051f051f062006 -yes  # change coords
sfk setbytes VD1T.PAK 0x590 0x400A260C2d0C2d0B260B000A -yes  # r frame
sfk setbytes VD1T.PAK 0x59c 0x4009260C250C250B260B004B -yes  # c frame
sfk setbytes VD1T.PAK 0x5a8 0x40082d0C260C260B2d0B005F -yes  # l frame
#sfk setbytes YA1C.PAK 0x5ec 0x001720051F051F062006004B -yes  # Potion segment entry
sfk setbytes YA1C.PAK 0x5c8 0x4016260C2d0C2d0B260B000A -yes  # R frame coords
sfk setbytes YA1C.PAK 0x5d4 0x4015260C250C250B260B004B -yes  # C frame coords
sfk setbytes YA1C.PAK 0x5e0 0x40142d0C260C260B2d0B005F -yes  # L frame coords
sfk replace -binary /0D05020502050D05004B/20051f051f062006004B/ -yes -firsthit -dir . -file VD1T.PAK YA1C.PAK  # coords
# TODO: also in DC1T ?

# gem_shop -> test in 055-00
sfk setbytes DC1T.PAK 0xb0 0x00201138300c000E -yes  # size 40x11->48x12
sfk setbytes DC1T.PAK 0x354 0x000E1805170517061806004B -yes  # change texture size
# TODO: adjust frame
# TODO: also in DC1C ?

# gem_shop2 -> test in 081-00
sfk setbytes VG1T.PAK 0x70 0x00206EC8300c0012 -yes  # Gem segment entry
sfk setbytes VG1T.PAK 0x22c 0x00061805170517061806004B -yes  # Gem segment coords
#sfk setbytes VG1T.PAK 0x238 0x00080000000000000000000A -yes  # Blank $ segment coords for gem shop
# TODO: also in DC1T ?

# King's Palace -> test in 055-00
sfk setbytes DC1T.PAK 0xa8 0x0020105C480c000D -yes  # King segment entry
sfk setbytes DC1T.PAK 0x324 0x000D2405230523062406004B -yes  # King segment coords
sfk setbytes DC1T.PAK 0x300 0x4014250C2C0C2C0C250C000A -yes  # r frame coords
sfk setbytes DC1T.PAK 0x30c 0x4013240C240C240C240C004B -yes  # c frame coords
sfk setbytes DC1T.PAK 0x318 0x40122c0C250C250C2c0C005F -yes  # l frame coords

# (Magic) school -> test in 045-00
sfk setbytes KD3T.PAK 0x60 0x00206FD0480C0014 -yes  # 'School'->'Magic School' segment entry
sfk setbytes KD3T.PAK 0x1f6 0x0000000000000000 -yes  # 'Magic' segment coords blank
sfk setbytes KD3T.PAK 0x200 0x00042405230523062406004b -yes  # segment coords
sfk setbytes KD3T.PAK 0x1d0 0x4008230C2A0C2A0C230C000A -yes  # R frame coords
sfk setbytes KD3T.PAK 0x1dc 0x4007220C220C220C220C004B -yes  # C frame coords
sfk setbytes KD3T.PAK 0x1e8 0x40062A0C230C230C2A0C005F -yes  # L frame coords

# warp (Magic) -> test in 045-00
#sfk setbytes KD3T.PAK 0x88 0x0020E2BC380C0068 -yes  # segment entry
sfk setbytes KD3T.PAK 0x8C 0x380C -yes  # size change
sfk setbytes NE1T.PAK 0xB4 0x380C -yes  # size change
#TODO: sfk setbytes SM1T.PAK 0x8C 0x380C -yes  # size change
sfk setbytes VD1T.PAK 0x9C 0x380C -yes  # size change
sfk setbytes VG1T.PAK 0xAC 0x380C -yes  # size change
#sfk setbytes KD3T.PAK 0x554 0x00091c051b051b061c06004B  -yes  # segment coords
sfk replace -binary /1E05070507051E05005F/1C051B051B061C06004B/ -yes -firsthit -dir . -file KD3T.PAK NE1T.PAK SM1T.PAK VD1T.PAK VG1T.PAK   # coords
#sfk setbytes KD3T.PAK 0x54a 0x0000000000000000 -yes  # 'Magic' segment coords blank
sfk replace -binary /0605110511050605004B/0000000000000000004B/ -yes -firsthit -dir . -file KD3T.PAK NE1T.PAK SM1T.PAK VD1T.PAK VG1T.PAK   # coords

# magic_shop -> test in 045-00, ...
#OLD: file_patch KD3T.CHR $(file2hexstr ${SIGNS_PATH}/magic_shop_jap.bin ; echo "0000000000000000000000F000000FFFFFF000000F00FFFFF0000000") $(file2hexstr ${SIGNS_PATH}/magic_shop_eng.bin)
# ...
#sfk setbytes KD3T.PAK 0x68 0x002083EC380C002B -yes  # segment entry
#sfk setbytes KD3T.PAK 0x6C 0x380C -yes  # size change
#sfk setbytes NE1T.PAK 0x8C 0x380C -yes  # size change
#sfk setbytes SG1T.PAK 0x64 0x380C -yes  # size change
#TODO: sfk setbytes SM1T.PAK 0x380C -yes  # size change
#sfk setbytes VD1T.PAK 0x74 0x380C -yes  # size change
#sfk setbytes VG1T.PAK 0x84 0x380C -yes  # size change
#sfk setbytes KD3T.PAK 0x1ac 0x00051c051b051b061c06004B -yes  # segment coords
#sfk replace -binary /0705160516050705000A/1C051B051B061C06004B/ -yes -dir . -file KD3T.PAK NE1T.PAK SG1T.PAK SM1T.PAK VD1T.PAK VG1T.PAK   # coords
#sfk setbytes KD3T.PAK 0x1a2 0x0000000000000000 -yes  # 'Magic' segment coords blank
#sfk replace -binary /1105060506051105004B/0000000000000000004B/ -yes -dir . -file KD3T.PAK NE1T.PAK SG1T.PAK SM1T.PAK VD1T.PAK VG1T.PAK   # coords
#sfk setbytes KD3T.PAK 0x17c 0x40081E0C250C250C1E0C000A -yes  # R frame coords
#MULTI-MATCH: sfk replace -binary /150C1C0C1C0C150C000A/1E0C250C250C1E0C000A/ -yes -dir . -file KD3T.PAK NE1T.PAK SG1T.PAK SM1T.PAK VD1T.PAK VG1T.PAK   # coords
#sfk setbytes KD3T.PAK 0x188 0x40071C0C1D0C1D0C1C0C004B -yes  # C frame coords
#MULTI-MATCH: sfk replace -binary /130C140C140C130C004B/1C0C1D0C1D0C1C0C004B/ -yes -dir . -file KD3T.PAK NE1T.PAK SG1T.PAK SM1T.PAK VD1T.PAK VG1T.PAK   # coords
#sfk setbytes KD3T.PAK 0x194 0x4006240C1D0C1D0C240C005F -yes  #Cadho Badho 'Magic Shop' L frame coords
#MULTI-MATCH: sfk replace -binary /1B0C140C140C1B0C005F/240C1D0C1D0C240C005F/ -yes -dir . -file KD3T.PAK NE1T.PAK SG1T.PAK SM1T.PAK VD1T.PAK VG1T.PAK   # coords

# Elfaran's -> test in 000-04 (old castle, save-dependent)
sfk setbytes VD14.PAK 0xd4 0x380b -yes  # segment entry dimensions 88x10->56x11
sfk setbytes VD14.PAK 0x298 0x00122c050b050b052c05004B -yes  # "Elfaran" segment coords
sfk setbytes VD14.PAK 0x2a4 0x00081105280528051105000A -yes  # "Room" segment coords
sfk setbytes VD14.PAK 0x2b2 0x0000000000000000 -yes  # blank " 's" segment
sfk setbytes VD14.PAK 0x274 0x40172C0D330D330C2C0C000A -yes  # R frame coords
sfk setbytes VD14.PAK 0x280 0x40162b0D2b0D2b0C2b0C004B -yes  # C frame coords
sfk setbytes VD14.PAK 0x28c 0x4015330D2C0D2C0C330C005F -yes  # L frame coords

# Guestroom -> test in 000-04 (old castle, save-dependent)
sfk setbytes VD14.PAK 0xdc 0x38 -yes  # segment entry dimensions 24x11->56x11
sfk setbytes VD14.PAK 0x2e0 0x00131b061C061C041B04004B -yes  # segment coords
sfk setbytes VD14.PAK 0x2bc 0x40171F0D260D260C1F0C000A -yes  # R frame coords
sfk setbytes VD14.PAK 0x2c8 0x40161E0D1E0D1E0C1E0C004B -yes  # C frame coords
sfk setbytes VD14.PAK 0x2d4 0x4015260D1F0D1F0C260C005F -yes  # L frame coords

# aud_hall -> test in 000-03
sfk setbytes VD13.PAK 0x7c 0x48 -yes  # entry dimensions 48x11->72x11
sfk setbytes VD13.PAK 0x140 0x00072305240524052305004B -yes  # coords
sfk setbytes VD13.PAK 0x11c 0x4010250D2C0D2C0C250C000A -yes  # R frame coords
sfk setbytes VD13.PAK 0x128 0x400F240D240D240C240C004B -yes  # C frame coords
sfk setbytes VD13.PAK 0x134 0x400E2B0D230D230C2B0C005F -yes  # L frame coords

# aud_hall2 -> test in 079-03
sfk setbytes VG11.PAK 0x6c 0x480b -yes  # entry dimensions 48x12->72x11
sfk setbytes VG11.PAK 0x1e4 0x00072305240524052305004B -yes  # coords
sfk setbytes VG11.PAK 0x1c0 0x4010250D2C0D2C0C250C000A -yes  # R frame coords
sfk setbytes VG11.PAK 0x1cc 0x400F240D240D240C240C004B -yes  # C frame coords
sfk setbytes VG11.PAK 0x1d8 0x400E2B0D230D230C2B0C005F -yes  # L frame coords 

# Courtyard  -> test in 000-01
sfk setbytes VD11.PAK 0xfc 0x380c -yes  # entry dimensions 24x11->56x12
sfk setbytes VD11.PAK 0x2a4 0x00171C051B051B061C06004B -yes  # coords
sfk setbytes VD11.PAK 0x280 0x401A1C0C230C230C1C0C000A -yes  # R frame coords
sfk setbytes VD11.PAK 0x28c 0x40191C0C1B0C1B0C1C0C004B -yes  # C frame coords
sfk setbytes VD11.PAK 0x298 0x4018240C1D0C1D0C240C005F -yes  # L frame coords
#sfk setbytes TROC.PAK 0x114 0x380c -yes  # entry dimensions 24x11->56x12
#sfk setbytes TROC.PAK 0x250 0x001A1C051B051B061C06004B -yes  # coords
#sfk setbytes TROC.PAK 0x280 0x??? -yes  # R frame coords
#sfk setbytes TROC.PAK 0x28c 0x??? -yes  # C frame coords
#sfk setbytes TROC.PAK 0x298 0x??? -yes  # L frame coords

# lost_forest  -> test in 166-00 (Proserpina book)
sfk setbytes LL1T.PAK 0x7c 0x40 -yes  # entry dimensions 48x11->64x11
sfk setbytes LL1T.PAK 0x2d8 0x000720051F051F052005004B -yes  # coords
# TODO: also in DC1T ?
  
# Southwest  -> test in 033-00
sfk setbytes MR1C.PAK 0x13c 0x38 -yes  # entry dimensions 40x11->56x11
sfk setbytes MR1C.PAK 0x454 0x001F2A060D060D042A04004B -yes  # Southwest coords
sfk setbytes MR1C.PAK 0x460 0x00211206290629041204000A -yes  # Cave coords
sfk setbytes MR1C.PAK 0x430 0x40262C0D330D330B2C0B000A -yes  # R frame coords
sfk setbytes MR1C.PAK 0x43c 0x40252B0D2B0D2B0B2B0B004B -yes  # C frame coords
sfk setbytes MR1C.PAK 0x448 0x4024330D2C0D2C0B330B005F -yes  # L frame coords
 
# Southeast  -> test in 047-00
sfk setbytes MR1C.PAK 0x144 0x38 -yes  # entry dimensions 40x11->56x11
sfk setbytes MR1C.PAK 0x3b8 0x00202A060D060D042A04004B -yes  # Southeast coords
sfk setbytes MR1C.PAK 0x3c4 0x00211206290629041204000A -yes  # Cave coords
sfk setbytes MR1C.PAK 0x394 0x40262C0D330D330B2C0B000A -yes  # R frame coords
sfk setbytes MR1C.PAK 0x3a0 0x40252B0D2B0D2B0B2B0B004B -yes  # C frame coords
sfk setbytes MR1C.PAK 0x3ac 0x4024330D2C0D2C0B330B005F -yes  # L frame coords

#TODO: Exit  -> test in 038-01


## more gfx hacks

# replace BEGIN text in load save dialog  https://github.com/eadmaster/pcrown/issues/90
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" COCKPIT.CHB  
file_patch COCKPIT.CHB $(file2hexstr ${SIGNS_PATH}/save_begin_jap.bin) $(file2hexstr ${SIGNS_PATH}/save_begin_eng.bin)
cd-replace  "$PATCHED_IMAGE_FILE" COCKPIT.CHB COCKPIT.CHB

# fix Engrish in names https://github.com/eadmaster/pcrown/issues/93
# PORTGAS->PORTGUS banner
#ALREADY EXTRACTED: 7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" COMM.CHR
file_patch COMM.CHR $(file2hexstr ${SIGNS_PATH}/portgus_jap.bin) $(file2hexstr ${SIGNS_PATH}/portgus_eng.bin)

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


## fix Engrish in the map  https://github.com/eadmaster/pcrown/issues/87
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

# fix Engrish in the ending scrolls
# "SCENARIO WRITING & VILLAGERS SET" (4bpp 232x16 1856B)
file_patch ROLL.CHR $(file2hexstr ${SIGNS_PATH}/credits_1_jap.bin) $(file2hexstr ${SIGNS_PATH}/credits_1_eng.bin)
# sfk setbytes ROLL.CHR  0x1140  0x$(file2hexstr ${SIGNS_PATH}/credits_1_eng.bin) -yes 
# "SCENARIO WRITING & EVENT DESIGN" (4bpp 224x16 1792B)
file_patch ROLL.CHR $(file2hexstr ${SIGNS_PATH}/credits_2_jap.bin) $(file2hexstr ${SIGNS_PATH}/credits_2_eng.bin)
# sfk setbytes ROLL.CHR  0x1880 0x$(file2hexstr ${SIGNS_PATH}/credits_2_eng.bin) -yes 

# replace all the chr files with updated signs
find *.CHR -mtime -1 | while read chrfile; do     
    cd-replace  "$PATCHED_IMAGE_FILE"  $chrfile $chrfile  > /dev/null
done

# replace all the pak files with updated data
find *.PAK -mtime -1 | while read pakfile; do     
    cd-replace  "$PATCHED_IMAGE_FILE"  $pakfile $pakfile  > /dev/null
done


## fix Engrish in enemy banner names https://github.com/eadmaster/pcrown/issues/93
7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" '*.PRG'  > /dev/null
sfk replace DOHDOH.PRG -text '/DOHDOH/ DODO /' -yes
sfk replace CEYE.PRG -text '/CHAOTHIC EYE/CHAOTIC EYE /'  -yes
sfk replace NECRO.PRG -text '/NECRO SAMANSA/NECRO-SAMANTHA/'  -yes  # 1-byte overflow
sfk replace RYON.PRG -text '/RYON/LEON/'  -yes
sfk replace SIRENE.PRG -text '/SIRENE/SIREN /'  -yes
sfk replace WGOD.PRG -text '/EVIL GOLGODA/EVIL GOLGOTHA/'  -yes  # 1-byte overflow
sfk replace NISE.PRG -text '/CAPTAIN BIGFOOK/CAPTAIN BIGHOOK/'  -yes
sfk replace POLT.PRG -text '/SLAVE WOOD/SLAVEWOOD /'  -yes
sfk replace PUPPET.PRG -text '/SLAVE WOOD/SLAVEWOOD /'  -yes
sfk replace VORG.PRG -text '/VORGLODO/VOLGROD /'  -yes
sfk replace KUMO.PRG -text '/TARANTURA/TARANTULA/'  -yes
find *.PRG -mtime -1 | while read prgfile; do     
    cd-replace  "$PATCHED_IMAGE_FILE"  $prgfile $prgfile  > /dev/null
done