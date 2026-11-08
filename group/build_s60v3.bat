@echo off
set EPOCROOT=\S60\devices\S60_3rd_FP2_SDK_v1.1\
set GCC_EXEC_PREFIX=
set COMPILER_PATH=
set PATH=C:\PROGRA~2\CSLARM~1\bin;C:\Perl\bin;C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\tools;C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\gcc\bin;C:\PROGRA~2\COMMON~1\Symbian\tools;C:\Windows\system32;C:\Windows
set Path=%PATH%

cd /d "c:\Users\THANG\Documents\Antigravity\gnubox-s60v3-vetera\group"
echo === Running bldmake bldfiles ===
call bldmake bldfiles
if errorlevel 1 exit /b 1

echo === Running abld makefile gcce ===
call abld makefile gcce
if errorlevel 1 exit /b 1

set GCC_EXEC_PREFIX=C:\PROGRA~2\CSLARM~1\lib\gcc\
set COMPILER_PATH=C:\PROGRA~2\CSLARM~1\libexec\gcc\arm-none-symbianelf\3.4.3;C:\PROGRA~2\CSLARM~1\arm-none-symbianelf\bin;C:\PROGRA~2\CSLARM~1\bin
set PATH=C:\PROGRA~2\CSLARM~1\bin;C:\PROGRA~2\CSLARM~1\libexec\gcc\arm-none-symbianelf\3.4.3;C:\PROGRA~2\CSLARM~1\arm-none-symbianelf\bin;%PATH%
set Path=%PATH%

echo === Running abld resource gcce urel ===
call abld resource gcce urel
if errorlevel 1 exit /b 1

echo === Running abld target gcce urel ===
call abld target gcce urel
if errorlevel 1 exit /b 1

echo === Running abld final gcce urel ===
call abld final gcce urel
if errorlevel 1 exit /b 1

if not exist "C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\release\gcce\urel\gnubox.exe" (
    echo ERROR: gnubox.exe was not produced!
    exit /b 1
)

cd /d "c:\Users\THANG\Documents\Antigravity\gnubox-s60v3-vetera\sis"
copy /y "C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\release\gcce\urel\gnubox.exe" "gnubox.exe"
copy /y "C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\data\z\resource\apps\gnubox.rsc" "gnubox.rsc"
copy /y "C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\data\z\private\10003a3f\apps\gnubox_reg.rsc" "gnubox_reg.rsc"

echo === Running makesis ===
"C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\tools\makesis.exe" gnubox_s60v3_vetera.pkg gnubox_s60v3_vetera.sis
if errorlevel 1 exit /b 1

echo === Running signsis ===
"C:\S60\devices\S60_3rd_FP2_SDK_v1.1\epoc32\tools\signsis.exe" -s gnubox_s60v3_vetera.sis gnubox_s60v3_vetera.sisx selfsign.cer selfsign.key
if errorlevel 1 exit /b 1

copy /y gnubox_s60v3_vetera.sis "..\gnubox_z44ar1k5.sis"
copy /y gnubox_s60v3_vetera.sisx "..\gnubox_s60v3_vetera.sisx"

echo === BUILD AND PACKAGING COMPLETE ===
