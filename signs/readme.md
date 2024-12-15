
# How to translate a doorway sign

 - find the sign ingame, and save the bmp from the Yabause Debug->VDP1 menu as "mysign_jap.bmp" ("mysign" will be used as a reference name in the code).
 - copy "mysign_jap.bmp" to "mysign_eng.bmp"
 - open "mysign_eng.bmp" with a paint tool, translate to English using the font and color in `_font.png`. Then save (don't change its size if the translation does not fit, it won't work).

OPTIONAL:  test the new sign ingame:

 - convert both "mysign_jap.bmp" and "mysign_eng.bmp" with the `_bmp2bin.sh` script.
 - add a line like this in `src/buildcd/_signs_test.sh` and update the iso:
 ````
     # mysign ($WIDTH * $HEIGHT) -> test in ...
    replace_sign  mysign  $MYSIGN_BIN_FILESIZE  # replace $WIDTH $HEIGHT $MYSIGN_BIN_FILESIZE with the correct values
 ````
 