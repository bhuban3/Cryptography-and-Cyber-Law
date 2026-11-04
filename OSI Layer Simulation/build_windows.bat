@echo off
setlocal
where gcc >nul 2>nul
if errorlevel 1 (
 echo GCC not found. Install MSYS2 MinGW-w64 GCC and add its bin folder to PATH.
 echo Example PATH entry: C:\msys64\mingw64\bin
 pause
 exit /b 1
)
if not exist build mkdir build
echo Building Windows client...
gcc -Wall -Wextra -O2 -std=c11 c_src\client.c c_src\osi_common.c -o build\osi_client.exe -lws2_32
if errorlevel 1 goto failed
echo Building Windows server...
gcc -Wall -Wextra -O2 -std=c11 c_src\server.c c_src\osi_common.c -o build\osi_server.exe -lws2_32
if errorlevel 1 goto failed
echo.
echo Build complete. Start run_windows.bat
pause
exit /b 0
:failed
echo Build failed. Check GCC installation and errors above.
pause
exit /b 1
