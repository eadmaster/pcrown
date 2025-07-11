
# Princess Crown Translation GPL Edition


## Current status  🏗️

 - All story events, items and names translated
   - Using a fixed-width/monospaced font, with condensed spaces (not VWF)
 - All doorway signs translated and resized properly: [#102](https://github.com/eadmaster/pcrown/issues/102)
 - All book/scenarios fully tested and crash-free
   - Some events commands are still not parsed correctly: [#91](https://github.com/eadmaster/pcrown/issues/91)


## [FAQs](https://github.com/eadmaster/pcrown/wiki/FAQs)  ❓


## [Extras](https://github.com/eadmaster/pcrown/wiki/Extras)  🎁


## Preview  👀

![demo1](https://raw.githubusercontent.com/eadmaster/pcrown/refs/heads/master/shots/demo1.png)  ![demo2](https://raw.githubusercontent.com/eadmaster/pcrown/refs/heads/master/shots/demo2.png)  ![demo3](https://raw.githubusercontent.com/eadmaster/pcrown/refs/heads/master/shots/demo3.png)


## Patch instructions  🩹

1. Obtain a game image matching [these hashes](http://redump.org/disc/4901/), and extract it
2. Grab the latest xdelta patch from the [Releases section](https://github.com/eadmaster/pcrown/releases/latest/download/Princess.Crown.Japan.1M.Track.01.bin.xdelta).
3. Visit [Rom Patcher JS](https://www.marcrobledo.com/RomPatcher.js/)
4. Select `Princess Crown (Japan) (1M) (Track 01).bin` as ROM file
5. Select `Princess.Crown.Japan.1M.Track.01.bin.xdelta` as Patch file
6. Click "Apply patch" and save in the same folder without changing the filename: `Princess Crown (Japan) (1M) (Track 01) (patched).bin`
7. Download and use [this cue sheet](https://raw.githubusercontent.com/eadmaster/pcrown/refs/heads/master/src/buildcd/Princess%20Crown%20(Japan)%20(1M)%20(English).cue) to play the game.

> [!TIP]
> Problems with Rom Patcher JS? Try these alternatives:
>  - [Manual offline patcher](https://github.com/marco-calautti/DeltaPatcher)
>  - [Semi-automatic offline patcher](https://github.com/eadmaster/pcrown/releases/latest/download/Princess.Crown.English.Offline.Patcher.zip)
>  - [Semi-automatic online patcher](https://eadmaster.github.io/pcrown_patcher/)

## Issues/Contributions  🐛
 
Issues and PRs are currently restricted to approved contributors to keep things easy to manage.

Contact me via Discord or forums if you can help or have issues to report (please read the [FAQs](https://github.com/eadmaster/pcrown/wiki/FAQs) first):

 - [Discord](https://discord.gg/CPCpEwyhjf)
 - [SegaXtreme](https://segaxtreme.net/members/eadmaster.30323/)
 - [GBAtemp](https://gbatemp.net/members/eadmaster.52646/)

> [!NOTE]
> Any bit contributed will be shared in this repository under the terms of [GPL-2.0](https://github.com/eadmaster/pcrown/blob/master/LICENSE).


# Credits/Contributors/Special thanks  🤝

In chronological order:

 - [SamIAm](https://www.romhacking.net/forum/index.php?action=profile;u=10553) (original script translation draft)
 - [CyberWarriorX](https://github.com/cyberwarriorx) (EVN file format and text compression reverse engineering, eventeditor and itemsutil patching tools)
 - [eadmaster](https://github.com/eadmaster) (ASCII support and debug mode for eventeditor and itemsutil, dialogs text splitter, doorway signs and other patching scripts, translation fixes, project maintainer)
 - [paul-met](https://github.com/paul-met) ([text spacing and dialog fixes](https://github.com/eadmaster/pcrown/issues/1), [main font](https://github.com/eadmaster/pcrown/issues/98), [gfx enhancements in the EX build](https://github.com/eadmaster/pcrown/issues/18))
 - [MiYakuGaming](https://github.com/MiYakuGaming/) ([missing translation parts and corrections](https://github.com/eadmaster/pcrown/issues/4))
 - [MADevil](https://www.twitch.tv/madevil/), [RetroBorgo](https://www.twitch.tv/retroborgo/), [Stacatto](https://www.youtube.com/c/StilesReviews) (playtesting and bug reports)
 - [Malenko](https://segaxtreme.net/members/malenko.22808/) (gfx hacking help)
 - [DerekPascarella](https://github.com/DerekPascarella) ([Saturn-4bpp-Graphics-Converter script](https://github.com/DerekPascarella/Saturn-4bpp-Graphics-Converter/))
 - [Mentil](https://github.com/mentill) (lot of Engrish and translation fixes, gfx hacking, [PAK file format](https://github.com/eadmaster/pcrown/wiki/PAKFormat) reverse engineering, [eventeditor fixes](https://github.com/eadmaster/pcrown/issues/91))

> [!NOTE]
> CyberWarriorX and SamIAm are still working privately on this project, and since this was forked many years ago it does not reflect the [current status of their project](https://www.romhacking.net/forum/index.php?topic=39984.0).
> Btw since this was originally [released with a GPL-2.0 license](https://github.com/eadmaster/pcrown/blob/master/LICENSE), i'd like to exercise my right to keep working on this and make improvements.

[old readme here](readme.txt.old)
