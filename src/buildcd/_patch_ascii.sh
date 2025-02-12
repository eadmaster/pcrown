#!/bin/bash

# invoked by _patch_items.sh, shared by all builds
 
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
sfk setbytes 0.BIN 0xACE91  "AND"  -yes
sfk setbytes 0.BIN 0xACE9A  "OPEN"  -yes
sfk setbytes 0.BIN 0xACEA5  0x04  -yes
sfk setbytes 0.BIN 0xACEA6  "DEMON REALM'S GATE"  -yes
# "BUT THIS CAUSED THE GATE TO UNDERWORLD"->"THROUGH THE DEMON REALM'S GATE"
#sfk setbytes 0.BIN 0xACE91  "THROUGH THE " 0x04 "DEMON REALM'S GATE" 0x00  -yes
# "GRADRIEL WENT TO FACE THE GREATERDEMONS" -> "GRADRIEL WENT TO FACE THE DEMONIC FORCES"  (1-byte overflow)
sfk setbytes 0.BIN 0xACED8  "DEMONIC FORCES"  -yes
# "GOLGOTHA <02>THE EXGENERAL,ARRIVED" -> "GOLGOTHA <02>THE EX-GENERAL ARRIVED" (1-byte overflow)
sfk setbytes 0.BIN 0xACEF9  0x2D "GENERAL ARRIVED"  -yes

# "FALLDOWN FROM THE BOOKWORLD" (gameover screen) -> "FALLEN FROM THE BOOKWORLD"
sfk setbytes 0.BIN 0xACC32 0x20032003460341034C034C0345034E   -yes

# "GET EXP" (end battle screen) -> "  EXP  "
sfk setbytes 0.BIN 0xA6B4C  "  EXP  "  -yes
# "TREASURE MENU" (end battle screen) -> " ITEMS FOUND "
sfk setbytes 0.BIN 0xA6B60  " ITEMS FOUND "  -yes

# fix Engrish in the save manager https://github.com/eadmaster/pcrown/issues/94
# "The SAVE-FILE has crashed." -> "The SaveFile is corrupted"
sfk setbytes 0.BIN 0xA4099  "FILE is corrupt"  -yes
# "BACK-UP RAM is lacking." -> "BACK-UP RAM is full.   "
sfk setbytes 0.BIN 0xA40E0  "full.   "  -yes
