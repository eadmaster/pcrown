
EVENT_NAME=$1
TRANSLATED_SCRIPT_PATH=../../script/eng

if [ ! -f "${EVENT_NAME}.EVN" ]; then
    echo "${EVENT_NAME}.EVN missing"
    exit 1
fi

python _split_long_lines.py  "${TRANSLATED_SCRIPT_PATH}/events/${EVENT_NAME}.TXT" "${TRANSLATED_SCRIPT_PATH}/events_splitted_35chars/${EVENT_NAME}.TXT"

#7z e -y "Princess Crown (Japan) (1M) (Track 01).iso" ${EVENT_NAME}.EVN

wine eventeditor.exe -i ${EVENT_NAME}.EVN  "${TRANSLATED_SCRIPT_PATH}/events_splitted_35chars/${EVENT_NAME}.TXT"  -o ${EVENT_NAME}.EVN.eng

cd-replace  "Princess Crown (Japan) (1M) (Track 01) (English).iso" ${EVENT_NAME}.EVN  ${EVENT_NAME}.EVN.eng