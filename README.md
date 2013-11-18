V I E: an automata sequencer 
===
ENCYST LABS x NATANH

Licensed under BSD 2-Clause license

===

This project is created using the JUCE library and compiled at Universal(32/64-bit intel)(x86_64,i386) architecture with SDK OSX 10.8.
For SDK Mac OS X 10.7 and below, open the xcode project and change the base SDK to your current/ latest Mac OS X version.

[beta 0.1]

Cells are non-clickable to avoid confusion. All inputs/outputs by MIDI.

non- active cells are darkened.
fades on active/ non-active cells.

Rotary sliders are standardized to Ableton's Vertical Drag action.   

Velocity range slider:
	- min value constrained to 20.
	- max value constrained to not be lesser than min value. if so, max value shall be 
	  offset 10 away from min value.
	- no-random implemented when min value == max value
