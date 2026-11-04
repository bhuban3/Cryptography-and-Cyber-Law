@echo off
if not exist build\osi_client.exe (
 echo First run build_windows.bat
 pause
 exit /b 1
)
echo Starting OSI demo...
python python\visualizer.py --message "Hello from native Windows"
pause
