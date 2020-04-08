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

        // CHAR_INFO actualMap[10][10] = {
        //     {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        //     {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        //     {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        //     {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        //     {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        //     {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        //     {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        //     {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        //     {'#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        //     {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
        // };

        // This is an array of character and color data
        CHAR_INFO map[400] = {
            '#', '#', '#', '#', '#', '#', '#', '#', '#', '#','#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
            '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
            '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
            '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
            '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
            '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
            '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
            '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
            '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#', '#',
            '#', '#', '#', '#', '#', '#', '#', '#', '#', '#','#', '#', '#', '#', '#', '#', '#', '#', '#', '#',
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


        HANDLE oldScreenBuffer, newScreenBuffer;
        

        oldScreenBuffer = GetStdHandle(STD_OUTPUT_HANDLE);
        newScreenBuffer = CreateConsoleScreenBuffer(
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


}