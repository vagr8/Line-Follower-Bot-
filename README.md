# Line-Follower-Bot-
Line Follower Bot

Environment: Visual Code, extnsion: PlatformIO

This program runs on Arduino Uno with 4 IR sensors; 1 7-segment displayer; 4 LED bulbs.
Sensor.cpp will check if sensor is working and shows the read.

Control_PID uses PID controler to control.
Control_White.cpp takes all 4 sensors on white as straight line.
Control_Black.cpp supposes when the 2 sensors in the middle is black as a straight line.

All these 3 are possible to run a line follower bot.

Amoing those, Control_White is the most stable one, and it has LED effect so its cooler.
