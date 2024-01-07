/* Librarii predefinite */
#include <iostream>
#include <stdio.h>
#include <graphics.h>
#include <winbgim.h>
#include <stdio.h>
#include <string.h>
/* Header files */
#include "include/UI.h"
#include "include/Constants.h"
#include "include/Structures.h"
#include "include/Functions.h"

using namespace std;

int main()
{
    FILE *pseudocode = fopen(INPUT_FILE, "r");
    if (!pseudocode)
    {
        printf("Could not open input code file");
        return -1;
    }

    // Process input file
    processFile(pseudocode);
    fclose(pseudocode);

    run();

    return 0;
}

void processFile(FILE *pseudocode)
{
    char rawCode[61];
    analyzeCode(pseudocode, rawCode);
}

void run()
{
    for (int i = 1; i <= blockVector.blockCount; i++)
        printf("i=%d, index=%d\n", i, blockVector.Block[i].index);
    bool running = true;
    int y = MAX_HEIGHT * 0.05;
    //printf("\n\n\n%d\n\n\n", getBlockSize(blockVector.Block[1]));
    const int diagramHeight = getDiagramHeight(blockVector);
    const int top_border = MAX_HEIGHT * 0.05, bottom_border = MAX_HEIGHT;
    initwindow(WIDTH, HEIGHT);
    setWindowTitle(TITLE);
    generateWindowContent();
    const int SCROLL_UNIT = 25;
    cout<<getBlockSize(blockVector.Block[1]);
    while (running) {
        printf("---\nCurrent top is at y=%d\n---\n", y);
        createDiagram(blockVector, y);
        // Double buffering to avoid screen tearing
        swapbuffers();
        char ch = getch();
        printf("got character %c - ascii: %d\n", (char)ch, ch);
        if (ch == 27 || ch == 13) running = false; // ESC KEY
        else {
            ch = getch();
            printf("got 2nd character %c - ascii: %d\n", (char)ch, ch);
            switch (ch) {
            case 72:
                //if (y - SCROLL_UNIT >= top_border)
                y += SCROLL_UNIT;
                break;
            case 80:
                cout << y + DIAGRAM_SIZE << '\n';
                //if (y + SCROLL_UNIT <= bottom_border)
                y -= SCROLL_UNIT;
                break;
            default:
                running = false;
                break;
            }
        }
        cleardevice();
    }
    closegraph();
}
