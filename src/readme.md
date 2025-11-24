 

```
├── buildcd      # main patching scripts (vanilla version)
├── buildcd_ex   # patching scripts for the EX version https://github.com/eadmaster/pcrown/issues/18
├── buildcd_lt   # patching scripts for the LT version https://github.com/eadmaster/pcrown/issues/18
├── buildcd_psp  # patching scripts for the PSP version https://github.com/eadmaster/pcrown/issues/92
├── buildcd_reva # patching scripts for the Satakore version https://github.com/eadmaster/pcrown/issues/110
├── eventeditor  # tool to patch event files
├── itemsutil    # tool to patch names and items
└── *.old        # old code no longer used
```

# How to build

1. setup a Linux environment based on [Ubuntu 22.04 LTS](https://releases.ubuntu.com/jammy/).
> [!TIP]
> You can use a Virtual Machine, [Docker](https://github.com/eadmaster/pcrown/blob/master/Dockerfile) or [WSL](https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux).
2. download [all the repository files](https://github.com/eadmaster/pcrown/archive/refs/heads/master.zip), and extract them into a writable directory like `$HOME/pcrown`.
3. extract a game image matching [these hashes](http://redump.org/disc/4901/) into `$HOME/pcrown/src/buildcd`
4. install all the [dependencies listed here](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/_build.sh#L6-L9)
5. from the terminal, `cd` to the extraction path (e.g. `cd $HOME/pcrown/src/buildcd`) and execute `_build.sh`.
6. when the patching process is complete, a file `Princess.Crown.Japan.1M.Track.01.bin.xdelta` is created in the same directory.
7. you can now also make the EX and LT builds by executing `buildcd_ex/_build.sh` and `buildcd_lt/_build.sh`.

> [!NOTE]
> If you want to share the patch with some changes you've made, please include the source code and a link to this repository to give attribution.
