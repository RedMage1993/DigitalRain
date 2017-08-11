//*********************************************************
// Author: Fritz Ammon
// Date: 3/1/2012
// Program: Digital Rain
// Description: Creates the DigitalRain effect popularized
// by The Matrix.
//*********************************************************

#define HIBIT(b) ((0x80 << ((sizeof(b) - 1) * 0x08)) & b)
#define FRAME_DELAY 40

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <Windows.h>

using namespace std;

WORD RandomInteger(WORD min, WORD max);

const int MIN_ASCII = 48;
const int MAX_ASCII = 96;
const int MIN_COLOR = 2;
const int MAX_COLOR = 7;

int main()
{
    HANDLE hMyScreen = 0;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int size = 0;
    CHAR_INFO* chInfoTable = 0;
    COORD origin;
    SMALL_RECT boundaries;

	SetConsoleTitle(TEXT("A Noob's Cry by Fritos"));
    
    srand(static_cast<unsigned int> (time(0)));
    
    hMyScreen = GetStdHandle(STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(hMyScreen, &csbi); // Get the size of the console screen buffer.
    
    size = csbi.dwSize.X * csbi.dwSize.Y;
    
    chInfoTable = new CHAR_INFO[size];
    memset(chInfoTable, 0, sizeof(CHAR_INFO) * size);
    
    origin.X = origin.Y = 0; // Where do the special characters begin in the grid array?
    boundaries.Top = boundaries.Left = 0; // Where can the grid array be displayed?
    boundaries.Bottom = csbi.dwSize.Y;
    boundaries.Right = csbi.dwSize.X;
    
    do
    {   
        for (int row = csbi.dwSize.Y - 1; row >= 0; row--)
        {
            for (int col = 0; col < csbi.dwSize.X; col++)
            {
                // Push raindrop down.
                if (chInfoTable[(row * csbi.dwSize.X) + col].Char.AsciiChar)
                {
					 chInfoTable[((row + 1) * csbi.dwSize.X) + col].Char.AsciiChar = static_cast<char> (RandomInteger(MIN_ASCII, MAX_ASCII));
					 chInfoTable[((row + 1) * csbi.dwSize.X) + col].Attributes = RandomInteger(MIN_COLOR, MAX_COLOR) | FOREGROUND_INTENSITY;
                     
                     chInfoTable[(row * csbi.dwSize.X) + col].Char.AsciiChar = 0;
                     chInfoTable[(row * csbi.dwSize.X) + col].Attributes = 0;
                }
            }    
        } 
        
        for (int col = 0; col < csbi.dwSize.X; col++)
        {
            if (chInfoTable[csbi.dwSize.X + col].Char.AsciiChar == 0) // If a raindrop isn't there.
            {
                switch (RandomInteger(1, 100))
                { // Create chance that a raindrop will begin to fall.
                case 1:
                    chInfoTable[col].Char.AsciiChar = static_cast<char> (RandomInteger(MIN_ASCII, MAX_ASCII));
                    chInfoTable[col].Attributes = RandomInteger(MIN_COLOR, MAX_COLOR) | FOREGROUND_INTENSITY;
                    break;
                }
            }
            else // If raindrop /is/ there.
            {
                switch (static_cast<int> (RandomInteger(1, 100) / 10))
                { // Create chance that the raindrop will continue to grow.
                    case 1:
						chInfoTable[col].Char.AsciiChar = 0;
                        chInfoTable[col].Attributes = 0;
						break;
					default: // Chance that the raindrop will end.
						chInfoTable[col].Char.AsciiChar = static_cast<char> (RandomInteger(MIN_ASCII, MAX_ASCII));    
                        chInfoTable[col].Attributes = RandomInteger(MIN_COLOR, MAX_COLOR) | FOREGROUND_INTENSITY;
                        break;
                }   
            }
        }
        
        // Display the entire grid array.
        WriteConsoleOutput(
            hMyScreen,
            chInfoTable,
            csbi.dwSize,
            origin,
            &boundaries);
      
		Sleep(FRAME_DELAY);
    } while (!(HIBIT(GetAsyncKeyState(VK_SHIFT))
        && HIBIT(GetAsyncKeyState(VK_ESCAPE)))); // Shift + Escape closes the program.
    
    delete [] chInfoTable; // Delete the grid array.
    
    return 0;
}

WORD RandomInteger(WORD min, WORD max)
{
    return min + (rand() % (max - min + 1));
}
