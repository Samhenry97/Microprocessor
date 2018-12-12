@echo off

rem Capture "full" path to target module
set TARGET=%~dpfn1

rem Set CWD to the directory holding this batch file
pushd %~dp0

rem Make sure build directory exists
mkdir obj >nul 2>nul

rem Step 1: build baseline ATOS (toy OS; basically just libc at this point) if needed
rem ---------------------------------------------------------------------------------
if not exist obj\libc.a (
	rem Compile/assemble libc sources
	arm-none-eabi-gcc.exe -c -o obj\libc.o -Iinc\ lib\libc.c -nostdlib -fno-builtin -nostartfiles -nodefaultlibs  -mcpu=arm7tdmi
	arm-none-eabi-as.exe -o obj\crt.o lib\crt.s

	rem Produce an archive of libc
	arm-none-eabi-ar.exe r obj\libc.a obj\crt.o obj\libc.o
	arm-none-eabi-ar.exe s obj\libc.a
)

rem Step 2: build whatever .C module was passed as %1, linking to libc.a
rem --------------------------------------------------------------------

rem Compile program module (pass our second argument, too, in case they specify a -Dxxxxx define)
arm-none-eabi-gcc.exe -c -o obj\%~n1.o %TARGET% %2 -Iinc\ -nostdlib -fno-builtin -nostartfiles -nodefaultlibs  -mcpu=arm7tdmi

rem Link all modules
arm-none-eabi-ld.exe -Map obj\%~n1.map -nostdlib -Ttext 0x1000 -N -o %~n1.exe obj\%~n1.o obj\libc.a

rem Add in the compatibility manifest
arm-none-eabi-objcopy --add-section .armsim=etc\compat.txt --set-section-flags .armsim=noload %~n1.exe

rem Restore CWD
popd
