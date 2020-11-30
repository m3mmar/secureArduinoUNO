#!/usr/bin/env python3
import sys, os, binascii
import hmac, hashlib, time
sys.path += [ os.path.join(os.path.split(__file__)[0], 'libs') ]
import serial
from intelhex import IntelHex

# 160 bit key
key = b'\x6e\x26\x88\x6e\x4e\x07\x07\xe1\xb3\x0f\x24\x16\x0e\x99\xb9\x12\xe4\x61\xc4\x24'

def main(argv):
   if len(argv) != 2:
      print('challenger.py <hexfile> <serialport>')
      sys.exit(2)

   # Check if hexfile exists
   hexfile = argv[0]
   if not os.path.isfile(hexfile):
      print("ERROR: File not found:", hexfile)
      sys.exit(2)
   ih = IntelHex(hexfile)

   ser = serial.Serial(argv[1], 9600)
   time.sleep(1)

   # Generate nonce
   nonce = os.urandom(20)
   print("Nonce: ")
   print(binascii.hexlify(nonce))

   # Send nonce to prover
   ser.write(nonce)

   # Wait for answer 'o' --> OK -- This is only for debugging purposes, to make sure that the nonce is received.
   answer = ser.read()
   while answer != b'o':
       answer = ser.read()
   print(answer)

   #Calc digest
   hmac_gen = hmac.new(key, None, hashlib.sha1)
   hmac_gen.update(ih.tobinstr(0,32*1024-1)) #32*1024-1
   hmac_gen.update(nonce)
   print("Expected response:");
   print(binascii.hexlify(hmac_gen.digest()))

   # Get digest from mote
   mote_digest = ser.read(20)
   print("Prover response:");
   print(binascii.hexlify(mote_digest))


if __name__ == "__main__":
     main(sys.argv[1:])
