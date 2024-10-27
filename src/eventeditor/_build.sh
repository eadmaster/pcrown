
# PRE: sudo apt-get install gcc-mingw-w64-x86-64 g++-mingw-w64-x86-64 wine64

rm ../buildcd/eventeditor.exe

x86_64-w64-mingw32-g++ -fpermissive *.c *.cpp -lcomdlg32  -lmingw32 -o ../buildcd/eventeditor.exe
 
