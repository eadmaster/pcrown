
```
├── buildcd      # main patching tools (vanilla version)
├── buildcd_ex   # patching tools for the EX version https://github.com/eadmaster/pcrown/issues/18
├── buildcd_lt   # patching tools for the LT version https://github.com/eadmaster/pcrown/issues/100
├── buildcd_psp  # patching tools for the PSP version https://github.com/eadmaster/pcrown/issues/92
├── buildcd_reva # patching tools for the Satakore version https://github.com/eadmaster/pcrown/issues/110
├── eventeditor  # tool to patch event files (source code)
├── itemsutil    # tool to patch names and items (source code)
└── *.old        # old code no longer used
```

# How to build

## With Docker (easier)

1. Install [Docker CE](https://docs.docker.com/engine/install/).
2. Download [all the repository files](https://github.com/eadmaster/pcrown/archive/refs/heads/master.zip), and extract them into a writable directory like `$HOME/pcrown`.
3. Extract a game image matching [these hashes](http://redump.org/disc/4901/) into `$HOME/pcrown/src/buildcd`
4. From the terminal, `cd` to the extraction path (e.g. `cd $HOME/pcrown`) and execute [docker_build.sh](https://github.com/eadmaster/pcrown/blob/master/docker_build.sh) (on Windows just rename as `docker_build.bat`).
5. When the patching process is complete, a file `Princess.Crown.Japan.1M.Track.01.bin.xdelta` is created in the same directory.


## Without Docker (manual)

1. Setup a Linux environment based on [Ubuntu 22.04 LTS](https://releases.ubuntu.com/jammy/).
> [!TIP]
> You can use a Virtual Machine or [WSL](https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux).
2. Download [all the repository files](https://github.com/eadmaster/pcrown/archive/refs/heads/master.zip), and extract them into a writable directory like `$HOME/pcrown`.
3. Extract a game image matching [these hashes](http://redump.org/disc/4901/) into `$HOME/pcrown/src/buildcd`
4. Install all the [dependencies](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/_build_install_deps.sh)
5. From the terminal, `cd` to the extraction path (e.g. `cd $HOME/pcrown/src/buildcd`) and execute `_build.sh`.
6. When the patching process is complete, a file `Princess.Crown.Japan.1M.Track.01.bin.xdelta` is created in the same directory.


## Tips for new translations

After the first build is complete:

 - to quickly repatch a single event file (e.g to fix a typo), use the [_update_single_event.sh script](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/_update_single_event.sh). 
     e.g. `_update_single_event.sh 000_00_1"` will repatch `000_00_1.EVN` using [000_00_1.TXT](https://github.com/eadmaster/pcrown/blob/master/script/eng/events/000_00_1.TXT)
   - Use the [debug mode](https://tcrf.net/Princess_Crown_(Sega_Saturn)#Debug_Features) to quickly test an event.
   - [List of event numbers and game locations](https://github.com/eadmaster/pcrown/wiki/FileStructure#files-reference)
 - to quickly repatch the main font and all the items names/descriptions, execute `_patch_items.sh`.
   - [Items text used for vanilla and EX builds](https://github.com/eadmaster/pcrown/blob/master/script/eng/items_ex.txt)
   - [Font replacement issue](https://github.com/eadmaster/pcrown/issues/3)
 - to quickly repatch all the doorway signs, execute `_patch_signs.sh`
   - [How to retranslate a doorway sign](https://github.com/eadmaster/pcrown/tree/master/signs#how-to-retranslate-a-doorway-sign)
 - to customize the copyright text in the title screen, [change this line](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/_patch_items.sh#L81).
 - to generate a new xdelta patch with the latest changes: `cd /app/src/buildcd ; xdelta3 -v -S none -f -e -s "Princess Crown (Japan) (1M) (Track 01).bin" "Princess Crown (Japan) (1M) (Track 01) (patched).bin"  "Princess.Crown.Japan.1M.Track.01.bin.xdelta"`
