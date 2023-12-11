#include <graphics.h>
#include <winbgim.h>
#include "Constants.h"
#include "Structures.h"
#include "Files.h"
#include <iostream> // remove later
#define TITLE "Diagrame Nassi-Shneiderman"
#pragma once

// Variables
const int WIDTH = 900, HEIGHT = 900;
const int MAX_WIDTH = WIDTH * 0.9, MAX_HEIGHT = HEIGHT * 0.9;

// Text sizes
enum TEXT_SIZES {
    H1 = 6,
    H2 = 5,
    H3 = 3,
    P = 2,
};

// show the code from block
void showCodeFromBlock(block Block) {
    FILE *in = fopen(INPUT_FILE, "r");
    goToLine(in, Block.lineNum);

    char buffer[61];
    bool done = false;
    bool started = false;
    while (!done && fgets(buffer, 61, in)) {
        int lineType = getLineType(buffer);

        if (lineType == 7 && !started) {
            started = true;
            continue;
        }

        if (lineType == 8) {
            done = true;
            continue;
        }

        if (started) {
                // TODO: change to outtextxy, review function definition
                std::cout << buffer;
        }

    }

    fclose(in);
}


void setWindowTitle(char* title) {
    HWND hwnd = GetFocus(); // Get the handle to the current window
    SetWindowText(hwnd, title);
}

void showCenteredText(char* text, int x, int y) {
    // Width & height of text
    int textWidth = textwidth(text);
    int textHeight = textheight(text);

    // Adjust coordinates so text is centered
    x -= textWidth / 2;
    y -= textHeight / 2;

    // Display text
    outtextxy(x, y, text);
}

// Center text on the x axis
void outTextMiddle(int y, char* s) {
    outtextxy(WIDTH/2, y, s);
}

void generateWindowContent() {

    setbkcolor(COLOR(45,45,45));
    setfillstyle(SOLID_FILL, COLOR(45,45,45));
    bar(0, 0, getmaxx(), getmaxy());  // Draw a rectangle covering background of entire window
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, H3); // Default font is too thin, use sans serif
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outTextMiddle(WIDTH/6, TITLE);
    /*
    * cleardevice(); -- clears window, to be used later
    * clearviewport(); -- clears bg color
    */
}

// Draw if block in NS diagram
void drawIfStartBlock(char *condition, int top, int left, int blockSize = 60) {
    int bottom = top + blockSize,
        right = MAX_WIDTH;
    rectangle(left, top, right, bottom);
    int center = (left+right)/2;
    line(left, top, center, bottom);
    line(right, top, center, bottom);

    outtextxy(left + textwidth("TRUE") /2, bottom, "TRUE");
    outtextxy(right - textwidth("FALSE") / 2, bottom, "FALSE");
    outtextxy(center, top + textheight(condition), condition);

}

// --- For, while, do while (<=> repeat until !) ---

// Draw loop with initial test (eg. while) in NS diagram
void drawLoopTestBefore(char *condition, int top, int left, int blockSize = 200) {
    int bottom = top + blockSize,
    right = MAX_WIDTH;
    rectangle(left, top, right, bottom);
    rectangle(left + textheight(condition), top + textheight(condition), right, bottom);
    int center = (left+right)/2;
    outtextxy(center, top + textheight(condition), condition);
}

// Draw loop with final test(eg do while) in NS diagram
void drawLoopTestAfter(char *condition, int top, int left, int blockSize = 200) {
    int bottom = top + blockSize,
    right = MAX_WIDTH;
    rectangle(left, top, right, bottom);
    rectangle(left + textheight(condition), top, right, bottom - textheight(condition));
    int center = (left+right)/2;
    outtextxy(center, bottom, condition);
}

// Draw for loop in NS diagram
void drawForLoop(char *condition, char *codeBlock, int top, int left, int blockSize = 200) {
    int bottom = top + blockSize,
    right = MAX_WIDTH;
    rectangle(left, top, right, bottom);
    int center = (left+right)/2;
    outtextxy(center, top + textheight(condition), condition);
    // Code block coords:
    int codeBlockTop = top + textheight(condition) * 1.5,
        codeBlockBottom = bottom - textheight(condition) * 1.5,
        codeBlockLeft = left+textheight(condition);
    rectangle(codeBlockLeft, codeBlockTop, right, codeBlockBottom);
    // Put text inside code block
    outtextxy(codeBlockLeft + textwidth(codeBlock), codeBlockTop + textheight(codeBlock), codeBlock);
    // ^^ to replace with drawCodeFromBlock
}

// Draw main (rectangle) diagram border
void drawDiagramBorder(int top, int left) {
    const int blockSize = MAX_HEIGHT - top;
    int bottom = top + blockSize,
    right = MAX_WIDTH;
    rectangle(left, top, right, bottom);
}


// Create diagram block by block
void createDiagram(blockChain blockVector) {
    /* Merg prin blocuri, tin cont de prioritati
     * adaug blockSize la X pentru fiecare prioritate crescuta (>)
     * si scad blockSize din X pentru fiecare prioritate scazuta (<)
     * incepand de la x=0;
     * pt if-uri logica separata; de acoperit;
     * --- de adagaut structura asociata lineType-urilor:
     * --- daca lineType = if, drawIfBlock etc.
    */

    drawDiagramBorder(100,100);
    // drawLoopTestAfter("x <= 321", 300, 300);
    // drawIfStartBlock("xxxx", 300, 300, 150);
    drawForLoop("(i=0;;i++)", "cout<<i; \n break;", 300, 300);

    // Aici doar parcurg block-urile din blockVector cu un for
    // TODO: schimba functiile sa ia ca argument un block, nu char *, ca pot folosi drawCodeFromBlock

    showCodeFromBlock(blockVector.Block[2]);
}

void createWindow(blockChain blockVector) {
    initwindow(WIDTH, HEIGHT);
    setWindowTitle(TITLE);
    generateWindowContent();
    createDiagram(blockVector);
    getch(); // Keep window open
    closegraph();
}
