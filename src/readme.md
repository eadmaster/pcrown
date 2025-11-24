
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
4. From the terminal, `cd` to the extraction path (e.g. `cd $HOME/pcrown`) and execute [docker_build.sh](https://github.com/eadmaster/pcrown/blob/master/docker_build.sh).
5. When the patching process is complete, a file `Princess.Crown.Japan.1M.Track.01.bin.xdelta` is created in the same directory.


## Without Docker / manual

1. Setup a Linux environment based on [Ubuntu 22.04 LTS](https://releases.ubuntu.com/jammy/).
> [!TIP]
> You can use a Virtual Machine or [WSL](https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux).
2. Download [all the repository files](https://github.com/eadmaster/pcrown/archive/refs/heads/master.zip), and extract them into a writable directory like `$HOME/pcrown`.
3. Extract a game image matching [these hashes](http://redump.org/disc/4901/) into `$HOME/pcrown/src/buildcd`
4. Install all the [dependencies](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/_build_install_deps.sh)
5. From the terminal, `cd` to the extraction path (e.g. `cd $HOME/pcrown/src/buildcd`) and execute `_build.sh`.
6. When the patching process is complete, a file `Princess.Crown.Japan.1M.Track.01.bin.xdelta` is created in the same directory.

> [!NOTE]
> If you want to share the patch with some changes you've made, please include the source code and a link to this repository to give attribution.
