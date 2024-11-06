
# Princess Crown Translation GPL edition

[old readme here](readme.txt.old)

My attempt to continue on the CyberWarriorX translation project.

**P.N.: reportedly CyberWarriorX and SamIAm are still working privately on this project, and since this was forked many years ago it does not reflect the current status of their project.
Btw since this was originally released with a GPL-2.0 license, i'd like to exercise my right to keep working on this and make improvements as long as a better translation patch is released.**


## Current status

 - all story events and names translated using a fixed-width font, with bugs in some dialogs
 - [missing items translations](https://github.com/eadmaster/pcrown/issues/2) (coming in v0.5!)
 - [missing translation for location signs](https://github.com/eadmaster/pcrown/issues/5)
 - needs play testing for bugs


## [FAQs](https://github.com/eadmaster/pcrown/wiki/FAQs)


## Preview

![demo1](https://github.com/user-attachments/assets/b4116a9b-2410-474c-8ad0-a64bc6a2266b)  ![demo2](https://github.com/user-attachments/assets/1b0aebdf-efc6-4c21-9c32-ca81ddf03acd)


## Semi-automated patch instructions

1. obtain a game image matching [these hashes](http://redump.org/disc/4901/), and extract it
2. download and execute one of these easy patching scripts in the extraction dir:
 
  - [for Windows](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/_patch_eng.bat)
  - [for Linux](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/_patch_eng.sh)


## Manual patch instructions

1. obtain a game image matching [these hashes](http://redump.org/disc/4901/), and extract it
2. grab the latest xdelta patch from the [Releases section](https://github.com/eadmaster/pcrown/releases).
3. download and install `iat` and `xdelta3`
4. convert track 1 to iso with
`iat -i "Princess Crown (Japan) (1M) (Track 01).bin" -o "Princess Crown (Japan) (1M) (Track 01).iso" --iso`
5. apply the xdelta patch with
`xdelta3 -d -s  "Princess Crown (Japan) (1M) (Track 01).iso"  "Princess.Crown.Japan.1M.Track.01.iso.xdelta"  "Princess Crown (Japan) (1M) (Track 01) (English).iso"`
6. use [this cue sheet](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/Princess%20Crown%20(Japan)%20(1M)%20(English).cue) to play with the patched iso.



## Contributions
 
Issues and PRs are currently restricted to contributors to avoid spamming, trolling, etc.

Contact me via discord or forums if you can contribute or have issues to report:

 - https://discord.com/channels/348545631985467394/797608417857372161
 - https://segaxtreme.net/members/eadmaster.30323/
 - https://gbatemp.net/members/eadmaster.52646/

**P.N.: any bit contributed will be re-shared in this repo under the terms of GPL-2.0.**


# New contributors/Special thanks

 - [paul-met](https://github.com/paul-met) and [derek](https://github.com/DerekPascarella) ([short spaces and dialog fixes](https://github.com/eadmaster/pcrown/issues/1))
 - Spike and MLagaffe (bug reports)
 - [MiYakuGaming](https://github.com/MiYakuGaming/) ([missing translation parts](https://github.com/eadmaster/pcrown/issues/4))

