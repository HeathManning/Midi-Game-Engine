#include "RtMidi.h"

#pragma once
class MidiDevice : public MidiOutput, public MidiInput
{

};

class MidiOutput
{
private:
	RtMidiOut* midiOut;

public:
	MidiOutput();
	~MidiOutput();

	
};

class MidiInput
{
private:
	RtMidiIn* midiIn;

public:
	MidiInput();
	~MidiInput();


};