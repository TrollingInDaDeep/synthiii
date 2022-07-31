a micropatch, for crappy cpus (like raspberry PI......)
some basic modules, producing some sounds.
Is set up to be operated with midi controller akai lpd8 (or any midi controller)

controls modify parameters as below:
ctlin 1:	pitch of OSC1
ctlin 2:	pitch of OSC2
ctlin 3:	clock rate (to enable the clock see below)
ctlin 4:	LFO1 frequency
ctlin 5:	LPF1 cutoff (connected to OSC 2)
ctlin 6:	LPF2 cutoff (connected to Noise generator)
ctlin 7:	Master volume


connections are made as follows:
you have to hold the note to retain the connection (stupid i know)
Notein 40:	connect LFO1 to LPF1 cutoff
Notein 41:	connect LFO1 to LPF2 cutoff
Notein 42:	connect OSC2 Pitch to Sequencer (only makes sense when clock is enabled)
Notein 43:	enable clock (set rate see above)