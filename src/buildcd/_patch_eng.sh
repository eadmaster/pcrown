
#PRE: apt install iat xdelta3

# LATEST_PATCH_DOWNLOAD_URL=$(curl -s https://api.github.com/repos/eadmaster/pcrown/releases/latest | grep "browser_download_url.*xdelta" | cut -d : -f 2,3 | tr -d \")

iat -i "Princess Crown (Japan) (1M) (Track 01).bin" -o "Princess Crown (Japan) (1M) (Track 01).iso"  --iso

xdelta3 -d -s  "Princess Crown (Japan) (1M) (Track 01).iso"  "Princess.Crown.Japan.1M.Track.01.iso.xdelta"  "Princess Crown (Japan) (1M) (Track 01) (English).iso" 

rm "Princess Crown (Japan) (1M) (Track 01).iso"
