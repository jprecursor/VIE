V I E: an automata sequencer 
===
ENCYST LABS x NATANH

Licensed under BSD 2-Clause license

===

This project is created using the JUCE library. 

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
