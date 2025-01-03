

# PRE: sudo apt install imagemagick

if [ ! -f "$1" ] || [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
	echo "usage: $(basename $0) INPUT_FILE.BMP"
	echo
	exit 1
fi

#export WIDTH=$2
#export HEIGHT=$3
# read from the bmp itself
export WIDTH=$(identify -format '%w' $1)
export HEIGHT=$(identify -format '%h' $1)

# original green-color font:
#perl _bmp-2-4bpp.pl $1 save_begin.pal 7 $WIDTH $HEIGHT

# white-color font:
perl _bmp-2-4bpp.pl $1 save_begin.pal 0 $WIDTH $HEIGHT


SIGNNAME=$(echo $1 | rev | cut -d'_' -f2- | rev)
INPUTFILENAME=$(basename $1 | rev | cut -d'.' -f2- | rev)
SIGN_BIN_FILESIZE=$(stat -c%s "${INPUTFILENAME}.bin")

echo "to try this sign, add these lines in _patch_signs.sh:"
echo "# $SIGNNAME (${WIDTH}*${HEIGHT}/2=${SIGN_BIN_FILESIZE}) -> test in ... (add event number found via debug mode)"
echo "replace_sign  $SIGNNAME  $SIGN_BIN_FILESIZE"
