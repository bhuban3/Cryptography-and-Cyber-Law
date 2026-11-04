#!/usr/bin/env python3
"""Windows-friendly console flow guide and C socket demo launcher."""
import argparse, subprocess, sys, time
from pathlib import Path
ROOT=Path(__file__).resolve().parents[1]
CLIENT=ROOT/"build"/"osi_client.exe"
SERVER=ROOT/"build"/"osi_server.exe"
LAYERS=["7 Application","6 Presentation","5 Session","4 Transport","3 Network","2 Data Link","1 Physical"]
def main():
 p=argparse.ArgumentParser(description="OSI zig-zag client/server demo")
 p.add_argument("--host",default="127.0.0.1")
 p.add_argument("--message")
 p.add_argument("--server",action="store_true")
 args=p.parse_args()
 if not CLIENT.exists() or not SERVER.exists():
  print("Build first by double-clicking build_windows.bat",file=sys.stderr);return 2
 if args.server:
  print("Receiver waiting at TCP port 5055. Press Ctrl+C to stop.")
  return subprocess.call([str(SERVER)],cwd=ROOT)
 msg=args.message if args.message is not None else input("Message to send: ")
 if not msg.strip(): print("Message cannot be empty.");return 2
 receiver=subprocess.Popen([str(SERVER)],cwd=ROOT,stdout=None,stderr=None)
 try:
  time.sleep(.4)
  print("\n=== ZIG-ZAG DATA FLOW ===")
  print("SENDER (encapsulation)                 RECEIVER (decapsulation)")
  for i in range(7):
   print(f"  ↓ L{7-i if False else i+1} {LAYERS[i]:<26} ↑ {LAYERS[6-i]}")
   if i in (1,3,5): print(f"       [milestone: {i+2}/7 sender stages crossed]")
  print("\n=== C CLIENT OUTPUT ===")
  r=subprocess.run([str(CLIENT),args.host,msg],cwd=ROOT,text=True)
  print("\nReceiver window shows decapsulation and delivered message.")
  return r.returncode
 except KeyboardInterrupt:
  return 130
 finally:
  receiver.terminate()
  try: receiver.wait(timeout=2)
  except subprocess.TimeoutExpired: receiver.kill()
if __name__=="__main__": raise SystemExit(main())
