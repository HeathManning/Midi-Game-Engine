#pragma comment(lib, "winmm.lib") // I have not much idea what this is all about but it works

#include "MidiGameEngine.h"
#include "RtMidi.h"

#include <cstdlib>
#include <chrono>
#include <thread>

Colour::Colour()
{
	r = 0;
	g = 0;
	b = 0;
}

Colour::Colour(byte red, byte green, byte blue)
{
	r = red / 2;
	g = green / 2;
	b = blue / 2;
}

MidiGameEngine::MidiGameEngine()
{
	isRunning = true;
	isPaused = false;
	midiout = new RtMidiOut();
	midiin = new RtMidiIn();
	midiin->ignoreTypes(false, false, false);

	grid = new std::vector<Colour>;
	grid->resize(81);
}

MidiGameEngine::~MidiGameEngine()
{
	delete midiout;
	delete midiin;
	delete grid;
}

void MidiGameEngine::Run()
{
	// TODO: Upgrade device logic to support more devices and hot plugging
	
	int ports = midiout->getPortCount();
	std::string chosenDevice;
	midiout->openPort(2);
	chosenDevice = midiout->getPortName(1);
	if (!(midiout->isPortOpen()))
	{
		return;
	}
	
	std::cout << "Output device: " << chosenDevice << std::endl;
	
	midiin->openPort(1);
	chosenDevice = midiin->getPortName(1);
	std::cout << "Input device: " << chosenDevice << std::endl;
	
	// set launchpad to programmer mode
	std::vector<byte> message;
	message.resize(9);
	message[0] = 0xF0;
	message[1] = 0x00;
	message[2] = 0x20;
	message[3] = 0x29;
	message[4] = 0x02;
	message[5] = 0x0D;
	message[6] = 0x0E;
	message[7] = 0x01;
	message[8] = 0xF7;
	midiout->sendMessage(&message);
	std::cout << "Set Launchpad to programmer mode" << std::endl;


	// TODO: Upgrade timing logic to make more robust, although not sure what is best practice
	Start();

	while (isRunning)
	{
		if (!isPaused)
		{
			Update();
			UpdateGrid();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(20)); // 50Hz?
	}

	// set launchpad back to live mode
	message[7] = 0x00;
	midiout->sendMessage(&message);
	std::cout << "Set Launchpad back to live mode" << std::endl;
}

int MidiGameEngine::CoordToIndex(int x, int y)
{
	return 11;
}

void MidiGameEngine::SetGridPixel(int x, int y, Colour colour)
{
	int index = CoordToIndex(x, y);
	if (index != -1)
	{
		(*grid)[index] = colour;
	}
}

bool MidiGameEngine::GetInput(std::vector<byte> *message)
{
	midiin->getMessage(message);
	return (*message).size() != 0;
}

void MidiGameEngine::SendOutput(std::vector<byte>* message)
{
	midiout->sendMessage(message);
}

void MidiGameEngine::UpdateGrid()
{
	std::vector<byte> message;
	message.resize(8 + 5 * (*grid).size());
	message[0] = 0xF0;
	message[1] = 0x00;
	message[2] = 0x20;
	message[3] = 0x29;
	message[4] = 0x02;
	message[5] = 0x0D;
	message[6] = 0x03;
	int l = 11;
	for (int i = 7; i < message.size(); i += 5)
	{
		message[i] = 0x03;
		message[i + 1] = (byte)(l);
		message[i + 2] = (*grid)[l].r;
		message[i + 3] = (*grid)[l].g;
		message[i + 4] = (*grid)[l].b;
		l++;
	}
	message.back() = 0xF7;

	midiout->sendMessage(&message);
}

void MidiGameEngine::Stop()
{
	isRunning = false;
}


void MidiGameEngine::Start()
{
	return;
}

void MidiGameEngine::Update()
{
	return;
}