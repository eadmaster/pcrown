
export EVENT_NAME=$1
export TRANSLATED_SCRIPT_PATH=../../script/eng
export PATCHED_IMAGE_FILE="Princess Crown (Japan) (1M) (Track 01) (patched).bin"

if [ ! -f "${EVENT_NAME}.EVN" ]; then
    echo "${EVENT_NAME}.EVN missing"
    exit 1
fi

python3 _split_long_lines.py  "${TRANSLATED_SCRIPT_PATH}/events/${EVENT_NAME}.TXT" "${TRANSLATED_SCRIPT_PATH}/events_splitted/${EVENT_NAME}.TXT"

#7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" ${EVENT_NAME}.EVN

wine eventeditor.exe -i ${EVENT_NAME}.EVN  "${TRANSLATED_SCRIPT_PATH}/events_splitted/${EVENT_NAME}.TXT"  -o ${EVENT_NAME}.EVN.eng

cd-replace  "$PATCHED_IMAGE_FILE" ${EVENT_NAME}.EVN  ${EVENT_NAME}.EVN.eng

#xdelta3 -S none -f -e -s "Princess Crown (Japan) (1M) (Track 01).bin" "$PATCHED_IMAGE_FILE"  "Princess.Crown.Japan.1M.Track.01.bin.xdelta"
