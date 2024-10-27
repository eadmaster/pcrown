
# PRE: sudo apt-get install gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 wine64

rm ../buildcd/itemsutil.exe

x86_64-w64-mingw32-g++ -fpermissive main.cpp  ../eventeditor/*.c   ../eventeditor/evn.cpp   ../eventeditor/texttbl.cpp  ../eventeditor/trnstext.cpp -lcomdlg32  -lmingw32 -o ../buildcd/itemsutil.exe

 
