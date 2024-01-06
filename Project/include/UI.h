#include <graphics.h>
#include <winbgim.h>
#include "Constants.h"
#include "Structures.h"
#include "Files.h"
#include "Functions.h"
#include <iostream> // remove later
#define TITLE "Diagrame Nassi-Shneiderman"
#pragma once

// Variables
const int WIDTH = 900, HEIGHT = 900;
const int MAX_WIDTH = WIDTH * 0.95, MAX_HEIGHT = HEIGHT * 0.95;
int DIAGRAM_SIZE = 0;


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
    FOR_COLOR = COLOR(173,216,230),
    SIMPLE_COLOR = COLOR(255,255,255);

const int getBlockColor(int lineType) {
    if (lineType == 0) return SIMPLE_COLOR;
    if (lineType == 1) return IF_COLOR;
    if (lineType == 3) return TEST_BEFORE_COLOR;
    if (lineType == 4) return TEST_AFTER_COLOR;
    if (lineType == 6) return FOR_COLOR;
    return DEFAULT_COLOR;
}

// Choose function to use based on lineType
//void drawBlock(block Block, int index, int top, int left, int right = MAX_WIDTH, bool showText = false);
void drawBlock(block Block, int &currTop, int &currLeft, int currRight);

// Block was visited and drawn
bool visited[101] = {false};

