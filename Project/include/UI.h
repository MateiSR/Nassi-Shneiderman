#include <graphics.h>
#include <winbgim.h>
#include "Constants.h"
#include "Structures.h"
#include "Files.h"
#include "Functions.h"
#include <iostream> // remove later
#define TITLE "Diagrame Nassi-Shneiderman"
#pragma once

/*
TODO: Dynamic blockSize by number of lines
TODO: Fix text indent (ie. in repeat until), something to do with resolution & font size
*/

// Variables
const int WIDTH = 900, HEIGHT = 900;
const int MAX_WIDTH = WIDTH * 0.95, MAX_HEIGHT = HEIGHT * 0.95;

// Text sizes
enum TEXT_SIZES {
    H1 = 6,
    H2 = 5,
    H3 = 3,
    P = 2,
};

// Block colors
const int DEFAULT_COLOR = COLOR(255,255,255),
    IF_COLOR = COLOR(255, 0, 0),
    TEST_BEFORE_COLOR = COLOR(255,255,200),
    TEST_AFTER_COLOR = COLOR(200,200,0),
    FOR_COLOR = COLOR(173,216,230);

const int getBlockColor(int lineType) {
    if (lineType == 1) return IF_COLOR;
    if (lineType == 3) return TEST_BEFORE_COLOR;
    if (lineType == 4) return TEST_AFTER_COLOR;
    if (lineType == 6) return FOR_COLOR;
    return DEFAULT_COLOR;
}
// Choose function to use based on lineType
void drawBlock(block Block, int index, int top, int left, int right = MAX_WIDTH, bool showText = false, int color = getcolor());

// Block was visited and drawn
bool visited[101] = {false};

// show the code from block
void showCodeFromBlock(block Block, int x, int y) {
    FILE *in = fopen(INPUT_FILE, "r");
    goToLine(in, Block.lineNum);
    printf("Showing code from block %d\n", Block.index);
    char buffer[61];
    bool done = false;
    bool started = false;
    bool stoppedVisiting = false;
    int index = Block.index;
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
                ++index;
                if (lineType == 1 || lineType == 3 || lineType == 4 || lineType == 6 && index != Block.index) stoppedVisiting = true;
                if (lineType == 0 && !stoppedVisiting) {
                        visited[index] = true;
                    printf("visited index %d in function showCodeFromBlock\n", index);
                }
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
    //outTextMiddle(HEIGHT * 0.05, TITLE);
    /*
    * cleardevice(); -- clears window, to be used later
    * clearviewport(); -- clears bg color
    */
}

// Draw simple instruction block
void drawSimpleBlock(block Block, int top, int left, int color = getcolor()) {
    int originalColor = getcolor();
    setcolor(color);
    printf("Drawing simple block id=%d\n", Block.index);
    const int blockSize = textheight(Block.rawLine),
        bottom = top + blockSize,
        right = MAX_WIDTH;
    //rectangle(left, top, right, bottom);
    //int center = (left+right)/2;
    outtextxy(left + textwidth(Block.rawLine), bottom, Block.rawLine);
    setcolor(originalColor);
}

