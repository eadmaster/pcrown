
# Princess Crown Translation (Unofficial) Redux

[old readme here](readme.txt.old)

My attempt to continue on the CyberWarriorX translation project.

**P.N.: reportedly CyberWarriorX and SamIAm are still working privately on this project, and since this was forked many years ago it does not reflect the current status of their project.
Btw since this was originally released with a GPL-2.0 license, i'd like to exercise my right to keep working on this and make improvements as long as a better translation patch is released.**


## Current status

 - all story events and names translated using a fixed-width font
   - [use a bigger monospaced font](https://github.com/eadmaster/pcrown/issues/3)
 - [missing items translations](https://github.com/eadmaster/pcrown/issues/2)
 - [missing translation for location signs](https://github.com/eadmaster/pcrown/issues/5)
 - needs play testing for bugs

## Preview

![welcome](https://github.com/user-attachments/assets/c86585d6-106a-409b-951b-5bbd072533f6)  ![npc](https://github.com/user-attachments/assets/65079e18-e319-48a9-9838-e569729a7951)


## Manual Patch instructions

1. get a game image matching [these hashes](http://redump.org/disc/4901/), and extract it
2. grab the latest xdelta patch from the [Releases section](https://github.com/eadmaster/pcrown/releases).
3. download and install `iat` and `xdelta3`
4. convert track 1 to iso with
`iat -i "Princess Crown (Japan) (1M) (Track 01).bin" -o "Princess Crown (Japan) (1M) (Track 01).iso" --iso`
5. apply the xdelta patch with
`xdelta3 -d -s  "Princess Crown (Japan) (1M) (Track 01).iso"  "Princess.Crown.Japan.1M.Track.01.iso.xdelta"  "Princess Crown (Japan) (1M) (Track 01) (English).iso"`
6. use [this cue sheet](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/Princess%20Crown%20(Japan)%20(1M)%20(English).cue) to play with the patched iso.


# Semi-automated Patch instructions

1. get a game image matching [these hashes](http://redump.org/disc/4901/), and extract it
2. download and execute one of these easy patching scripts in the extraction dir:
 
  - [for Windows](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/_patch_eng.bat)
  - [for Linux](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/_patch_eng.sh)


## Contributions
 
Issues and PRs are currently restricted to contributors to avoid spamming, trolling, etc.

Contact me via discord or forums if you can contribute:

 - https://discord.com/channels/348545631985467394/797608417857372161
 - https://segaxtreme.net/members/eadmaster.30323/
 - https://gbatemp.net/members/eadmaster.52646/


# New contributors

 - [paul-met](https://github.com/paul-met) and derek ([short spaces and dialog fixes](https://github.com/eadmaster/pcrown/issues/1))
 - Spike and MLagaffe (bug reports)
 - [MiYakuGaming](https://github.com/MiYakuGaming/) ([missing translation parts](https://github.com/eadmaster/pcrown/issues/4))
 
(sorry if i missed somebody, just contact me and you will be added in the list)
