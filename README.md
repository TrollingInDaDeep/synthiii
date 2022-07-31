# synthiii

PureData Synthesizer using Automationism3.1 -ish
started out by using Raspberry Pi 3b but it's a piece of junk.
using and old AMD platform PC instead now.
On Raspberry I used the Orac Patchbox distro, it features a low latency kernel and built-in PureData installation.
Was much better and faster but still giving only performance problems.
Also Raspberry can't read analog signals like potentiometer values, so I need an ADC.
Currently using arduino Uno for that (attiny85 would also work but sending the value to PC is pain in the butthole)
With the Arduino I can just send it to serial via USB.
The base of the synth will still be Pure data and Automationism, this is fully platform independent!
So on this AMD platform currently running it on (hopefully headless) windows, but might switch to Patchbox or another low-latency linux kernel later.

Pure Data:
https://puredata.info/

Automationism:
https://www.automatonism.com/

Comport (PD plugin to get serial data in there)
https://puredata.info/downloads/comport

Orac Patchbox
https://community.blokas.io/t/orac-2-0-for-the-raspberry-pi/1099

