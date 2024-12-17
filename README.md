# synthiii

## Storytime
I wanted a synthesizer. A modular Synthesizer. Analog. But they expensive, me no money. But I like electronics and figured I could build one myself.
Seen some videos from LookMumNoComputer and it didn´t look that hard. Believe me, it is. Not the "soldering some pots and caps and jack sockets together" part, but the "decide on a way to go in this endless jungle of possibilities".
I have spent several years just starting different approaches, changing direction halfway through, restarting, failing, stopping and retrying and now I can proudly say, that I am not even close to having a finished music product.

### Analog Modular
First plan was to build analog modules with existing Kits. Problem: Either the Kits are available as full set but that has it´s price. Other option is freely available kits like from LookMomNoComputer or MFOS, however there you have to spend a lot of time to get everything together, electronics, PCB, Frontpanel. Even when spending a good amount of time with DIY analog Kits, you will probably not have a crazy multi-effect-station with polyrhythms and all the good things. The advantage of Modular is, you can just build one by one module, test it individually and then move to the next. In hindsight I maybe should´ve done this to stay organized. The price, complexity and relatively limited results sent me off this path and I wanted to try something else.

### Digital semi-modular, Midi Controller
Wanting more fancy effects, more flexibility and less fixed hard wired things, I started trying to go a software route. I discovered VCV rack, a virtual modular synthesizer software, insane great stuff. Exactly what I wanted, but not hardware controlled. I needed a computer and mouse to use it, which is against the idea of having an instrument. You can control VCV Rack via MIDI, so I could just try to recreate modules from VCV rack and have them controlled by hardware knobs. But this would not have the possibility to patch the modules together with cables, leading me to the idea to build a MIDI controller with just knobs and buttons, but no patch cables (In the meantime this actually exists: https://midilar-controller.webnode.sk/, maybe would be an option for me.).
Sort of a fixed Modular system. As you can imagine, its impossible to settle on a collection of modules you like and then decide on a fixed patch setup. Also getting VCV Rack to run on a device which isn´t a full PC had some challenges for me.

### Raspberry Pi, Pisound





I think that was the thing I will stress to you the most: *Don´t expect to be finished ever*. You´ll have a much better time when you just enjoy the process. And if you don´t? Then your lifetime is not worth building everything on your own. Better spend a bit of money, to have at least a small sound making device, for me, a Korg Volca sample was a cheap, great way to just jam. It brought me to new ideas for my own built synth. And I still can rock on even if my synth is not finished.

Second advice I can give you: *Small steps*



I thought spending even close to 500 moneycoins for a synth I can design and build myself is stupid. Then I spent 3 years of learning, hacking, buying parts, hardware, screws, and even designing PCB´s. Over time I probably spent more than 500€ and did not even have one session of making something musical with my synth.



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

