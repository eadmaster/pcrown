
# PRE: sudo apt-get install gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 wine64

rm ../buildcd/eventeditor.exe
rm ../buildcd/eventeditor_debug.exe

#x86_64-w64-mingw32-g++ -w -fpermissive *.c *.cpp -lcomdlg32  -lmingw32 -o ../buildcd/eventeditor.exe

x86_64-w64-mingw32-g++ -DDEBUG_MODE -w -fpermissive *.c *.cpp -lcomdlg32  -lmingw32 -o ../buildcd/eventeditor_debug.exe
 
