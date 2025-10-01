# synthiii

For technical overview and documentation, head [to the Documentation](./Documentation/Documentation.md).

If you want to build your own project, take my advice below.

## Storytime
I wanted a synthesizer. A modular Synthesizer. Analog. But they expensive, me no money. But I like electronics and figured I could build one myself.
Seen some videos from LookMumNoComputer and it didn´t look that hard. Believe me, it is. Not the "soldering some pots and caps and jack sockets together" part, but the "decide on a way to go in this endless jungle of possibilities".
I have spent several years just starting different approaches, changing direction halfway through, restarting, failing, stopping and retrying and now I can proudly say, that I am not even close to having a finished music product.

### Analog Modular
First plan was to build analog modules with existing Kits. Problem: Either the Kits are available as full set but that has it´s price. Other option is freely available kits like from LookMomNoComputer or MFOS, however there you have to spend a lot of time to get everything together, electronics, PCB, Frontpanel. Even when spending a good amount of time with DIY analog Kits, you will probably not have a crazy multi-effect-station with polyrhythms and all the good things. The advantage of Modular is, you can just build one by one module, test it individually and then move to the next. In hindsight I maybe should´ve done this to stay organized. The price, complexity and relatively limited results sent me off this path and I wanted to try something else.

### Digital fixed-modular, Midi Controller
Wanting more fancy effects, more flexibility and less fixed hard wired things, I started trying to go a software route. I discovered VCV rack, a virtual modular synthesizer software, insane great stuff. Exactly what I wanted, but not hardware controlled. I needed a computer and mouse to use it, which is against the idea of having an instrument. You can control VCV Rack via MIDI, so I could just try to recreate modules from VCV rack and have them controlled by hardware knobs. But this would not have the possibility to patch the modules together with cables, leading me to the idea to build a MIDI controller with just knobs and buttons, but no patch cables (In the meantime this actually exists: https://midilar-controller.webnode.sk/, maybe would be an option for me.).
Sort of a fixed Modular system. As you can imagine, its impossible to settle on a collection of modules you like and then decide on a fixed patch setup. Also getting VCV Rack to run on a device which isn´t a full PC had some challenges for me.


### A satisfying "work in progress" state
Following I will stress to you the most: *Don´t expect to be finished ever*.
In the beginning I loved the electronics, soldering, programming, I even learned how to design PCBs. When I check back where I started, it's incredible to me how much i've learned. After some years it started to frustrate me. I spent so much time troubleshooting, programming and soldering but not making music, which was my ultimate goal in the first place. Sure I had prototypes, but it was not possible to just power them on and enjoy making sounds. I figured it was time to spend a little bit of money to have at least a small sound making device and discovered the Korg Volca Sample (V1 with pajen firmware). Out of nowhere I was making music and could just sit at the window and tweak knobs to get *sound*.
And with that joy, also my passion got re-ignited to customize and augment the sampler I had bought. Suddenly it was much clearer for me what I wanted to build because I knew how I wanted it to sound! And while these builds were not finished (which they never are), I could still jam happily.
You need to be in a state that you feel like you're doing exactly what you wanted. Don't postpone that happiness until your device is finished, because it never really will be.

### Time is valuable
Out of all the limited resources you'll have: Money, Frustration tolerance, space on your Desk etc... time is the most valuable one. If you spend several months reading about synths, ordering parts, writing code, being frustrated and heading to the bar in the evening, etc...
before you know, you will have spent a considerable amount of money in wrong Parts, beer and therapy. If you had just bought a small, finished device to build your ecosystem *around* you'd be actually off way cheaper.
If you do it good, you will eventually even replace the first instrument you bought with your DIY projects and you can sell it again. Don't get me wrong, you can spend all your life in tinkering, as long as you enjoy it.And if you don´t? Then your lifetime is not worth building everything on your own just to save a little bit of money.


### Simplicity
Last advice I can give you: *Small steps*
Limit yourself to build very simple devices first. I just kept expanding the "thing" I wanted to build. First it was a modular synth, then a midi controller, then a midi sequencer, then a drum machine. Currently it's most of that - a little bit. At the same time.
I would be so much further if I had just built a simple midi controller first. Then a midi sequencer. You learn so much in every single project and that knowledge will drastically improve your results in all further projects.
Plus if you have a project that's going nowhere, it's much easier to just scrap it (or save it for later) than if you have this huge monolith of code and cables that you spent so much energy on.
LITTLE PROJECTS. WORK YOUR WAY UP.


### My journey (don't follow)
Here's what i tried and where it lead me. Funny to look back to this thinking I actually thought the start of this was any good. Silly me...

* PureData Synthesizer using Automationism3.1 -ish
* started out by using Raspberry Pi 3b but it's a piece of junk.
* using and old AMD platform PC instead now.
* On Raspberry I used the Orac Patchbox distro, it features a low latency kernel and built-in PureData installation.
* Was much better and faster but still giving only performance problems.
* Also Raspberry can't read analog signals like potentiometer values, so I need an ADC.
* Currently using arduino Uno for that (attiny85 would also work but sending the value to PC is pain in the butthole)
* With the Arduino I can just send it to serial via USB.
* The base of the synth will still be Pure data and Automationism, this is fully platform independent!
* So on this AMD platform currently running it on (hopefully headless) windows, but might switch to Patchbox or another low-latency linux kernel later.
* Raspberry PI with pisound is great platform. includes patchbox low latency linux os.
* This creates great sound and has good software on it. (Modep)
* This SW I can control via midi.
* I build a midi controller with many inputs to control the Modep synth plugins (AMSYNTH)
* Arduino does not have enough analog inputs
* Teensy is the way to go. Low latency, fast, midi and great libraries already included, saves a ton of work
* Teensy still does not have enough analog I/O, i need to use multiplexers
* Soldering multiplexers together in a stable way with protoboard PCBs works but is pain
* Designing own PCB to have an easier system to plug many potentiometers via multiplexers into the teensy
* All working great, i write a shit ton of code to run on the teensy, my possibilities are (too) endless

