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
    rectangle(left, top, right, bottom + SPACE_UNDER_TEXT);
    outtextxy((left + right)/2, bottom, Block.rawInstruction);
    top = bottom + SPACE_UNDER_TEXT;
    printf("showing text: %s at %d, block id %d, y=%d\n", Block.rawInstruction, bottom, Block.index, top);
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


int drawLoopTestAfter(block Block, int& top, int& left, int right) {
    int originalColor = getcolor();
    setcolor(getBlockColor(Block.lineType));
    int blockHeight = getBlockSize(Block);
    // calculam bottom pt dreptunghiul MIC!! (si leftul tot ptr ala)
    int bottom = top + blockHeight - (textheight(Block.rawLine) + SPACE_UNDER_TEXT);
    left += textheight(Block.rawLine);
    rectangle(left, top, right, bottom); cout << "am desenat\n";

    //apoi calculam bottom si left pt dreptunghiul ala mare !!
    left -= textheight(Block.rawLine);
    bottom += textheight(Block.rawLine) + SPACE_UNDER_TEXT;
    rectangle(left, top, right, bottom); cout << "am desenat din nou\n";

    // si la sfarsit le ducem inapoi la valoarea de la dreptunghiul mic ca sa ramana left-ul cum trb si sa mearga si bottom.
    left += textheight(Block.rawLine);
    bottom -= textheight(Block.rawLine) + SPACE_UNDER_TEXT;
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

void drawIf(block Block, int &top, int &left, int &right) {
    int blockHeight = getBlockSize(Block);
    int bottom = top + (textheight(Block.rawLine) + SPACE_UNDER_TEXT) * 2;
    rectangle(left, top, right, bottom);
    line(left, top, (left + right) / 2, bottom);
    line(right, top, (left + right) / 2, bottom);

    outtextxy((left + right) / 2, top + textheight(Block.rawLine) + SPACE_UNDER_TEXT, Block.rawInstruction);

    // scoate textul la mijlocul dintre left si (left+right)/2, respectiv (left + right)/2 si right. de acolo vine formula ciudata.
    outtextxy((3 * left + right) / 4, bottom - SPACE_UNDER_TEXT, "TRUE");
    outtextxy((left + 3 * right) / 4, bottom - SPACE_UNDER_TEXT, "FALSE");

    right = (left + right) / 2;
    top = bottom;
}

void createDiagram(blockChain blockVector, int currTop = MAX_HEIGHT * 0.05) {
    int originTop = currTop;
    int diagramLeft = MAX_WIDTH * 0.05;
    int diagramRight = MAX_WIDTH * 0.95;

    int currLeft = diagramLeft;
    int currRight = MAX_WIDTH;
    int currBottom;

    int lastPriority = -1;

    int oldTop[11] = { -1 };
    int loopPriority[11] = { -1 };
    int blockType[11] = { -1 };
    int lastLoop = 0;

    bool inIf = false;
    bool ifEnded = false;
    //bool elseEnded = false;

    int oldRight[11] = { -1 };
    int ifPriority[11] = { -1 };
    int ifHeight[11] = { -1 };
    int elseHeight[11] = { -1 };
    int oldLeft[11] = { -1 };
    int lastIf = 0;

    // trebuie schimbat cum calculeaza functia de inaltime pe if
    // if-urile si for-urile nu se plac deloc

    lastPriority = blockVector.Block[1].priority;
    for (int currIndex = 1; currIndex <= blockVector.blockCount; currIndex++) {
        //delay(1000);
        if ((blockVector.Block[currIndex].priority <= ifPriority[lastIf]) && inIf) {
            ifEnded = true;
            if (blockVector.Block[currIndex].lineType != 2) { // nu e niciun else
                inIf = false;
                while (ifPriority[lastIf] > blockVector.Block[currIndex].priority) {
                    oldRight[lastIf] = -1;
                    ifPriority[lastIf] = -1;
                    ifHeight[lastIf] = -1;
                    lastIf--;
                }
                currLeft = oldLeft[lastIf];
                currRight = oldRight[lastIf];
                if (elseHeight[lastIf] < ifHeight[lastIf] && elseHeight[lastIf] !=-1) {
                    currTop = currTop + (ifHeight[lastIf] - elseHeight[lastIf]);
                }
                oldRight[lastIf] = -1;
                oldLeft[lastIf] = -1;
                ifPriority[lastIf] = -1;
                ifHeight[lastIf] = -1;
                elseHeight[lastIf] = -1;
                lastIf--;
            }
            else { // este un else pt if-ul curent
                while (ifPriority[lastIf] > blockVector.Block[currIndex].priority) {
                    oldRight[lastIf] = -1;
                    ifPriority[lastIf] = -1;
                    lastIf--;
                }
                currLeft = currRight;
                currRight = oldRight[lastIf];
                printf("if height of last if is %d, currTop=%d\n", ifHeight[lastIf], currTop);
                currTop = currTop - ifHeight[lastIf];
                elseHeight[lastIf] = getBlockSize(blockVector.Block[currIndex]) - (SPACE_UNDER_TEXT + textheight(blockVector.Block[currIndex].rawLine));
                // elseEnded, top -= elseHeight[lastIf], top += max(elseHeight, ifHeight)
            }
        }
        if (blockVector.Block[currIndex].priority < lastPriority && !ifEnded) {
            currLeft -= textheight(blockVector.Block[currIndex].rawLine) * (lastPriority - blockVector.Block[currIndex].priority);
        }
        // daca ultimul statement a fost else, inIf = false si current
        //if (blockVector.Block[currIndex - 1].lineType == 2) elseEnded = true;
        if (blockVector.Block[currIndex].priority < lastPriority && ifEnded) {
            currTop -= elseHeight[lastIf];
            currTop += max(elseHeight[lastIf], ifHeight[lastIf]);
        }
        ifEnded = false;
        if (blockVector.Block[currIndex].lineType == 1)
        {
            inIf = true;
            oldRight[++lastIf] = currRight;
            oldLeft[lastIf] = currLeft;
            ifPriority[lastIf] = blockVector.Block[currIndex].priority;
            ifHeight[lastIf] = getBlockSize(blockVector.Block[currIndex]) - (SPACE_UNDER_TEXT + textheight(blockVector.Block[currIndex].rawLine));
        }
        // PARTE LEGATA DE FOR-URI SI REPEAT UNTIL-URI
        lastPriority = blockVector.Block[currIndex].priority;
        // daca blocul e for, ii bagam prioritatea si valoarea top in stiva de valori
        if (blockVector.Block[currIndex].lineType == 6 || blockVector.Block[currIndex].lineType == 4) {
            if (blockVector.Block[currIndex].lineType == 4) {
                oldTop[++lastLoop] = drawLoopTestAfter(blockVector.Block[currIndex], currTop, currLeft, currRight);
                blockType[lastLoop] = 0;
            }
            else {
                oldTop[++lastLoop] = drawForLoop(blockVector.Block[currIndex], currTop, currLeft, currRight);
                blockType[lastLoop] = 1;
            }
            loopPriority[lastLoop] = blockVector.Block[currIndex].priority;
        }
        // daca iesim din for, verificam cate for-uri s-au inchis de cand nu am mai scazut prioritatea (ex: 2 foruri se inchid simultan,
        // va trb sa luam top-ul de la cel de-al doilea for.
    else if (loopPriority[lastLoop] >= blockVector.Block[currIndex].priority) {
            while (loopPriority[lastLoop] > blockVector.Block[currIndex].priority && lastLoop > 1) {
                oldTop[lastLoop] = -1;
                loopPriority[lastLoop] = -1;
                lastLoop--;
            }
            if (blockVector.Block[currIndex].lineType != 2) {
                currTop = oldTop[lastLoop] + (textheight(blockVector.Block[currIndex].rawLine) + SPACE_UNDER_TEXT) * blockType[lastLoop];
            }
            else{
                //daca un for e ultima chestie dintr-un if, atunci nu actualizam top-ul ca deobicei.
                currTop += textheight(blockVector.Block[currIndex].rawLine) + SPACE_UNDER_TEXT;
            }
            oldTop[lastLoop] = -1;
            loopPriority[lastLoop] = -1;
            blockType[lastLoop] = -1;
            lastLoop--;
        }

        cout << '\n' << currTop << "current top \n";
        if (blockVector.Block[currIndex].lineType == 1) drawIf(blockVector.Block[currIndex], currTop, currLeft, currRight);
        // PARTE LEGATA DE BLOCURI SIMPLE SI WHILE-URI
        if (blockVector.Block[currIndex].lineType!=6 && blockVector.Block[currIndex].lineType != 4) drawBlock(blockVector.Block[currIndex],currTop,currLeft,currRight);
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
    if (Block.lineType == 0 || Block.lineType == 5) drawSimpleBlock(Block, currTop, currLeft, currRight);
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
