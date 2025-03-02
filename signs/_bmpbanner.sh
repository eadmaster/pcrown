#!/bin/bash

if [ -z "$1" ] || [ "$1" = "-h" ] || [ "$1" = "--help" ]; then
	echo "usage: $(basename $0) text"
	echo
	exit 1
fi

#FONT_DIR="_font_normal"  # Directory containing character BMP files
FONT_DIR="_font_condensed"  # Directory containing character BMP files
OUTPUT_FILE="$@_eng.bmp"  # Output banner file
TEXT="$@"  # Text to render

# Temporary file to hold intermediate results
TEMP_FILE="/tmp/temp_banner.bmp"

# Initialize an empty banner
#convert -size 1x12 xc:black $TEMP_FILE
cp he_kana_eng.bmp  $TEMP_FILE

# Loop through each character in the text
for ((i=0; i<${#TEXT}; i++)); do
    # Get the current character
    CHAR="${TEXT:i:1}"
    
    # Handle spaces explicitly
    if [[ "$CHAR" == " " ]]; then
        # Create a blank space image (adjust width for spacing)
        convert -size 5x12 xc:black /tmp/space.bmp
        CHAR_FILE="/tmp/space.bmp"
    else
        # Path to the corresponding BMP file
        CHAR_FILE="$FONT_DIR/${CHAR}.bmp"
    fi

    if [[ -f "$CHAR_FILE" ]]; then
        # Append the character to the banner
        convert +append $TEMP_FILE "$CHAR_FILE" $TEMP_FILE
    else
        echo "Warning: No BMP file found for character '$CHAR'."
    fi
done

# Save the final banner
convert $TEMP_FILE -background black -flatten -depth 8 -type TrueColor -crop +16+0 +repage "$OUTPUT_FILE"


echo "Banner saved to $OUTPUT_FILE"
