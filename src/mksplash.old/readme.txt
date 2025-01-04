mksplash. By cafe-alpha, http://ppcenter.free.fr
For Princess Crown English translation.

Uses LZF compression algorithm : http://oldhome.schmorp.de/marc/liblzf.html

Description : it gathers the following files in a single Saturn binary :
 - mksplash\splash_loader.bin : the program that decompress/display/execute data
 - splash.bmp : image displayed on startup. Size must be 320 x 224.
 - 0.bin : binary file that will be loaded from address 0x0600400 after splash screen is displayed.

About keys when splash image is displayed :
 - A/C/Start : Execute 0.bin
 - B : Wait for data from USB dev cart. Don't do anything if USB dev cart is not detected.

Usage examples:
 mksplash.exe splash_loader.bin splash.bmp 0_org.BIN cd\0.bin
