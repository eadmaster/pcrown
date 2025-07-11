
# PRE: sudo apt-get install gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 wine64

#rm ../buildcd/itemsutil.exe
#rm ../buildcd/itemsutil_debug.exe

x86_64-w64-mingw32-g++ -w -fpermissive main.cpp  ../eventeditor/*.c   ../eventeditor/evn.cpp   ../eventeditor/texttbl.cpp  ../eventeditor/trnstext.cpp -lcomdlg32  -lmingw32 -o ../buildcd/itemsutil.exe
x86_64-w64-mingw32-g++ -DDEBUG_MODE -w  -fpermissive main.cpp  ../eventeditor/*.c   ../eventeditor/evn.cpp   ../eventeditor/texttbl.cpp  ../eventeditor/trnstext.cpp -lcomdlg32  -lmingw32 -o ../buildcd/itemsutil_debug.exe
x86_64-w64-mingw32-g++ -DSATAKORE -w  -fpermissive main.cpp  ../eventeditor/*.c   ../eventeditor/evn.cpp   ../eventeditor/texttbl.cpp  ../eventeditor/trnstext.cpp -lcomdlg32  -lmingw32 -o ../buildcd_reva/itemsutil.exe

 
