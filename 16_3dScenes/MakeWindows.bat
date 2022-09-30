:: use with Visual Studio Developer command Propmpt for VS 2019
set VSLIBSDIR=D:\vs_libs
set INCDIRS=/I %VSLIBSDIR%\include /I. /I../lib
set LIBDIRS=/link /LIBPATH:%VSLIBSDIR%\lib
set LIBS=zlibd.lib
set CFLAGS=/EHsc /O2 /std:c++17
set SOURCES="*.cpp" "..\lib\*.cpp"

cl %CFLAGS% %SOURCES% /Femain.exe %INCDIRS% %LIBDIRS% %LIBS%
