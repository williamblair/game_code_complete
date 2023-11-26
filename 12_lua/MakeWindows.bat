:: use with Visual Studio Developer command Propmpt for VS 2019
set VSLIBSDIR=D:\vs_libs
::set VSLIBSDIR=C:\VSLibs
set INCDIRS=/I %VSLIBSDIR%\include /I. /I../lib /I../lib/LuaPlus/lua53-luaplus/src
set LIBDIRS=/link /LIBPATH:%VSLIBSDIR%\lib
set LIBS=glew32.lib opengl32.lib SDL2.lib SDL2main.lib SDL2_mixer.lib zlibd.lib assimp-vc142-mtd.lib LinearMath.lib Bullet3Common.lib BulletInverseDynamics.lib Bullet3Geometry.lib BulletCollision.lib BulletDynamics.lib BulletSoftBody.lib Ws2_32.lib
::set LIBS=glew32.lib opengl32.lib SDL2.lib SDL2main.lib SDL2_mixer.lib zlibd.lib assimp-vc143-mtd.lib LinearMath.lib Bullet3Common.lib BulletInverseDynamics.lib Bullet3Geometry.lib BulletCollision.lib BulletDynamics.lib BulletSoftBody.lib Ws2_32.lib
set CFLAGS=/EHsc /O2
::set CFLAGS=/EHsc /O2 /DEBUG /Zi /MTd
set SOURCES="*.cpp" "..\lib\GCC4\*.cpp" "..\lib\LuaPlus\*.c*" "..\lib\LuaPlus\lua53-luaplus\src\*.c"

cl %CFLAGS% %SOURCES% /Femain.exe %INCDIRS% %LIBDIRS% %LIBS%
