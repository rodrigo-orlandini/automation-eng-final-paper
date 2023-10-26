# My Automation Engineering Final Paper 🦾🔌

My final paper is an intelligent machine to collect ripe strawberries.
The project is divided in 3 parts: mechanics, hardware and software.

** 1. Mechanics: ** 

The mechanical structure is composed of a 3-axis robotic arm, a threaded shaft, an electric engine and a support. 
The robotic arm is responsible for making the movements to pick up the selected strawberries. Threaded shaft is coupled in electric engine shaft and placed horizontally on top of the support. The arm is connected to the threaded shaft so that it's suspended and can move its base from left to right.

** 2. Hardware: **

The hardware is an Arduino Nano that is responsible for receive and send a specific protrocol via serial. This protocol is processed and the robotic arm coordinates is extracted from the message and the move is done. Also, it has a camera to take pictures for AI extract coordinates.

** 3. Software: **

It is a Python application that integrates the camera, serial port and an Artificial Inteligence. The software take a picture using camera, process the image with AI and identificate the coordinates of ripe strawberries and build a protocol with information to send via serial port for hardware interpretation.
