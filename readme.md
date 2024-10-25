
# Princess Crown Translation Redux

[old readme here](readme.txt.old)

My attempt to restart the CyberWarriorX translation project.

## Current status

 - all story events and names translated using a fixed-width font
   - make some lines shorter to reduce pauses: https://github.com/eadmaster/pcrown/issues/1
   - use a bigger monospaced font: https://github.com/eadmaster/pcrown/issues/3
   - some symbols in the font still missing
 - items translations missing (HELP NEEDED): https://github.com/eadmaster/pcrown/issues/2
 - needs play testing for bugs

## Preview

![welcome](https://github.com/user-attachments/assets/c86585d6-106a-409b-951b-5bbd072533f6)


## Patch instructions

1. grab the latest xdelta patch from the [Releases section](https://github.com/eadmaster/pcrown/releases).
2. get a game image matching [these hashes](http://redump.org/disc/4901/), and extract it
3. download and install `iat` and `xdelta3` (Windows binaries are here)
4. convert track 1 to iso with
`iat -i "Princess Crown (Japan) (1M) (Track 01).bin" -o "Princess Crown (Japan) (1M) (Track 01).iso" --iso`
5. apply xdelta patch with
`xdelta3 -d -s  "Princess Crown (Japan) (1M) (Track 01).iso"  "Princess.Crown.Japan.1M.Track.01.iso.xdelta"  "Princess Crown (Japan) (1M) (Track 01) (English).iso"`
6. use [this cue sheet][(https://github.com/eadmaster/pcrown/blob/master/src/buildcd/Princess%20Crown%20(Japan)%20(1M).cue](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/Princess%20Crown%20(Japan)%20(1M)%20(English).cue)) to play with the patched iso.

On Windows you can use [this script](https://github.com/eadmaster/pcrown/releases/download/v0.3/Princess.Crown.ez.Winodws.patching.script.zip) to automate the process.


## Contributions
 
Issues, PRs and discussions are currently restricted to contributors to avoid spamming.

Contact me via forums if you can contribute:

 - https://segaxtreme.net/threads/help-me-translate-princess-crown.18555/
 - https://gbatemp.net/threads/princess-crown-psp-any-updates.645443/

Currently i mostly need help with the code part, a [semi-complete translation](https://github.com/eadmaster/pcrown/tree/master/script/eng) was already contributed by SamIAm.
