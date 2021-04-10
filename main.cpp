#include <iostream>

#include "MidiGameEngine.h"

class Snake : public MidiGameEngine
{
    class SnakeGame
    {
    private:
        struct Point
        {
            unsigned int x;
            unsigned int y;
        };

        struct Apple
        {
            Point pos;
            bool eaten;
        };

        std::vector<Point> segments;
        Apple apple;
        int direction = 0;

    public:
        SnakeGame()
        {
            apple = Apple();
            segments.push_back(Point());
        }
        SnakeGame(Point pos)
        {
            segments.push_back(pos);
        }
        ~SnakeGame()
        {

        }

        bool SetDirection(int dir)
        {
            
            int newDir = dir % 4;
            if (direction + 2 != newDir && direction - 2 != newDir)
            {
                direction = newDir;
                return true;
            }
            else {
                return false;
            }
        }

        bool Update()
        {
            if (apple.eaten)
            {
                apple.eaten = false;
                apple.pos = { (unsigned int)(rand() % 9), (unsigned int)(rand() % 9) };
            }

            int dx, dy;
            switch (direction)
            {
            case 0:
                dx = 1;
                dy = 0;
                break;
            case 1:
                dx = 0;
                dy = 1;
                break;
            case 2:
                dx = -1;
                dy = 0;
                break;
            case 3:
                dx = 0;
                dy = -1;
                break;
            default:
                dx = 0;
                dy = 0;
            }



            Point last = segments.back();
            for (int i = segments.size() - 1; i > 0; i--)
            {
                segments[i] = segments[i - 1];
            }

            segments[0].x = (segments[0].x + dx);
            segments[0].y = (segments[0].y + dy);

            if (segments[0].x < 0 || segments[0].y < 0 || segments[0].x > 8 || segments[0].y > 8)
            {
                segments[0].x = (segments[0].x - dx);
                segments[0].y = (segments[0].y - dy);
                return false;
            }

            if (segments[0].x == apple.pos.x && segments[0].y == apple.pos.y)
            {
                segments.push_back(last);
                apple.eaten = true;
            }

            for (int i = 1; i < segments.size(); i++)
            {
                if ((segments[0].x == segments[i].x && segments[0].y == segments[i].y))
                {
                    return false;
                }
            }

            return true;
        }
        void Draw(MidiGameEngine* instance, Colour snakeCol, Colour appleCol)
        {
            if (!apple.eaten)
            {
                instance->SetGridPixel(apple.pos.x, apple.pos.y, appleCol);
            }

            for (int i = 0; i < segments.size(); i++)
            {
                instance->SetGridPixel(segments[i].x, segments[i].y, snakeCol);
            }
        }
    };

    std::vector<byte> input;
    SnakeGame snake;
    void Start() override
    {
        input.resize(3);
    }

    void Update() override
    {
        for (int x = 0; x < 9; x++)
        {
            for (int y = 0; y < 9; y++)
            {
                SetGridPixel(x, y, Colour(0, 0, 0));
            }
        }


        bool gotInput = GetInput(&input);
        if (gotInput)
        {
            std::cout << (int)input[0];
            for (int i = 1; i < input.size(); i++)
            {
                std::cout << " " << (int)input[i];
            }
            std::cout << std::endl;

            if ((input[0] == 0x90 || input[0] == 0xB0) && input[2] != 0x00)
            {
                if (input[1] > 90 && input[1] < 95)
                {
                    bool moveSuccessful = false;
                    switch (input[1])
                    {
                    case 91: // up arrow
                        moveSuccessful = snake.SetDirection(1);
                        break;
                    case 92: // down arrow
                        moveSuccessful = snake.SetDirection(3);
                        break;
                    case 93: // left arrow
                        moveSuccessful = snake.SetDirection(2);
                        break;
                    case 94: // right arrow
                        moveSuccessful = snake.SetDirection(0);
                        break;
                    }
                    if (moveSuccessful)
                    {
                        bool snakeDead = !snake.Update();
                        if (snakeDead)
                        {
                            snake = SnakeGame();
                        }
                    }
                }
            }
        }

        
        snake.Draw(this, Colour(0, 127, 0), Colour(127, 0, 0));
        
        

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