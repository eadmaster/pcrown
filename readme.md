
# Princess Crown Translation (Unofficial) Redux

[old readme here](readme.txt.old)

My attempt to continue on the CyberWarriorX translation project.

**P.N.: reportedly CyberWarriorX and SamIAm are still working privately on this project, and since this was forked many years ago it does not reflect the current status of their project.
Btw since this was originally released with a GPL-2.0 license, i'd like to exercise my right to keep working on this and make improvements as long as a better translation patch is released.**


## Current status

 - all story events and names translated using a fixed-width font
   - make some lines shorter to reduce pauses: https://github.com/eadmaster/pcrown/issues/1
   - use a bigger monospaced font: https://github.com/eadmaster/pcrown/issues/3
   - some symbols in the font still missing
 - items translations missing (HELP NEEDED): https://github.com/eadmaster/pcrown/issues/2
 - needs play testing for bugs

## Preview

![welcome](https://github.com/user-attachments/assets/c86585d6-106a-409b-951b-5bbd072533f6)  ![npc](https://github.com/user-attachments/assets/65079e18-e319-48a9-9838-e569729a7951)


## Patch instructions

1. grab the latest xdelta patch from the [Releases section](https://github.com/eadmaster/pcrown/releases).
2. get a game image matching [these hashes](http://redump.org/disc/4901/), and extract it
3. download and install `iat` and `xdelta3`
4. convert track 1 to iso with
`iat -i "Princess Crown (Japan) (1M) (Track 01).bin" -o "Princess Crown (Japan) (1M) (Track 01).iso" --iso`
5. apply the xdelta patch with
`xdelta3 -d -s  "Princess Crown (Japan) (1M) (Track 01).iso"  "Princess.Crown.Japan.1M.Track.01.iso.xdelta"  "Princess Crown (Japan) (1M) (Track 01) (English).iso"`
6. use [this cue sheet](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/Princess%20Crown%20(Japan)%20(1M)%20(English).cue) to play with the patched iso.

On Windows you can use [this script](https://github.com/eadmaster/pcrown/releases/download/v0.3/Princess.Crown.ez.Windows.patching.script.zip) to automate the process.


## Contributions
 
Issues, PRs and discussions are currently restricted to contributors to avoid spamming.

Contact me via forums if you can contribute:

 - https://segaxtreme.net/threads/help-me-translate-princess-crown.18555/
 - https://gbatemp.net/threads/princess-crown-psp-any-updates.645443/

Currently i mostly need help with the code part, a [semi-complete translation](https://github.com/eadmaster/pcrown/tree/master/script/eng) was already contributed by SamIAm.
