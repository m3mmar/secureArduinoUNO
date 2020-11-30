# A simple Software-based Security Architecture for ATmega328P Microcontroller

## This repo represents an instantiated implementation of the Security MicroVisor on Arduino UNO that has AVR ATmega328P MCU. This implementation provides memory isolation, remote attestation, and authenticated code deployment. 

## Folders Description

- `core/`: This folder contains the Trusted Computing Module (TSM) along with the edited toolchain (+ any other required files).
    -  `Makefile.include`: This is the main makefile that holds all the configurations for deployed either the TSM or the untrusted application. 
    -  `microvisor.c`: This file represents the core functionality of the TSM. All code and data in this file are stored in the protected memory area. 
    -  `mem_layout.h`: It contains the memory layout of the referenced microcontroller w.r.t. the logical separation between the protected and unprotected memory zones. 
    -  `virt_i.S`: An assembly implementation of the secure virtualized instructions. They are stored in the secure memory area as well. The edited toolchain will replace all unsafe instructions with one of these secure virualized ones. They can be ivoked by the untrusted software due to the fact that they hard-coded as main entry points to the secure memory area. To see the entire list of entry points, see lines 28-37 in microvisor.c file.
    -  Other files are either parts of the modified toolchain or the cryptographic primitives used (all are inside crypto folder). The 'scripts' folder contains two python scripts that are needed to either inject some metadata, i.e. MAC value, during producing the binary image to be deployed, or to produce an image that is valid for over-the-air update.

- `apps/`: This folder contains samples of untrusted applications that can be deployed in the insecure memory area.
    -  `hello_world`: This is just a simple testing application that sends the word 'test' over UART and then keeps looping infinitely to prevents return from the main memory. To send the word again, press the reset button of Arduino UNO.
    -  `remote_attest`: This is another simple application that would invoke the remote attestation (RA) function inside the secure memory area to compute the attestation report whenever an RA request (nonce) is received. Please note that, as an integrity checking function, we use here HMAC-SHA1. This can be simply replaced with any other crypto module.
    -  `secure_loading`: This is a simple receiver application that would automatically receive any binary image of any size and invoke the required functions inside the secure memory area to verify it and deploy it if it is safe. 



## NOTES

- The current configurations supports an optimized assembly implementation of the HMAC-SHA1 function. It is faster and smaller in size that the .c verion of the same implementation. However, if you want to use the .c implementation, please update line #16 in core/Makefile.include. You have to replace 'sha1-asm.S' with 'sha1.c'. The 'sha1.c' is already provided to you in the core/crypto/ folder. If you want to use your own crypto primitive, i.e. HMAC-SHA2, you have to add the required files into the core/crypto/ folder, update line #16 in Makefile.include, and update all functions inside core/microvisor.c that make use of HMAC-SHA1. In case of RA, you have to update the python code of the verifier as well, which is stored in apps/remote_attest/verifier.py.

- This implementation could work perfectly with any AVR MCU of 32kB of Flash size. All you need to update is the name of the MCU in the core/Makefile.include (and propably adjust the serial communication if it is different from Atmega328P MCU. The serial configurations are stored in serial.c file in each of the referenced apps).

- To update any of the configurations of MSP328P MCU, please look at core/Makefile.include. For example, the current configurations are adjusted to use the internal 8MHz clock. You can edit them to consider using the external one that is already supported on Arduino UNO, which is 16MHz. Please make sure that whatever you modify is considered also in the deployment commands that are written the end of the file. For instance, fuses have to be adjusted accordingly. 

- Our current configurations for Arduino UNO are: 8MHz of internal clock, and using 4kB as a bootloader memory.

- The cross-developement toolchain is tested on MAC OS. If you are using another operating system, please make sure that the commands inside core/Makefile.include are compatible with your enviroment.  





## Let's do a quick Test!
- Let's test the implementation with the hello_world app!
- Connect your Arduino UNO platform to your computer through the usb port. Also, please connect the JTAG port using JTAGICE3. If you have other external programers, modify the configurations accordingly in core/Makefile.include.
- navigate to apps/hello_world.
- Run as a first command: "make fuse4". This will configure some hardware fuses on the platform. Please make sure it succeeded before moving to the next step. You should expect as output like: 

            avrdude: verifying ...
            avrdude: 1 bytes of lock verified

            avrdude: safemode: Fuses OK (E:FD, H:D1, L:E2)

            avrdude done.  Thank you.

- Run the command: "make main.hex" to produce the .HEX file that has to be deployed on the platform. You should see this file in apps/hello_world folder after running this command.
- Run the command: "make flash" to deploy the .HEX file on the connected platform. 
- Now, using any serial interface on your computer, you should be able to receive the word "test" at baudrate 9600. To see it again and again, keep pressing the reset button of your Arduino UNO.

## Further Things!
- To see the layout of your memory, run the command "make size". By then, you should be able to see the size of bootloader section (the protected memorty area), the text section (the unprotected memory area), etc.
- To read the entire Flash memory, run the command "make read". This command will produce .bin file to you that is similar to the .hex one.

## Running RA!
- Run the first step in the previous section. 
- navigate to apps/remote_attest
- Run: make main.hex
- Run: make flash
- Then, run the python script file verifier.py that serves as a verifier, attaching as parameters the hex file and the serial port of the attached MCU. It should be run using Python 3. This script will send a 20-byte nonce to the prover, compute the MAC over the entire 32kB of Flash using a pre-shared secret key, print the results, and print what is received as an attestation response from the prover. In the benign settings, both values much match!.

## Running Authenticted code deployment!!
- Run the first step in the previous section. 
- navigate to apps/secure_loading
- Run: make main.hex
- Run: make flash
- Now the app is deployed and ready to receive binary images. Let's assume that we want to deploy the hello_world app again but over the air. Let's navigate back to apps/hello_world and run the command: make main.bin. This command will produce a binary image without the TSM. 
- Run the python script serial.loader.py giving it as a parameter the .bin file and the usb port where the Arduio is connected. This will send the entire image to the prover (Arduino). By then, the untrusted software will invoke the necessary function to verify the received image and deploy it if it is safe. Otherwise, it will not be deployed. To see the effect, connect to the serial interface gain and press the reset button of Arduino. 


### Enjoy the secure world! For further info, please contact me via: ma7moud.ammar@gmail.com 















