#include <cstdlib>
#include "RtMidi.h"

#pragma once

typedef unsigned char byte;
struct Colour
{
	byte r;
	byte g;
	byte b;

	Colour();
	Colour(byte red, byte green, byte blue);
};

class MidiGameEngine
{
private:
	RtMidiOut* midiout;
	RtMidiIn* midiin;

	bool isRunning, isPaused;
	std::vector<Colour>* grid;

	int CoordToIndex(int x, int y);

	void UpdateGrid();

public:
	MidiGameEngine();
	~MidiGameEngine();

	void Run();
	void Stop();

	virtual void Start();
	virtual void Update();

	void SetGridPixel(int x, int y, Colour colour);
	bool GetInput(std::vector<byte>* message);
	void SendOutput(std::vector<byte>* message);
};

