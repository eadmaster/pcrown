
# How to retranslate a doorway sign

 1. open "mysign_eng.bmp" with a paint tool, change the text using the font and color in `_font.png` or `_font_condensed` dir. Then save.
> [!TIP]
> You can use `_bmpbanner.sh` to quickly generate the bitmap with any text, then copy-paste into the existing sign.

> [!IMPORTANT]
> Don't change the bitmap size if the text does not fit. Doing so will break all the texture data offsets. Need to rehack the pak files for that, see https://github.com/eadmaster/pcrown/issues/102 .

 2. run `_bmp2bin.sh mysign_eng.bmp` to generate the binary texture file `mysign_eng.bin`
 3. run `src/buildcd/_patch_signs.sh` to update the game image
 4. test the new sign ingame

> [!TIP]
> You can use the [in-game debug mode](https://github.com/eadmaster/pcrown/wiki/Extras#cheats) for quick testing.

> [!CAUTION]
> Alternatively, if using savestates, keep in mind the current signs are cached in RAM, so you may need to go through multiple black loading screens to reload from disk.
