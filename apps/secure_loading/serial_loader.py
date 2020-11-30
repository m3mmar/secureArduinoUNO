#!/usr/bin/env python3
import sys, os, struct, time
sys.path += [ os.path.join(os.path.split(__file__)[0], 'libs') ]
import serial

PAGE_SIZE = 256

def main(argv):
   if len(argv) != 2:
      print('serial_loader.py <binfile> <serialport>')
      sys.exit(2)

   # Check if binfile exists
   binfile = argv[0]
   if not os.path.isfile(binfile):
      print("ERROR: File not found:", binfile)
      sys.exit(2)

   ser = serial.Serial(argv[1], 9600)
   time.sleep(3)
   print('opened connection!')


   f = open(binfile, "rb")
   filecontent = f.read()

   # Decode some stuff from metadata header..
   total = struct.unpack("<H", filecontent[:2])[0]
   #data = struct.unpack("<H", filecontent[2:4])[0]
   twoword = struct.unpack("<H", filecontent[4:6])[0]

   # Calculate metadata header size
   header_size = 6 + (2 * twoword) + 20

   # Write metadata
   ser.write(filecontent[:header_size])

   # Wait for answer 'o' --> OK
   answer = ser.read()
   #while answer != b'o':
   #    answer = ser.read()
   print(answer)

   for i in range(0,total//PAGE_SIZE):
       ser.write(filecontent[header_size+(PAGE_SIZE*i):header_size+PAGE_SIZE*(i+1)])

       # Wait for answer 'o' --> OK
       answer = ser.read()
     #  while answer != b'o':
      #     answer = ser.read()
       print(answer)

   if total%PAGE_SIZE:
       ser.write(filecontent[-(total%PAGE_SIZE):])

       # Wait for answer 'o' --> OK
       answer = ser.read()
     #  while answer != b'o':
     #     answer = ser.read()
       print(answer)

   # Wait for answer 'd' --> Done
   answer = ser.read()
   #while answer != b'd':
   #    answer = ser.read()
   print(answer)

if __name__ == "__main__":
     main(sys.argv[1:])
