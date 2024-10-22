
# Princess Crown Translation Redux

[old readme here](readme.txt.old)

My attempt to restart the CyberWarriorX translation project.

## Current status

 - story events patching works using a fixed-width font
   - word wrapping still needs reworking
 - items and names translation TBA soon
 - needs testing for crashes

## Preview

![shot1](https://github.com/user-attachments/assets/517c4f76-3926-4831-9924-0dd5ac1e6487)

## Patch instructions

1. grab the latest xdelta patch from the [Releases section](https://github.com/eadmaster/pcrown/releases).
2. get the [redump.org game image](http://redump.org/disc/4901/), and extract it
3. download and install [iat](http://sourceforge.net/projects/iat.berlios/) and xdelta3
4. convert track 1 to iso with
`iat "Princess Crown (Japan) (1M) (Track 01).bin" "Princess Crown (Japan) (1M) (Track 01) (jap).iso"`  (you may use another iso conversion tool)
5. apply xdelta patch with
`xdelta3 -d -s  "Princess Crown (Japan) (1M) (Track 01) (jap).iso"  "Princess.Crown.Japan.1M.Track.01.iso.xdelta"  "Princess Crown (Japan) (1M) (Track 01).iso"`
6. use [this cue sheet](https://github.com/eadmaster/pcrown/blob/master/src/buildcd/Princess%20Crown%20(Japan)%20(1M).cue) to play with the patched iso.


## Contributions
 
Issues, PRs and discussions are currently restricted to contributors to avoid spamming.

Contact me via forums if you can contribute:

 - https://segaxtreme.net/threads/help-me-translate-princess-crown.18555/
 - https://gbatemp.net/threads/princess-crown-psp-any-updates.645443/

Currently i mostly need help with the code part, a [semi-complete translation](https://github.com/eadmaster/pcrown/tree/master/script/eng) was already contributed by SamIAm.
