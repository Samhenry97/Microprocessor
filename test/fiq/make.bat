@echo off

rem Capture "full" path to target module
set TARGET=%~dpfn1

rem Set CWD to the directory holding this batch file
pushd %~dp0

rem Make sure build directories exist
mkdir obj\sys >nul 2>nul
mkdir obj\usr >nul 2>nul

rem Step 1: build baseline ATOS (toy OS), but only if needed
rem ---------------------------------------------------------------------------------
if not exist obj\libc.a (
	rem Compile the "kernel"
	arm-none-eabi-as.exe -o obj\sys\boot.o sys\boot.s -mcpu=arm7tdmi
	arm-none-eabi-gcc.exe -c -o obj\sys\kernel.o -Iinc\ sys\kernel.c -nostdlib -fno-builtin -nostartfiles -nodefaultlibs -mcpu=arm7tdmi

	rem Compile "libc"
	arm-none-eabi-as.exe -o obj\usr\crt.o lib\crt.s -mcpu=arm7tdmi
	arm-none-eabi-gcc.exe -c -o obj\usr\libc.o -Iinc\ lib\libc.c -nostdlib -fno-builtin -nostartfiles -nodefaultlibs  -mcpu=arm7tdmi

	rem While we're at it, produce an archive of libc
	arm-none-eabi-ar.exe r obj\libc.a obj\usr\crt.o obj\usr\libc.o
	arm-none-eabi-ar.exe s obj\libc.a

	rem And of the "kernel"
	arm-none-eabi-ar.exe r obj\libsys.a obj\sys\boot.o obj\sys\kernel.o
	arm-none-eabi-ar.exe s obj\libsys.a
)

rem Step 2: build whatever .C module was passed as %1, linking in libc and the kernel to produce a single ELF image
rem ---------------------------------------------------------------------------------------------------------------

rem Compile program module (pass our second argument, too, in case they specify a -Dxxxxx define)
arm-none-eabi-gcc.exe -c -o obj\%~n1.o %TARGET% %2 -Iinc\ -nostdlib -fno-builtin -nostartfiles -nodefaultlibs  -mcpu=arm7tdmi

rem Link all modules
arm-none-eabi-ld.exe -Map obj\%~n1.map -o %~n1.exe -T etc\linker.ld -t -N obj\%~n1.o obj\libc.a obj\libsys.a

rem No compatibility manifest required anymore--the simulator must start in the correct state as-is

rem Restore CWD
popd
