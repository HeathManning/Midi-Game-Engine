#include <iostream>

#define __WINDOWS_MM__
#include "MidiGameEngine.h"

class Snake : public MidiGameEngine
{
    std::vector<byte> input;
    void Start() override
    {
        input.resize(3);
    }

    void Update() override
    {
        bool gotInput = GetInput(&input);
        if (gotInput)
        {
            std::cout << (int)input[0];
            for (int i = 1; i < input.size(); i++)
            {
                std::cout << " " << (int)input[i];
            }
            std::cout << std::endl;

            if (input[0] == 0x90 || input[0] == 0xB0)
            {

            }
        }

        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                Colour colour((byte)rand(), (byte)rand(), (byte)rand());
                SetGridPixel(x, y, colour);
            }
        }



        if (input[1] == 19) // quit button
        {
            Stop();
        }
    }
};

int main()
{
    Snake snake;
    snake.Run();
}