Princess Crown Translation
--------------------------

How to Patch
------------
1. Make sure you have the original Princess Crown release(not satakore) and rip the disc to bin/cue using your favorite utility(if unsure how to do, check Google).
2. Go to command prompt and into where you have the disc image and patch files, and type "xdelta -d -s your_disc_image.bin princess_crown.xdelta princess_crown_english.bin"
3. Open the disc image cue file in a text editor and make sure the line that contains "your_disc_image.bin" is changed to princess_crown_english.bin.
4. Now you're ready to run it in your favorite emulator(Yabause ^_-) or burn on a disc and run it on your modified(or soft-modded using PseudoSaturn ^_-) Saturn.

Build Dependencies
------------------
-xdelta
-isotools(included in source)
-vcdbuild RE-dux(included in source)
-Princess Crown disk image in bin/cue format
-Saturn code was compiled using the kpit elf compiler. You will need to compile some kind of SH2 cross compiler beforehand. Otherwise everything else should be detected using cmake.

Build Instructions
------------------
1. Start up cmake-gui.
2. Set "Where is the source code" to where you downloaded source code to.
3. Set "Where to build the binaries" to another directory, ideally not the same directory as source code. Make a note of directory.
4. Click on "Configure" and answer yes to create directory if it doesn't exist. 
5. Choose which compiler you're using. Click "Next".
6. If not detected, specify locations of xdelta, isotools, vcdbuild. Also change language if necessary. Press Configure again.
7. If there's any errors, correct them. Otherwise click "Generate" button.
8. Close cmake.
9. If everything is successful you should get a message "[100%] Built target pcrown". The final patch is princess_crown.xdelta and the final disk image is binary is called "PseudoSaturn.BIN" is is located in the binaries directory.

Special Thanks
--------------
Thanks to my buds on #yabause, #mednafen, rhdn, and assemblergames
-Amon
-BlueCrab
-Charles MacDonald
-esperknight
-Guill
-jhl
-mrkotfw
-pinchy
-SamIAm
-SaturnAR
-tehcloud
-WhiteSnake

Other Acknowledgements
----------------------
-vbt for his code that became helpful for understanding and rebuilding the main code. Originally I was going to include a special thanks, however since then he's been an absolute douchebag. So he's getting no thanks, just this.
