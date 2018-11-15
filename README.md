# IOTCompass
A device that always points to where you want to go. Using the Azure Sphere

## Overview

A device consisting of an Azure Sphere connected to a serial GPS reciever and stepper motor with a pointer on the motor shaft. As the device is moved it will log its position and calculate what direction a pointer needs to point so that it's always pointing at the same destination.

Where would your pointer point? To your true love? To Home? To a different place depending on the time of day?

## Decoding the comms
The GNSS (Global Navigation Satellite System) reciever is a [NEO M8N](https://www.u-blox.com/en/product/neo-m8-series) this serial module streams the details of the satellite it detects using a protocol called NMEA. The project will need to filter out the right messages, and parse these to provide the location back to the code.

## Getting the destination
Although the Azure Sphere does not need to use the Azure IOT hub to work, I'll take advantage of the "Device Twin" functionality to pass the co-ordinates from a web UI back to the device.

## Calculating the direction
The [FunctionApp](https://github.com/Workshopshed/IOTCompass/tree/master/FunctionApp) folder contains the code for an Azure function that calculates a bearing based on two sets of co-ordinates. Thanks to Chris Veness for the function https://www.movable-type.co.uk/scripts/latlong.html which I've ported to C#. This function will be called from the Azure Sphere using the CURL library.

## Driving the stepper motor
The stepper motor for the is a little 28BYJ-48 with a darlinton array to drive it. This will be driven from 4 of the GPIOs on the Azure Sphere. I'll need to port/create a suitable library for this based on the LED examples.

 



 

