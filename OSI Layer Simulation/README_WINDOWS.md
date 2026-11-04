# Native Windows (no WSL) setup

## 1. Install a native GCC toolchain
1. Install MSYS2 from https://www.msys2.org/
2. Open **MSYS2 UCRT64** from Start.
3. Run: `pacman -Syu` (follow any close/reopen instructions), then install: `pacman -S --needed mingw-w64-ucrt-x86_64-gcc`
4. Add `C:\msys64\ucrt64\bin` to Windows PATH (search **Edit environment variables for your account**).
5. Close and reopen Command Prompt, then run `gcc --version`.

## 2. Build
Extract this project. Double-click `build_windows.bat`, or open CMD in the project folder and run:
`build_windows.bat`

The C programs use Windows Winsock (`ws2_32`) and are compiled as native `.exe` files.

## 3. Run
Double-click `run_windows.bat`, or in CMD:
`python python\\visualizer.py --message "Hello Windows"`

Python 3 must be installed and available as `python`.

## 4. Run endpoints separately
Terminal A: `build\\osi_server.exe`
Terminal B: `build\\osi_client.exe 127.0.0.1 "Hello OSI"`

## Notes
- Allow Python/GCC-created app access only on Private networks if Windows Firewall prompts; local loopback typically does not require a rule.
- For a LAN demo, use the receiver PC's LAN IPv4 address as the client host and configure a private-network firewall inbound rule for TCP port 5055.
- The layer headers remain an educational simulation. Winsock/TCP and the Windows networking stack carry the actual traffic.