// show the code from block
void showCodeFromBlock(block Block, int x, int y, int leftBorder) {
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
                //line(leftBorder, y, MAX_WIDTH, y);
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

const int BG_COLOR = COLOR(45,45,45);

void generateWindowContent() {

    setbkcolor(BG_COLOR);
    setfillstyle(SOLID_FILL, BG_COLOR);
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
void drawSimpleBlock(block Block, int &top, int &left, int right) {
    int originalColor = getcolor();
    setcolor(getBlockColor(Block.lineType));
    int blockSize = textheight(Block.rawLine);
    int bottom = top + blockSize;
    printf("!%d\n", bottom);
    line(left, bottom + SPACE_UNDER_TEXT, right, bottom + SPACE_UNDER_TEXT);
    outtextxy((left + right)/2, bottom, Block.rawLine);
    printf("showing text at %d\n", bottom);
    top = bottom + SPACE_UNDER_TEXT;
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

    outtextxy(left + textwidth("TRUE") / 2 + 10, bottom - 10, "TRUE");
    outtextxy(right - textwidth("FALSE") / 2 - 10, bottom - 10, "FALSE");
    outtextxy(center, top + textheight(condition), condition);

}

// Draw the parallel block of the if
void drawIfParallelBlock(block Block, int top, int left, int blockSize = 200, bool showText = false) {
    int bottom = top + blockSize,
        right = MAX_WIDTH;
    rectangle(left, top, right, bottom);
    int center = (left+right)/2;
    line(center, top, center, bottom);
    if (showText) showCodeFromBlock(Block, left, top,left);
    // Find next else block
    bool foundElse = false;
    int i = Block.index + 1;
    while (!foundElse) {
        if (blockVector.Block[i].lineType == 2) {
            foundElse = true;
            block elseBlock = blockVector.Block[i];
            if (showText) showCodeFromBlock(elseBlock, center, top,left);
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
    char* condition = Block.rawInstruction;
    const int ifBlockSize = max((int)(blockSize / 3.5), 50);

    drawIfStartBlock(condition, top, left, ifBlockSize);
    drawIfParallelBlock(Block, top + ifBlockSize, left, blockSize, showText);
}

// --- For, while, do while (<=> repeat until !) ---

// Draw for loop in NS diagram
//void drawForLoop(char *condition, char *codeBlock, int top, int left, int blockSize = 200) {
// Takes lineType = 6
/*void drawForLoop(block Block, int top, int left, int blockSize = 200, bool showText = false, int right = MAX_WIDTH) {
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
    if (showText) showCodeFromBlock(Block, left, codeBlockTop, left+textheight(condition));
}*/

int drawForLoop(block Block, int& top, int& left, int right) {
    int blockHeight = getBlockSize(Block);
    int bottom = top + blockHeight;
    int originalColor = getcolor();
    setcolor(getBlockColor(Block.lineType));
    printf("drawing for: bottom is %d\n", bottom);

    rectangle(left, top, right, bottom);
    top += textheight(Block.rawLine) + SPACE_UNDER_TEXT;
    left += textheight(Block.rawLine);
    bottom -= textheight(Block.rawLine) + SPACE_UNDER_TEXT;
    rectangle(left, top, right, bottom);
    outtextxy((left + right) / 2, top - SPACE_UNDER_TEXT, Block.rawInstruction);
    setcolor(originalColor);
    return bottom;
}

// Draw loop with initial test (eg. while) in NS diagram
/*
void drawLoopTestBefore(block Block, int top, int left, int blockSize = 200, bool showText = false, int right = MAX_WIDTH) {
    char* condition = Block.rawInstruction;
    int bottom = top + blockSize;
    rectangle(left, top, right, bottom);
    rectangle(left + textheight(condition), top + textheight(condition), right, bottom);
    int center = (left+right)/2;
    outtextxy(center, top + textheight(condition), condition);
    if (showText) showCodeFromBlock(Block, left, top + textheight(condition), left + textheight(condition));
}
*/

void drawLoopTestBefore(block Block, int &top, int &left, int right) {
    int blockHeight = getBlockSize(Block);
    int bottom = top + blockHeight;
    int originalColor = getcolor();
    setcolor(getBlockColor(Block.lineType));
    printf("drawing while: bottom is %d\n", bottom);
    rectangle(left, top, right, bottom);

    int middle = (left + right) / 2;
    outtextxy(middle, top + textheight(Block.rawInstruction), Block.rawInstruction);

    left += textheight(Block.rawInstruction);
    top += textheight(Block.rawInstruction) + SPACE_UNDER_TEXT;

    rectangle(left, top, right, bottom);
    setcolor(originalColor);
}

// Draw loop with final test(eg do while) in NS diagram
// Takes lineType = 4
/*void drawLoopTestAfter(block Block, int top, int left, int blockSize = 200, bool showText = false, int right = MAX_WIDTH) {
    //printf("----%d---%d----\n", showText, Block.lineType);
    //setcolor(4);
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
    if (showText) showCodeFromBlock(Block, left, top, left + textheight(condition));
    //setcolor(15);
}*/

int drawLoopTestAfter(block Block, int& top, int& left, int right) {
    int originalColor = getcolor();
    setcolor(getBlockColor(Block.lineType));
    int blockHeight = getBlockSize(Block);
    int bottom = top + blockHeight;

    left += textheight(Block.rawLine);
    rectangle(left, top, right, bottom); cout << "am desenat\n";
    left -= textheight(Block.rawLine);
    bottom += textheight(Block.rawLine) + SPACE_UNDER_TEXT;
    rectangle(left, top, right, bottom); cout << "am desenat din nou\n";
    left += textheight(Block.rawLine);
    setcolor(originalColor);
    return bottom;
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

// Get child coordinates
/*
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
*/

void createDiagram(blockChain blockVector, int currTop = MAX_HEIGHT * 0.05) {
    int originTop = currTop;
    int diagramLeft = MAX_WIDTH * 0.05;
    int diagramRight = MAX_WIDTH * 0.95;

    int currLeft = diagramLeft;
    int currRight = MAX_WIDTH;
    int currBottom;

    int lastPriority = -1;

    int oldTop[11] = {-1};
    int forPriority[11] = {-1};
    int last = 0;

    lastPriority = blockVector.Block[1].priority;
    for (int currIndex = 1; currIndex <= blockVector.blockCount; currIndex++) {
        //delay(1000);
        if (blockVector.Block[currIndex].priority < lastPriority) {
            currLeft -= textheight(blockVector.Block[currIndex].rawLine) * (lastPriority - blockVector.Block[currIndex].priority);
        }
        // PARTE LEGATA DE FOR-URI
        lastPriority = blockVector.Block[currIndex].priority;
        // daca blocul e for, ii bagam prioritatea si valoarea top in stiva de valori
        if (blockVector.Block[currIndex].lineType == 6 || blockVector.Block[currIndex].lineType == 4) {
            if (blockVector.Block[currIndex].lineType == 4) oldTop[++last] = drawLoopTestAfter(blockVector.Block[currIndex], currTop, currLeft, currRight);
            else oldTop[++last] = drawForLoop(blockVector.Block[currIndex], currTop, currLeft, currRight);
            forPriority[last] = blockVector.Block[currIndex].priority;
        }
        // daca iesim din for, verificam cate for-uri s-au inchis de cand nu am mai scazut prioritatea (ex: 2 foruri se inchid simultan,
        // va trb sa luam top-ul de la cel de-al doilea for.
        else if (forPriority[last] >= blockVector.Block[currIndex].priority) {
            while (forPriority[last] > blockVector.Block[currIndex].priority) {
                oldTop[last] = -1;
                forPriority[last] = -1;
                last--;
            }
            currTop = oldTop[last] + textheight(blockVector.Block[currIndex].rawLine) + SPACE_UNDER_TEXT;
            oldTop[last] = -1;
            forPriority[last] = -1;
            last--;
        }

        // PARTE LEGATA DE REPEAT UNTIL-URI

        // trebuie bagat sistem similar cu for-uri : o stiva de prioritati, ceva in genul/
        // propun sa tratam until-ul de la sfarsit ca un type 0? parca simt ca se poate face cv cu ideea asta. si doar sa afisezi la sfarsit
        // ca e ciudat ca tu desenezi odata cu repeat-ul si trb apoi sa astepti until-ul
        // in fine

        // PARTE LEGATA DE BLOCURI SIMPLE SI WHILE-URI
        if (blockVector.Block[currIndex].lineType!=6) drawBlock(blockVector.Block[currIndex],currTop,currLeft,currRight);
    }
    DIAGRAM_SIZE = currTop - originTop;
    printf("\nFinal top:%d, Diagram size: %d\n", currTop, DIAGRAM_SIZE);

    // For scroll function, when it goes out of bounds (out of diagramBorder)
    // Fill area above TOP border
    setfillstyle(SOLID_FILL, BG_COLOR);
    bar(0, 0, WIDTH, MAX_HEIGHT * 0.05);
    // Fill area above BOTTOM border
    bar(0, MAX_HEIGHT, WIDTH, HEIGHT);

    drawDiagramBorder();

}

/* Now done in main:run()
void createWindow(blockChain blockVector, int top = MAX_HEIGHT * 0.05) {
    initwindow(WIDTH, HEIGHT);
    setWindowTitle(TITLE);
    generateWindowContent();
    createDiagram(blockVector, top);
    //getch(); // Keep window open
    //closegraph();
}
*/

void drawBlock(block Block, int &currTop, int &currLeft, int currRight) {
    if (Block.lineType == 0) drawSimpleBlock(Block, currTop, currLeft, currRight);
    else if(Block.lineType == 3) drawLoopTestBefore(Block,currTop,currLeft,currRight);
}

/*void drawBlock(block Block, int index, int top, int left, int right, bool showText) {
    delay(1000);
    if (Block.lineType == 1) drawIfBlock(Block, top, left, getBlockSize(Block), showText);
    else if (Block.lineType == 3) drawLoopTestBefore(Block, top, left, getBlockSize(Block), showText, right);
    else if (Block.lineType == 4) drawLoopTestAfter(Block, top, left, getBlockSize(Block), showText, right);
    else if (Block.lineType == 6) drawForLoop(Block, top, left, getBlockSize(Block), showText, right);
    else if (Block.lineType == 0) drawSimpleBlock(Block, top, left, right);
}
*/
