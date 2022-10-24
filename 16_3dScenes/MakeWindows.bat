:: use with Visual Studio Developer command Propmpt for VS 2019
set VSLIBSDIR=D:\vs_libs
set INCDIRS=/I %VSLIBSDIR%\include /I. /I../lib
set LIBDIRS=/link /LIBPATH:%VSLIBSDIR%\lib
set LIBS=glew32.lib opengl32.lib SDL2.lib SDL2main.lib SDL2_mixer.lib zlibd.lib
::set CFLAGS=/EHsc /O2 /std:c++11
set CFLAGS=/EHsc /O2
set SOURCES="*.cpp" "..\lib\*.cpp"

cl %CFLAGS% %SOURCES% /Femain.exe %INCDIRS% %LIBDIRS% %LIBS%
