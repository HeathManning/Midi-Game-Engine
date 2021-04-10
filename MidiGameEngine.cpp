#pragma comment(lib, "winmm.lib") // I have not much idea what this is all about but it works
//#define __WINDOWS_MM__

#include "MidiGameEngine.h"
#include "RtMidi.h"

#include <cstdlib>
#include <chrono>
#include <thread>

MidiGameEngine::Colour::Colour()
{
	r = 0;
	g = 0;
	b = 0;
}

MidiGameEngine::Colour::Colour(byte red, byte green, byte blue)
{
	r = red / 2;
	g = green / 2;
	b = blue / 2;
}

MidiGameEngine::Colour MidiGameEngine::Colour::Random()
{
	return Colour((byte)rand(), (byte)rand(), (byte)rand());
}

MidiGameEngine::MidiGameEngine()
{
	isRunning = true;
	isPaused = false;
	midiout = new RtMidiOut();
	midiin = new RtMidiIn();
	midiin->ignoreTypes(false, false, false);

	grid = new std::vector<Colour>;
	grid->resize(256);
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
	std::string targetDevice("LPMiniMK3");
	std::string currentDevice;
	int chosenPort = -1;
	for (int i = 0; i < ports; i++)
	{
		currentDevice = midiout->getPortName(i);
		std::cout << currentDevice << std::endl;
		if (currentDevice.find(targetDevice) != std::string::npos)
		{
			chosenPort = i;
		}
	}
	if (chosenPort != -1)
	{
		midiout->openPort(chosenPort);
		std::cout << "Connected to: " << midiout->getPortName(chosenPort) << std::endl;
	}
	else
	{
		return;
	}

	ports = midiin->getPortCount();
	chosenPort = -1;
	for (int i = 0; i < ports; i++)
	{
		currentDevice = midiin->getPortName(i);
		std::cout << currentDevice << std::endl;
		if (currentDevice.find(targetDevice) != std::string::npos)
		{
			chosenPort = i;
		}
	}
	if (chosenPort != -1)
	{
		midiin->openPort(chosenPort);
		std::cout << "Connected to: " << midiout->getPortName(chosenPort) << std::endl;
	}
	else
	{
		return;
	}
	
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
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}

	// set launchpad back to live mode
	message[7] = 0x00;
	midiout->sendMessage(&message);
	std::cout << "Set Launchpad back to live mode" << std::endl;
}

unsigned int MidiGameEngine::CoordToIndex(int x, int y)
{
	if (x < 0)
	{
		x = 9 - (-x % 9);
	}
	if (y < 0)
	{
		y = 9 - (-y % 9);
	}
	return 11 + (x % 9) + 10 * (y % 9);
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
	// TODO: fix this hacky mess

	int foo = 0;

	std::vector<byte> message;
	message.resize(8 + 5 * 81);
	message[0] = 0xF0;
	message[1] = 0x00;
	message[2] = 0x20;
	message[3] = 0x29;
	message[4] = 0x02;
	message[5] = 0x0D;
	message[6] = 0x03;
	message.back() = 0xF7;

	for (int i = 7; i < 412; i += 5)
	{
		byte lightIndex = (byte)CoordToIndex(foo % 9, foo / 9);
		Colour lightColour = (*grid)[lightIndex];
		message[i] = 0x03;
		message[i + 1] = lightIndex;
		message[i + 2] = lightColour.r;
		message[i + 3] = lightColour.g;
		message[i + 4] = lightColour.b;
		foo++;
	}

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