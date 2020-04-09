#include "Console.h"

namespace MyAscii {

    Console::Console(void)
    : Console("New window") {
    }

    Console::Console(std::string windowTitle) {
        this->windowTitle =  windowTitle;
        setConsoleTitle(windowTitle);
    }

    void Console::setConsoleTitle(std::string windowTitle) {
        SetConsoleTitleA(windowTitle.c_str());
    }

    void Console::showPlayField(void) {
        // This is an array of character and color data
        CHAR_INFO map[400] = {
            '#', 0x002C, '#', 0x002C, '#', 0x002C, '#', 0x002C, '#', 0x002C,'#', 0x002C, '#', 0x002C, '#', 0x002C, '#', 0x002C, '#', 0x002C,
            '#', 0x002C, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', 0x002C,
            '#', 0x002C, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', 0x002C,
            '#', 0x002C, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', 0x002C,
            '#', 0x002C, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', 0x002C,
            '#', 0x002C, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', 0x002C,
            '#', 0x002C, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', 0x002C,
            '#', 0x002C, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', 0x002C,
            '#', 0x002C, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', 0x002C,
            '#', 0x002C, '#', 0x002C, '#', 0x002C, '#', 0x002C, '#', 0x002C,'#', 0x002C, '#', 0x002C, '#', 0x002C, '#', 0x002C, '#', 0x002C
        };

        COORD coordinateBufferSize;
        coordinateBufferSize.Y = 10;
        coordinateBufferSize.X = 10;

        COORD topLeftCoordinate;
        topLeftCoordinate.Y = 0;
        topLeftCoordinate.X = 0;

        PSMALL_RECT srcWriteRect;
        srcWriteRect->Top = 10;         // Number of rows to the top
        srcWriteRect->Left = 10;        // Numbers of columns to the side
        srcWriteRect->Bottom = 19;
        srcWriteRect->Right = 19;

        HANDLE oldScreenBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
        HANDLE newScreenBuffer = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL);
        
        BOOL succes = WriteConsoleOutputA(
            newScreenBuffer,                    // The new ScreenBuffer
            map,                                // The char array we want to display
            coordinateBufferSize,               // 
            topLeftCoordinate,
            srcWriteRect
        );

        SetConsoleActiveScreenBuffer(newScreenBuffer);
        Sleep(600000);        
        SetConsoleActiveScreenBuffer(oldScreenBuffer);
    }

    void Console::showPlayField(std::vector<Tile> tiles) {

        // 1 tile is 9 vertical and 5 horizontal characters


        const int MAP_SIZE = 45;
        CHAR_INFO map[MAP_SIZE];


        static int TILE_CENTER = 22;

        // Display playfield on screen
        for (static int j = 0; j < 45; j++) {
            map[j].Char.UnicodeChar = (j == TILE_CENTER ? tiles[0].getAsciiChar() : L' ');     // Use   L'▓' for unicode chars
            map[j].Attributes = 0x20;
        }




        COORD coordinateBufferSize;
        COORD topLeftCoordinate;
        PSMALL_RECT srcWriteRect;
        

        coordinateBufferSize.Y = 5;
        coordinateBufferSize.X = 9;

        topLeftCoordinate.Y = 0;
        topLeftCoordinate.X = 0;

        srcWriteRect->Top = 4;         // Number of rows to the top
        srcWriteRect->Left = 10;        // Numbers of columns to the side
        srcWriteRect->Bottom = 8;
        srcWriteRect->Right = 18;

        HANDLE oldScreenBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
        HANDLE newScreenBuffer = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            CONSOLE_TEXTMODE_BUFFER,
            NULL);
        
        BOOL succes = WriteConsoleOutputW(
            newScreenBuffer,                    // The new ScreenBuffer
            map,                                // The char array we want to display
            coordinateBufferSize,               // 
            topLeftCoordinate,
            srcWriteRect
        );

        SetConsoleActiveScreenBuffer(newScreenBuffer);

        // Can i change a different part of the screen?

        // for (static int j = 0; j < 45; j++) {
        //     map[j].Char.UnicodeChar = (j == TILE_CENTER ? tiles[1].getAsciiChar() : L' ');
        //     map[j].Attributes = 0x20;
        // }
        
        // coordinateBufferSize.Y = 5;
        // coordinateBufferSize.X = 9;

        // topLeftCoordinate.Y = 0;
        // topLeftCoordinate.X = 0;

        // srcWriteRect->Top = 4;         // Number of rows to the top
        // srcWriteRect->Left = 21;        // Numbers of columns to the side
        // srcWriteRect->Bottom = 8;
        // srcWriteRect->Right = 29;

        // succes = WriteConsoleOutputW(
        //     newScreenBuffer,                    // The new ScreenBuffer
        //     map,                                // The char array we want to display
        //     coordinateBufferSize,               // 
        //     topLeftCoordinate,
        //     srcWriteRect
        // );


        
        // for (static int j = 0; j < 45; j++) {
        //     map[j].Char.UnicodeChar = (j == TILE_CENTER ? tiles[2].getAsciiChar() : L' ');
        //     map[j].Attributes = 0x20;
        // }
        
        // coordinateBufferSize.Y = 5;
        // coordinateBufferSize.X = 9;

        // topLeftCoordinate.Y = 0;
        // topLeftCoordinate.X = 0;

        // srcWriteRect->Top = 10;         // Number of rows to the top
        // srcWriteRect->Left = 10;        // Numbers of columns to the side
        // srcWriteRect->Bottom = 14;
        // srcWriteRect->Right = 18;

        // succes = WriteConsoleOutputW(
        //     newScreenBuffer,                    // The new ScreenBuffer
        //     map,                                // The char array we want to display
        //     coordinateBufferSize,               // 
        //     topLeftCoordinate,
        //     srcWriteRect
        // );


        // for (static int j = 0; j < 45; j++) {
        //     map[j].Char.UnicodeChar = (j == TILE_CENTER ? tiles[3].getAsciiChar() : L' ');
        //     map[j].Attributes = 0x20;
        // }
        
        // coordinateBufferSize.Y = 5;
        // coordinateBufferSize.X = 9;

        // topLeftCoordinate.Y = 0;
        // topLeftCoordinate.X = 0;

        // srcWriteRect->Top = 10;         // Number of rows to the top
        // srcWriteRect->Left = 21;        // Numbers of columns to the side
        // srcWriteRect->Bottom = 14;
        // srcWriteRect->Right = 29;

        // succes = WriteConsoleOutputW(
        //     newScreenBuffer,                    // The new ScreenBuffer
        //     map,                                // The char array we want to display
        //     coordinateBufferSize,               // 
        //     topLeftCoordinate,
        //     srcWriteRect
        // );


        



    }

}