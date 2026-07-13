echo off
set arg=%1
if "%arg%"=="p" (goto path)
if "%arg%"=="r" (goto run)
goto clean

:path
path = %path%;c:\sdcc_old\bin;c:\retrodev\sms\psgtool

:clean
call clean.bat
sdcc -c -mz80 main.c
sdcc -c -mz80 bank2.c
sdcc -c -mz80 vdp.c
sdcc -o main.ihx -mz80 --no-std-crt0 --data-loc 0xC000 crt0\crt0_sms.rel main.rel vdp.rel bank2.rel SMSlib.lib PSGLib\PSGLib.lib
makesms main.ihx gamelife.sms

:run
c:\retrodev\sms\emulicious\emulicious.exe gamelife.sms