#!/bin/sh

# Get the essential packages
sudo apt-get update
sudo apt-get install build-essential python3-dev python3-pip -y

# Get the CircuitPython library for the BeagleBone
sudo pip3 install Adafruit_BBIO
sudo pip3 install --upgrade Adafruit_BBIO

# Get setuptools_scm (so that Blinka installation would work)
sudo pip3 install setuptools_scm

# Get Adafruit-Blinka (requirements for seesaw)
sudo pip3 install Adafruit-Blinka

# Get seesaw for moisture sensor
sudo pip3 install adafruit-circuitpython-seesaw