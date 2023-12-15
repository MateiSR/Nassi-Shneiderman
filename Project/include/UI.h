#include <graphics.h>
#include <winbgim.h>
#include "Constants.h"
#include "Structures.h"
#include "Files.h"
#include <iostream> // remove later
#define TITLE "Diagrame Nassi-Shneiderman"
#pragma once

/*
TODO: Dynamic blockSize by number of lines
TODO: Fix text indent (ie. in repeat until), something to do with resolution & font size
*/

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

// Choose function to use based on lineType
void drawBlock(block Block, int index, int top, int left, int blockSize);

// show the code from block
void showCodeFromBlock(block Block, int x, int y) {
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
                //std::cout << buffer;
                y += textheight(buffer);
                outtextxy(x + textwidth(buffer) / 2 + 50, y, buffer);
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
    outTextMiddle(HEIGHT * 0.05, TITLE);
    /*
    * cleardevice(); -- clears window, to be used later
    * clearviewport(); -- clears bg color
    */
}

// Draw simple instruction block
void drawSimpleBlock(block Block, int top, int left) {
    const int blockSize = textheight(Block.rawLine),
        bottom = top + blockSize,
        right = MAX_WIDTH;
    rectangle(left, top, right, bottom);
    int center = (left+right)/2;
    outtextxy(center, bottom, Block.rawLine);

}

// Draw if block start in NS diagram
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

// Draw the parallel block of the if/else
void drawIfParallelBlock(block Block, int top, int left, int blockSize = 200) {
    int bottom = top + blockSize,
        right = MAX_WIDTH;
    rectangle(left, top, right, bottom);
    int center = (left+right)/2;
    line(center, top, center, bottom);
    showCodeFromBlock(Block, left, top);
    // Find next else block
    bool foundElse = false;
    int i = Block.index + 1;
    while (!foundElse) {
        if (blockVector.Block[i].lineType == 2) {
            foundElse = true;
            block elseBlock = blockVector.Block[i];
            showCodeFromBlock(elseBlock, center, top);
        }
            i++;
    }
}

// Draw the full if block
void drawIfBlock(block Block, int top, int left, int blockSize = 200) {
    char* condition = Block.rawInstruction;
    const int ifBlockSize = 60;

    drawIfStartBlock(condition, top, left, ifBlockSize);
    drawIfParallelBlock(Block, top + ifBlockSize, left, blockSize = 200);
}

// --- For, while, do while (<=> repeat until !) ---

// Draw loop with initial test (eg. while) in NS diagram
void drawLoopTestBefore(block Block, int top, int left, int blockSize = 200) {
    char* condition = Block.rawInstruction;
    int bottom = top + blockSize,
    right = MAX_WIDTH;
    rectangle(left, top, right, bottom);
    rectangle(left + textheight(condition), top + textheight(condition), right, bottom);
    int center = (left+right)/2;
    outtextxy(center, top + textheight(condition), condition);
    showCodeFromBlock(Block, left, top + textheight(condition));
}

// Draw loop with final test(eg do while) in NS diagram
// Takes lineType = 4
void drawLoopTestAfter(block Block, int top, int left, int blockSize = 200) {
    // Find condition: Search for first lineType = 5
    char *condition = NULL;
    int i = Block.index + 1;
    while (!condition) {
        if (blockVector.Block[i].lineType == 5) condition = blockVector.Block[i].rawInstruction;
        i++;
    }
    // Draw and output text
    int bottom = top + blockSize,
    right = MAX_WIDTH;
    rectangle(left, top, right, bottom);
    rectangle(left + textheight(condition), top, right, bottom - textheight(condition));
    int center = (left+right)/2;
    outtextxy(center, bottom, condition);
    showCodeFromBlock(Block, left, top);
}

// Draw for loop in NS diagram
//void drawForLoop(char *condition, char *codeBlock, int top, int left, int blockSize = 200) {
// Takes lineType = 6
void drawForLoop(block Block, int top, int left, int blockSize = 200) {
    char* condition = Block.rawInstruction;
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
    //outtextxy(codeBlockLeft + textwidth(codeBlock), codeBlockTop + textheight(codeBlock), codeBlock);
    showCodeFromBlock(Block, left, codeBlockTop);
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
    */

    int top, left; top = left = 100;
    drawDiagramBorder(top, left);
    //drawSimpleBlock(blockVector.Block[1], 100, 100);
    //drawLoopTestAfter(blockVector.Block[6], 100, 100);
    //drawIfStartBlock("xxxx", 300, 100, 150);
    //drawForLoop(blockVector.Block[15], 300, 100);
    //drawLoopTestBefore(blockVector.Block[2], 500, 100);
    //drawIfBlock(blockVector.Block[6], 700, 100);
    /*
    int currentBlockSize = 150; // to be done automatically based on how much text there is
    for (int i = 0; i < blockVector.blockCount; i++) {
        int priority = blockVector.Block[i].priority + 1;
        //drawBlock(blockVector.Block[i], i, top, priority * currentBlockSize, currentBlockSize);
        top += currentBlockSize;
    }*/

    //showCodeFromBlock(blockVector.Block[2]);
}

void createWindow(blockChain blockVector) {
    initwindow(WIDTH, HEIGHT);
    setWindowTitle(TITLE);
    generateWindowContent();
    createDiagram(blockVector);
    getch(); // Keep window open
    closegraph();
}

void drawBlock(block Block, int index, int top, int left, int blockSize) {
/*
enum LineType
{
    otherStatement = 0,
    ifStatement = 1,
    elseStatement = 2,
    whileStatement = 3,
    repeatUntilStatementBegin = 4,
    repeatUntilStatementEnd = 5,
    forStatement = 6,
    braceBeggining = 7,
    braceEnd = 8
} ;*/
    if (Block.lineType == 1) drawIfBlock(Block, top, left, blockSize);
    else if (Block.lineType == 3) drawLoopTestBefore(Block, top, left, blockSize);
    else if (Block.lineType == 4) drawLoopTestAfter(Block, top, left, blockSize);
    else if (Block.lineType == 6) drawForLoop(Block, top, left, blockSize);
    else if (Block.lineType == 0) drawSimpleBlock(Block, top, left);
}
