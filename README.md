# SuperSaw
SuperSaw oscillator for KORG NTS-1


## What's this
SuperSaw oscillator, which is an emulation of JP8k for KORG NTS-1.
JP8k was a mid 90's great brand-new virtual analog synthesizer, and it introduces SuperSaw oscillator first.
Its characteristic sound is sometimes called something like "Anthem".
Swedish Royal Institute of Technology, Bachelor's Thesis "How to Emulate the Super Saw" written by ADAM SZABO was highly helpful to implement emulation. Special thanks to him.


## Prerequisite
[logue-sdk](https://github.com/korginc/logue-sdk)

If you use pre-built binary, logue-sdk is not required. Simply transfer [SuperSaw.ntkdigunit](https://github.com/kachine/nts1SuperSaw/raw/main/SuperSaw.ntkdigunit) to your NTS-1 using NTS-1 digital Librarian software.


## How to build
You have to change PLATFORMDIR variable in Makefile, it should correspond to where you installed logue-sdk (something like below).
```Makefile
PLATFORMDIR = $(PATHTO}/logue-sdk/platform/nutekt-digital
```

Then, simply type "make" to build this project.
```sh
$ make
```


## How to use
There are 2 paramters for this oscillator:
- Detune
This parameter controls the detune amount. Actual detune amount varies nonlinear like JP8k.
Mapped to OSC mode A knob(SHPE) and MIDI CC#54

- Mix
This parameter controls the mix amount of sideband saw waves. Actual mix amount varies nonlinear like JP8k.
Mapped to OSC mode B knob(ALT) and MIDI CC#55

If you use NTS-1's built-in LFO, only for the P(pitch) is effective and S(shape) will be ignored.


---
The names of products or companies are the trademarks or registered trademarks of their respective companies. These companies have no relation to this project.
