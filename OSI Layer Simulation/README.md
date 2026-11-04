# OSI Zig-Zag Communication Simulator (C + Python)

An educational client/server project inspired by the supplied simulation. It demonstrates **top-down encapsulation**, **real TCP socket communication**, **bottom-up decapsulation**, and a Python console visualization.

## Important accuracy note
The seven-layer headers in this project are **educational application-level envelopes**. They are not real Ethernet, IP, or TCP headers. The OS kernel's TCP/IP stack carries the simulated payload. A normal application cannot inject Layer 1/2 frames through an ordinary TCP socket. Thus the project illustrates OSI concepts while using genuine client/server socket communication.

## Requirements
- Linux/Ubuntu or WSL2 Ubuntu
- GCC, Make, Python 3

Install: `sudo apt update && sudo apt install build-essential python3`

## Build
From the project directory:
```bash
make
```

## Run (one-computer demo)
```bash
python3 python/visualizer.py --message "Hello from OSI!"
```
The Python app shows the zig-zag overview and runs C server/client.

## Run in two terminals (full endpoint logs)
Terminal 1:
```bash
./build/osi_server
```
Terminal 2:
```bash
./build/osi_client 127.0.0.1 "Hello OSI"
```
The server prints receive-side decapsulation progress and delivered text.

Or use Python for the receiver:
```bash
python3 python/visualizer.py --server
```

## Across two machines on the same network
1. Start `./build/osi_server` on the receiver.
2. Allow inbound TCP port **5055** in the receiver firewall, restricted to your private LAN.
3. Find the receiver's LAN IPv4 address.
4. On sender run: `./build/osi_client RECEIVER_IP "Hello over LAN"`.
5. Ensure both computers can reach each other. Avoid exposing port 5055 to the public internet.

## Project layout
```text
c_src/
  client.c          C sender, 7-stage envelope chain + TCP socket
  server.c          C receiver, validates/removes envelopes
  osi_common.c/.h   exact-length TCP send/receive helpers
python/
  visualizer.py     zig-zag flow display and process launcher
Makefile
README.md
```

## Encapsulation design
Application message is wrapped in sequence:
`APP[PRS[SES[TRN[NET[DLK[message]]]]]]`
At the receiver the six wrappers are validated and removed to restore the original message. The stage logs cover all seven conceptual layers. The wire uses a length-prefixed TCP byte stream because TCP itself does not preserve message boundaries.

## Progress output
Sender reports milestones after crossing layers 2, 4 and 6 (2/7, 4/7, 6/7); receiver reports its milestones as it decapsulates.

## Limitations / extensions
- This is a teaching model, not a protocol implementation or packet analyzer.
- Layer 1 is represented symbolically; physical bits/signals are handled below the socket API.
- Session/presentation functions are simplified labels.
- One client request is handled per accepted connection; the server loops for more.
- Consider adding checksums, sequence IDs, JSON event output, GUI animation, and automated tests.
