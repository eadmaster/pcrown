
# Princess Crown Translation GPL edition

[old readme here](readme.txt.old)

My attempt to continue on the CyberWarriorX translation project.

**P.N.: reportedly CyberWarriorX and SamIAm are still working privately on this project, and since this was forked many years ago it does not reflect the current status of their project. Btw since this was originally released with a GPL-2.0 license, i'd like to exercise my right to keep working on this and make improvements as long as a better translation patch is released.**


## Current status

 - all story events, items and names translated
   - using a fixed-width/monospaced font, with condensed spaces (not VWF)
 - [missing translation for some doorway signs](https://github.com/eadmaster/pcrown/issues/5)
 - needs playtesting for bugs
   - crash-free in the 1st book/scenario, 2 cutscenes have reduced dialogs to bypass softlocks
   - other books/scenarios still to be tested


## [FAQs](https://github.com/eadmaster/pcrown/wiki/FAQs)


## [Extras](https://github.com/eadmaster/pcrown/wiki/Extras)


## Preview

![demo1](https://github.com/user-attachments/assets/b4116a9b-2410-474c-8ad0-a64bc6a2266b)  ![demo2](https://github.com/user-attachments/assets/1b0aebdf-efc6-4c21-9c32-ca81ddf03acd)  ![selling](https://github.com/user-attachments/assets/8203505d-3850-41bd-a230-d2003f2551c7)


## Patch instructions (bin)

1. obtain a game image matching [these hashes](http://redump.org/disc/4901/), and extract it
2. grab the latest xdelta patch from the [Releases section](https://github.com/eadmaster/pcrown/releases/latest/download/Princess.Crown.Japan.1M.Track.01.bin.xdelta).
3. visit [Rom Patcher JS](https://www.marcrobledo.com/RomPatcher.js/)
4. select `Princess Crown (Japan) (1M) (Track 01).bin` as ROM file
5. select `Princess.Crown.Japan.1M.Track.01.bin.xdelta` as Patch file
6. click "Apply patch" and save in the same folder without changing its name: `Princess Crown (Japan) (1M) (Track 01) (patched).bin`
7. download and use [this cue sheet](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/Princess%20Crown%20(Japan)%20(1M)%20(English).cue) to play the game.

## Patch instructions (iso)

1. obtain a game image matching [these hashes](http://redump.org/disc/4901/), and extract it
2. download and execute one of these easy patching scripts in the same folder:
 
  - [for Windows](https://github.com/eadmaster/pcrown/tree/2084d8b6d6c87901ce78257f89f244f5d7c6d3f8/src/buildcd/_patch_eng.bat)
  - [for Linux](https://github.com/eadmaster/pcrown/tree/2084d8b6d6c87901ce78257f89f244f5d7c6d3f8/src/buildcd/_patch_eng.sh)
3. use the cue sheet inside [this zip file](https://github.com/eadmaster/pcrown/releases/download/v0.3/Princess.Crown.ez.Windows.patching.script.zip) to play the game.


## Contributions
 
Issues and PRs are currently restricted to contributors to avoid spamming, trolling, etc.

Contact me via discord or forums if you can contribute or have issues to report (please read the [FAQs](https://github.com/eadmaster/pcrown/wiki/FAQs) first):

 - [Discord](https://discord.com/channels/348545631985467394/797608417857372161)
 - [SegaXtreme](https://segaxtreme.net/members/eadmaster.30323/)
 - [GBAtemp](https://gbatemp.net/members/eadmaster.52646/)

**P.N.: any bit contributed will be re-shared in this repo under the terms of GPL-2.0.**


# New contributors/Special thanks

 - [paul-met](https://github.com/paul-met) and [derek](https://github.com/DerekPascarella) ([short spaces and dialog fixes](https://github.com/eadmaster/pcrown/issues/1))
 - Spike, MLagaffe, [MADevil](https://www.twitch.tv/madevil/) (playtesting and bug reports)
 - [MiYakuGaming](https://github.com/MiYakuGaming/) ([missing translation parts and corrections](https://github.com/eadmaster/pcrown/issues/4))
