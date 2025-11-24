

if [ ! -f "src/buildcd/Princess Crown (Japan) (1M) (Track 01).bin" ]; then
    echo "$0: missing: src/buildcd/Princess Crown (Japan) (1M) (Track 01).bin"
    exit 1
fi

docker build  . -t pcrown
docker rm pcrown
docker run -it  --name pcrown pcrown 
docker cp pcrown:/app/src/buildcd/Princess.Crown.Japan.1M.Track.01.bin.xdelta  .