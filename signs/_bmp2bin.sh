

# PRE: sudo apt install imagemagick

#export WIDTH=$2
#export HEIGHT=$3
# read from the bmp itself
export WIDTH=$(identify -format '%w' $1)
export HEIGHT=$(identify -format '%h' $1)

# original green-color font:
#perl _bmp-2-4bpp.pl $1 save_begin.pal 7 $WIDTH $HEIGHT

# white-color font:
perl _bmp-2-4bpp.pl $1 save_begin.pal 0 $WIDTH $HEIGHT