// Draw if block start in NS diagram
void drawIfStartBlock(char *condition, int top, int left, int blockSize = 50) {
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

// Draw the parallel block of the if
void drawIfParallelBlock(block Block, int top, int left, int blockSize = 200, bool showText = false) {
    int bottom = top + blockSize,
        right = MAX_WIDTH;
    rectangle(left, top, right, bottom);
    int center = (left+right)/2;
    line(center, top, center, bottom);
    int originalColor = getcolor();
    setcolor(IF_COLOR);
    if (showText) showCodeFromBlock(Block, left, top);
    setcolor(originalColor);
    // Find next else block
    bool foundElse = false;
    int i = Block.index + 1;
    while (!foundElse) {
        if (blockVector.Block[i].lineType == 2) {
            foundElse = true;
            block elseBlock = blockVector.Block[i];
            if (showText) showCodeFromBlock(elseBlock, center, top);
        }
            i++;
    }
    //printf("----%d----\n", foundElse);
}

// Draw the parallel block of the else
void drawElseParallelBlock(block Block, int top, int left, int blockSize = 200, bool showText = false) {

}

// Draw the full if block
void drawIfBlock(block Block, int top, int left, int blockSize = 200, bool showText = false) {
    printf("\n\n\n%d\n\n\n", blockSize);
    int originalColor = getcolor();
    int blockColor = IF_COLOR;
    setcolor(blockColor);
    char* condition = Block.rawInstruction;
    const int ifBlockSize = max((int)(blockSize / 3.5), 50);

    drawIfStartBlock(condition, top, left, ifBlockSize);
    //drawIfParallelBlock(Block, top + ifBlockSize, left, blockSize - ifBlockSize * 0.75, showText);
    drawIfParallelBlock(Block, top + ifBlockSize, left, blockSize - ifBlockSize, showText);
    delay(2000);
    setcolor(originalColor);
}

// --- For, while, do while (<=> repeat until !) ---

// Draw loop with initial test (eg. while) in NS diagram
void drawLoopTestBefore(block Block, int top, int left, int blockSize = 200, bool showText = false, int right = MAX_WIDTH) {
    int originalColor = getcolor();
    int blockColor = TEST_BEFORE_COLOR;
    setcolor(blockColor);
    char* condition = Block.rawInstruction;
    int bottom = top + blockSize;
    rectangle(left, top, right, bottom);
    rectangle(left + textheight(condition), top + textheight(condition), right, bottom);
    int center = (left+right)/2;
    outtextxy(center, top + textheight(condition), condition);
    if (showText) showCodeFromBlock(Block, left, top + textheight(condition));
    setcolor(originalColor);
}

// Draw loop with final test(eg do while) in NS diagram
// Takes lineType = 4
void drawLoopTestAfter(block Block, int top, int left, int blockSize = 200, bool showText = false, int right = MAX_WIDTH) {
    //printf("----%d---%d----\n", showText, Block.lineType);
    //setcolor(4);
    int originalColor = getcolor();
    int blockColor = TEST_AFTER_COLOR;
    setcolor(blockColor);
    // Find condition: Search for first lineType = 5
    char *condition = NULL;
    int i = Block.index + 1;
    while (!condition) {
        if (blockVector.Block[i].lineType == 5) condition = blockVector.Block[i].rawInstruction;
        i++;
    }
    // Draw and output text
    int bottom = top + blockSize;
    rectangle(left, top, right, bottom);
    rectangle(left + textheight(condition), top, right, bottom - textheight(condition));
    int center = (left+right)/2;
    outtextxy(center, bottom, condition);
    if (showText) showCodeFromBlock(Block, left, top);
    setcolor(originalColor);
    //setcolor(15);
}

// Draw for loop in NS diagram
//void drawForLoop(char *condition, char *codeBlock, int top, int left, int blockSize = 200) {
// Takes lineType = 6
void drawForLoop(block Block, int top, int left, int blockSize = 200, bool showText = false, int right = MAX_WIDTH) {
    int originalColor = getcolor();
    int blockColor = FOR_COLOR;
    setcolor(blockColor);
    char* condition = Block.rawInstruction;
    int bottom = top + blockSize;
    rectangle(left, top, right, bottom);
    int center = (left+right)/2;
    outtextxy(center, top + textheight(condition), condition);
    // Code block coords:
    int codeBlockTop = top + textheight(condition),
        codeBlockBottom = bottom - textheight(condition),
        codeBlockLeft = left+textheight(condition);
    rectangle(codeBlockLeft, codeBlockTop, right, codeBlockBottom);
    // Put text inside code block
    //outtextxy(codeBlockLeft + textwidth(codeBlock), codeBlockTop + textheight(codeBlock), codeBlock);
    if (showText) showCodeFromBlock(Block, left, codeBlockTop);
    setcolor(originalColor);
}

// Draw main (rectangle) diagram border
void drawDiagramBorder() {
    int bottom = MAX_HEIGHT,
    left = MAX_WIDTH * 0.05,
    top = MAX_HEIGHT * 0.05,
    right = MAX_WIDTH;
    printf("Diagram borders: %d %d %d %d \n", left, top, right, bottom);
    rectangle(left, top, right, bottom);
}

int getIfElseBlockSize(block Block) {
        int ifBlockSize = getBlockSize(Block),
            elseBlockSize = 0;
        unsigned int i = Block.index + 1;
        // caut urmatorul else cu aceeasi prioritate
        for (i; i < blockVector.blockCount && ! elseBlockSize; i++) {
            block current = blockVector.Block[i];
            if (current.lineType == 2 && current.priority == Block.priority) elseBlockSize = getBlockSize(current);
        }
        int maxBlockSize = max(ifBlockSize, elseBlockSize);
        printf("in getIfElseBlocksize: if : %d, else : %d, max : %d\n\n\n", ifBlockSize, elseBlockSize, maxBlockSize);
        return maxBlockSize;
}

// Get child coordinates
void getChildCoords(block Block, int &top, int &left, int &right, int blockSize) {
    char *condition = Block.rawInstruction;
    right = MAX_WIDTH;
    if (Block.lineType == 3) {
        top += textheight(condition);
        left += textheight(condition);
    }
    else if (Block.lineType == 4) {
        left += textheight(condition);
        //bottom -= textheight(condition);
    }
    else if (Block.lineType == 6) {
        left += textheight(condition);
        //bottom -= textheight(condition);
        top += textheight(condition);
    }
    else if (Block.lineType == 0) {
        top += textheight(Block.rawInstruction);
    }
    else if (Block.lineType == 1) {
        top += max((int)(blockSize / 3.5), 50);
        // daca e intr-un if
        right = (left+right)/2;
    }
    else if (Block.lineType == 2) {
        top += textheight(condition);
    }
}


// Create diagram block by block
void createDiagram(blockChain blockVector) {

    // la while in if nu se modifica corect top-ul
    // Block size ul unui if/else trebuie sa fie max(blockSizeIf, blockSizeElse)

    /* Merg prin blocuri, tin cont de prioritati
     * adaug blockSize la X pentru fiecare prioritate crescuta (>)
     * si scad blockSize din X pentru fiecare prioritate scazuta (<)
     * incepand de la x=0;
    */

    //drawSimpleBlock(blockVector.Block[1], 100, 100);
    //drawLoopTestAfter(blockVector.Block[8], 600, 100, 200, true);
    //drawIfStartBlock("xxxx", 300, 100, 150);
    //drawForLoop(blockVector.Block[16], 300, 100);
    //drawLoopTestBefore(blockVector.Block[2], 500, 100);
    //drawIfBlock(blockVector.Block[6], 100, 100);
    //drawBlock(blockVector.Block[6], -1, 300, 100);
    drawDiagramBorder();
    int diagram_bottom = MAX_HEIGHT * 0.95,
    diagram_left = MAX_WIDTH * 0.05,
    diagram_top = MAX_HEIGHT * 0.05,
    diagram_right = MAX_WIDTH * 0.95;


    int top = diagram_top, left = diagram_left;
    int oldIfTop = 0;
    for (int i = 1; i <= blockVector.blockCount; i++) {
        block current = blockVector.Block[i];
        if (visited[i]) continue;
        printf("Creating diagram: Block i = %d with %d children\n", i, current.children.num);
        int right = MAX_WIDTH;
        // check if everything is a lineType 0 after this
        bool allZeroes = true;
        for (int k = 1; k <= current.children.num; k++) {
            int kInd = current.children.indexes[k];
            block kBlock = blockVector.Block[kInd];
            if (kBlock.lineType != 0 && kBlock.lineType != 5) allZeroes = false;
        }

        // for if
        int oldRight = right;

        // treat else differently
        int oldLeft = left;
        int oldTop = top;
        if (current.lineType == 2) {
            int center = (left+right)/2;
            left = center;
            top = oldIfTop;
            top += max((int)(getIfElseBlockSize(blockVector.Block[current.index-1]) / 3.5), 50);
            //top += textheight("DOESNTMATTER");
            printf(">> Else block at index %d, handling separately\n", current.index);
        }
        if (current.lineType != 2) {
        if (!current.children.num || allZeroes) {
            // TODO: get colors from an enum
            drawBlock(current, -1, top, left, right, true, getBlockColor(current.lineType));
            visited[i] = true;
            top += getBlockSize(current);
            printf("showing text for id %d\n", current.index);
            continue;
        } else {
            drawBlock(current, -1, top, left, right, false);
            visited[i] = true;
            printf("NOT showing text for id %d\n", current.index);
        }
        }
        int _tempTop = top, _tempLeft = left, _tempRight = right;
        for (int j = 1; j <= current.children.num; j++) {
            printf("Current coords: top=%d left=%d right=%d\n", _tempTop, _tempLeft, _tempRight);
            int currBlockSize;
            if (current.lineType == 1 || current.lineType == 2) currBlockSize = getIfElseBlockSize(current);
                else currBlockSize = getBlockSize(current);
            getChildCoords(current, _tempTop, _tempLeft, _tempRight, currBlockSize);
            printf("Altered coords: top=%d left=%d right=%d\n", _tempTop, _tempLeft, _tempRight);
            int currentChildIndex = current.children.indexes[j];
            if (visited[currentChildIndex]) continue;
            block currentChild = blockVector.Block[currentChildIndex];
            // daca s-a ajuns la else dau break si il tratez separat
            if (currentChild.lineType == 2) break;
            // check if everything is a lineType 0 after this
            bool allZeroes = true;
            for (int k = j + 1; k <= current.children.num; k++) {
                int kInd = current.children.indexes[k];
                block kBlock = blockVector.Block[kInd];
                if (kBlock.lineType != 0 && kBlock.lineType != 5) allZeroes = false;
            }
            printf("Creating diagram: Block i = %d, Child j = %d, Drawable: %d, Validator: %d\n", i, currentChildIndex, isDrawableBlock(currentChild),
                   ((j == current.children.num) || allZeroes));
            if (!isDrawableBlock(currentChild) && currentChild.lineType != 0) continue;
            //if (currentChild.lineType == 0) outtextxy(left, top, currentChild.rawLine);
            drawBlock(currentChild, -1, _tempTop, _tempLeft, _tempRight, ((j == current.children.num) || allZeroes), getBlockColor(current.lineType));
            visited[currentChildIndex] = true;
        }
        // reset for if case
        if (current.lineType == 1) {
            right = oldRight;
            oldIfTop = top;
        }
        int currBlockSize;
        if (current.lineType == 1 || current.lineType == 2) currBlockSize = getIfElseBlockSize(current);
            else currBlockSize = getBlockSize(current);
        top += currBlockSize;
        // reset for else case
        if (current.lineType == 2) {
            left = oldLeft;
            top = oldTop;
        }
        //if (i == 6) break;
    }

    //showCodeFromBlock(blockVector.Block[2]);
    printf("Visited: \n");
    for (int i = 1; i <= blockVector.blockCount; i++)
        printf("Index: %d - Visited: %d \n", i, visited[i]);

}

void createWindow(blockChain blockVector) {
    initwindow(WIDTH, HEIGHT);
    setWindowTitle(TITLE);
    generateWindowContent();
    createDiagram(blockVector);
    getch(); // Keep window open
    closegraph();
}

void drawBlock(block Block, int index, int top, int left, int right, bool showText, int color) {
    delay(1000);
    if (Block.lineType == 1) drawIfBlock(Block, top, left, getIfElseBlockSize(Block), showText);
    else if (Block.lineType == 3) drawLoopTestBefore(Block, top, left, getBlockSize(Block), showText, right);
    else if (Block.lineType == 4) drawLoopTestAfter(Block, top, left, getBlockSize(Block), showText, right);
    else if (Block.lineType == 6) drawForLoop(Block, top, left, getBlockSize(Block), showText, right);
    else if (Block.lineType == 0) drawSimpleBlock(Block, top, left, color);
}
