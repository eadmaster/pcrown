#!/bin/bash

# invoked by _patch_items.sh, shared by all builds

# fix Engrish in the save manager https://github.com/eadmaster/pcrown/issues/94
# "The SAVE-FILE has crashed." -> "The SaveFile is corrupted"
sfk setbytes 0.BIN 0xA4099  "FILE is corrupt"  -yes
# "BACK-UP RAM is lacking." -> "BACK-UP RAM is full.   "
sfk setbytes 0.BIN 0xA40E0  "full.   "  -yes

# fix Engrish text in main program (opening, etc) https://github.com/eadmaster/pcrown/issues/94
# "A LONG LONG AGO..." -> "ONCE UPON A TIME.."
sfk setbytes 0.BIN 0xACC85  "ONCE UPON A TIME.."  -yes
# "I WANT TO BECOME A .BRAVE QUEEN" -> "I WANT TO BE A .BRAVE QUEEN"
sfk setbytes 0.BIN 0xACCD9 " A " 0x04 "BRAVE QUEEN" 0x00  -yes
# (IN ORDER TO STOP THE) "DEMONS"->"DRAGONS"
sfk setbytes 0.BIN 0xACD07  "DRAGONS"  -yes
# "PROSERPINA RUN A WAY AT TOP SPEED" -> "PROSERPINA RAN AWAY AT FULL SPEED"
sfk setbytes 0.BIN 0xACDFD  "RAN AWAY AT FULL"  -yes
# "THESE PEOPLE ARE USING MY NAME" -> "SOMEBODY IS USING MY NAME     "
sfk setbytes 0.BIN 0xACE15  "SOMEBODY IS USING MY NAME     " -yes
# "PARSONS HAPPENED TO BE REAL PORTGUS" -> "I HAPPEN TO BE THE REAL PORTGUS    "
sfk setbytes 0.BIN 0xACE35  "I HAPPEN TO BE THE REAL " 0x04 "PORTGUS    "  -yes
# "BUT THIS CAUSED THE GATE TO UNDERWORLD" -> "AND THIS OPENED THE DEMON REALM'S GATE"
#sfk setbytes 0.BIN 0xACE91  "AND"  -yes
#sfk setbytes 0.BIN 0xACE9A  "OPEN"  -yes
#sfk setbytes 0.BIN 0xACEA5  0x04  -yes
#sfk setbytes 0.BIN 0xACEA6  "DEMON REALM'S GATE"  -yes
# "BUT THIS CAUSED THE GATE TO UNDERWORLD"->"THROUGH THE DEMON REALM'S GATE"
sfk setbytes 0.BIN 0xACE91  "THROUGH THE " 0x04 "DEMON REALM'S GATE" 0x00  -yes
# "GRADRIEL WENT TO FACE THE GREATERDEMONS" -> "GRADRIEL WENT TO FACE THE DEMONIC FORCES"  (1-byte overflow)
sfk setbytes 0.BIN 0xACED8  "DEMONIC FORCES"  -yes
# "GOLGOTHA <02>THE EXGENERAL,ARRIVED" -> "GOLGOTHA <02>THE EX-GENERAL ARRIVED" (1-byte overflow)
sfk setbytes 0.BIN 0xACEF9  0x2D "GENERAL ARRIVED"  -yes

# "ENGAGED WITH ENEMY" (pre-battle screen) -> "ENEMY ENGAGED"
# "ENCOUNTER WITH MONSTER" (pre-battle screen) -> "MONSTER ENCOUNTERED"
# TODO: fix "D" letter invisible glitch
# 'ENCOUNTER'->'ENCOUNTERED'    height & string length change 0a->0d
#sfk setbytes 0.BIN 0xA6B19 "ED"     -yes
#sfk setbytes 0.BIN 0x5DF91 0x0D     -yes
# 'ENGAGE'->'ENGAGED',  string length 07->09, height change 0a->0d
#sfk setbytes 0.BIN 0xA6B32 "D"     -yes
#sfk setbytes 0.BIN 0x5DF89 0x09     -yes
#sfk setbytes 0.BIN 0x5DF79 0x0D     -yes
# 'ENEMY' height change    10->0a,  string length 06->07
#sfk setbytes 0.BIN 0x5DFE9 0x0A     -yes
#sfk setbytes 0.BIN 0x5E009 0x07     -yes
# 'MONSTER' height change 10->0a,    string length 08->09
#sfk setbytes 0.BIN 0x5DFF1 0x0A     -yes
#sfk setbytes 0.BIN 0x5DFFD 0x09     -yes
# 'WITH' displayed offscreen    height 0d->00
#sfk setbytes 0.BIN 0x5DFB1 0x00     -yes
# change battle message titlecard scrolling behavior to ensure words are centered    ff->01
#sfk setbytes 0.BIN 0x5DFA7 0x01     -yes

# "GET EXP" (end battle screen) -> "NEW EXP"
sfk setbytes 0.BIN 0xA6B4C  "NEW EXP"  -yes
# "TREASURE MENU" (end battle screen) -> " ITEMS FOUND "
sfk setbytes 0.BIN 0xA6B60  "ITEMS DROPPED"  -yes

# "FALLDOWN FROM THE BOOKWORLD" (gameover screen) -> "FALLEN FROM THE BOOKWORLD"
sfk setbytes 0.BIN 0xACC32 0x20032003460341034C034C0345034E   -yes
