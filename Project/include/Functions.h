#pragma once
#include "Structures.h"
#include <iostream>
using namespace std;

/* Function headers */
void processFile(FILE *pseudocode);
void run();

float ZOOM = 1.0;
int SPACE_UNDER_TEXT = 5;

// Is drawable block
bool isDrawableBlock(block Block) {
    const int validLineTypes[] = {1,3,4,6};
    for (int type=0;type<4;type++)
        if (Block.lineType == type) return true;
    return false;
}

// Return how many children it has
int findChildren(block Block) {
    //printf("Saving children for block id %d\n", Block.index);
    int i = Block.index + 1;
    int j = Block.index;
    int lastPrio = Block.priority;
    int cnt = 0;
    if (!isDrawableBlock(Block) && Block.lineType != 2) return 0;
    while (i <= blockVector.blockCount) {
        if (blockVector.Block[i].priority > lastPrio) {
                cnt++;
                blockVector.Block[j].children.num++;
                blockVector.Block[j].children.indexes[blockVector.Block[j].children.num] = blockVector.Block[i].index;
                //printf("----%d---cnt=%d-\n", blockVector.Block[j].children.num, cnt);

        }
        else if (blockVector.Block[i].priority == lastPrio) break;
        i++;
    }
    return cnt;
}

// Get blockSize (height) needed
/*int getBlockSize(block Block) {
    if (Block.lineType == 0) return textheight(Block.rawLine);
    FILE *in = fopen(INPUT_FILE, "r");
    goToLine(in, Block.lineNum);
    int instSize = 0;
    int y = 0;
    char buffer[61];
    bool done = false;
    bool started = false;
    while (!done && fgets(buffer, 61, in)) {
        if (!instSize) instSize = textheight(buffer);
        int lineType = getLineType(buffer);
        if (lineType == 7) continue;

        if ((lineType == 8 && Block.lineType != 4) || lineType == 5) {
            done = true;
        }
        y += textheight(buffer);
    }
    fclose(in);
    bool allZeroes = true;
    for (int k = 1; k <= Block.children.num; k++) {
            int kInd = Block.children.indexes[k];
            block kBlock = blockVector.Block[kInd];
            if (kBlock.lineType != 0 && kBlock.lineType != 5) allZeroes = false;
        }
    if (!Block.children.num || allZeroes) return y;
    // else go through children and get total sum of blocksizes
    else {
        int sum = 0;
        for (int j = 1; j <= Block.children.num; j++) {
            block currentChild = blockVector.Block[Block.children.indexes[j]];
            sum += getBlockSize(currentChild);
        }
    return sum + instSize;
    }
}*/
int getBlockSize(block Block);

int getIfSize(int index) {
    printf("Finding if size for block id %d\n", index);
    int blockCount = 1;
    int blockSizeIf = getBlockSize(blockVector.Block[index]);
    int blockSizeElse = 0;
    for (int i = index; i <= blockVector.blockCount; i++) {
        if (blockVector.Block[i].lineType == 2) {
            blockSizeElse = getBlockSize(blockVector.Block[i]);
            printf("Found else for if id %d at block id %d\n", index, i);
        }
    }
    printf("Block size else: %d, Block size if: %d\n", blockSizeElse, blockSizeIf);
    return max(blockSizeElse, blockSizeIf);
}

int getBlockSize(block Block) {
    SPACE_UNDER_TEXT = 5 * ZOOM;
    if (Block.lineType == invalidStatement) return 100;
    int blockSize = textheight(Block.rawInstruction) + SPACE_UNDER_TEXT;
    //blocurile de for au nevoie de spatiu suplimentar/
    if (Block.lineType == 6) blockSize += textheight(Block.rawInstruction) + SPACE_UNDER_TEXT;
    if (Block.lineType == 0) return blockSize + SPACE_UNDER_TEXT;
    else {
        int startingPriority = Block.priority;
        for (int i = Block.index + 1; i <= blockVector.blockCount; i++) {
            if (startingPriority >= blockVector.Block[i].priority) break;
            if (blockVector.Block[i].lineType == 6) blockSize += textheight(Block.rawInstruction) + SPACE_UNDER_TEXT;
            if (blockVector.Block[i].lineType == 1) {
                blockSize += getIfSize(i);
                int ifSizePriority = blockVector.Block[i].priority;
                i++;
                while (ifSizePriority != blockVector.Block[i].priority && blockVector.Block[i].lineType != 2 && i<=blockVector.blockCount) {
                    i++;
                }
            }
            else if (blockVector.Block[i].lineType != 4) blockSize += textheight(Block.rawInstruction) + SPACE_UNDER_TEXT;
        }
    }
    cout << '\n' << blockSize << '\n';
    return blockSize;
}
//undeva, trebuie o variabila globala la care sa adaug marimea asta ^

// Ia instructiune din parantezele unui statement
void getInstruction (char rawCodeLine[], char rawInstruction[])
{
    strcpy(rawInstruction, rawCodeLine);
    char *firstP = strchr(rawInstruction, '(');
    // Daca nu e instructiune valida, return
    if (!firstP)
    {
        rawInstruction[strlen(rawInstruction) - 1] = '\0'; // Se elimina '\n'
        return;
    }
    strcpy(rawInstruction, firstP + 1);
    // Se elimina ultima paranteza si spatiile in exces.
    char* secondP = strrchr(rawInstruction, ')');
    *secondP = '\0';
}
void addBlock(block newBlock)
{
    blockVector.blockCount++;
    blockVector.Block[blockVector.blockCount]=newBlock;
    blockVector.Block[blockVector.blockCount].index = blockVector.blockCount;
}

// clean null characters (.trim())
void cleanRawCode(char *s) {
    if (!s) return;
    int len = strlen(s);
    // Trim spaces after
    while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t')) { // \t == tab
        s[--len] = '\0';
    }
    // Trim leading spaces
    int start = 0;
    while (start < len && (s[start] == ' ' || s[start] == '\t')) { // \t == tab
        ++start;
    }
    // Move char[] array back
    if (start > 0) {
        for (int i = 0; i < len - start + 1; ++i) {
            s[i] = s[start + i];
        }
    }
}

// Parcurge codul linie cu linie si stabileste tipul de statement
void analyzeCode(FILE *fptr, char rawCode[])
{
    unsigned short line = 0;
    block newBlock;
    int currPriority = 0;
    char rawInstructions[61] = "\0";
    while(fgets(rawCode,61,fptr))
    {
        //Count the line
        line++;
        //Extract the instruction from the code
        getInstruction(rawCode, rawInstructions);
        //Create new block
        newBlock.lineType=getLineType(rawCode);
        strcpy(newBlock.rawLine,rawCode);
        // clean rawCode from null characters
        cleanRawCode(newBlock.rawLine);
        strcpy(newBlock.rawInstruction,rawInstructions);
        newBlock.rawLine[strlen(newBlock.rawLine)-1]='\0';
        newBlock.lineNum=line;
        //If block is not a brace, add it to the block list
        if(newBlock.lineType==7) currPriority++;
        else if(newBlock.lineType==8) currPriority--;
        else {
            newBlock.priority=currPriority;
            addBlock(newBlock);
        }
        cout<<rawCode<<' ';
    }
    for (int i = 1; i <= blockVector.blockCount; i++) {
        blockVector.Block[i].index = i;
        cout << "block at index: " << i << " has priority: " << blockVector.Block[i].priority << " and type: "<<blockVector.Block[i].lineType
                << " with text: " << blockVector.Block[i].rawLine << '\n';
    }
}

unsigned int getDiagramHeight(blockChain blockVector) {
    unsigned int diagramHeight = 0;
    for (int i = 1; i <= blockVector.blockCount; i++) {
        printf("i=%d, priority = %d\n", i, blockVector.Block[i].priority);
        //if (blockVector.Block[i].priority == 0) diagramHeight += getBlockSize(blockVector.Block[i]);
    }
    return diagramHeight;
}

void deleteAllBlocks()
{
    blockVector.blockCount=0;
}

void addButton(int x, int y, int width, int height, char* label, enum pages page) {
    buttonCount++;
    buttonList[buttonCount].x = x;
    buttonList[buttonCount].y = y;
    buttonList[buttonCount].width = width;
    buttonList[buttonCount].height = height;
    buttonList[buttonCount].label = label;
    buttonList[buttonCount].page = page;
}

void addButtons () {
    addButton(WIDTH / 2, HEIGHT / 3, WIDTH / 4, HEIGHT / 9, "Create diagram", mainPage);
    addButton(WIDTH / 1.1, HEIGHT / 8, WIDTH / 12, HEIGHT / 9, "Exit", mainPage);
    addButton(WIDTH / 2, HEIGHT / 2, WIDTH /4 , HEIGHT / 9, "Syntax rules", mainPage);
    addButton(WIDTH / 2, HEIGHT / 1.5, WIDTH / 4, HEIGHT / 9, "Edit diagram", mainPage);
    addButton(WIDTH / 2, HEIGHT / 1.2, WIDTH / 4, HEIGHT / 9, "Color picker", mainPage);
    addButton(WIDTH / 10, HEIGHT / 1.2, WIDTH / 12 , HEIGHT / 9, "MAIN_BG", colorPickerPage);
    addButton(WIDTH / 10 + (WIDTH / 12 + WIDTH / 20) * 1, HEIGHT / 1.2, WIDTH / 12 , HEIGHT / 9, "DIAGRAM", colorPickerPage);
    addButton(WIDTH / 10 + (WIDTH / 12 + WIDTH / 20) * 2, HEIGHT / 1.2, WIDTH / 12 , HEIGHT / 9, "SIMPLE", colorPickerPage);
    addButton(WIDTH / 10 + (WIDTH / 12 + WIDTH / 20) * 3, HEIGHT / 1.2, WIDTH / 12 , HEIGHT / 9, "FOR", colorPickerPage);
    addButton(WIDTH / 10 + (WIDTH / 12 + WIDTH / 20) * 4, HEIGHT / 1.2, WIDTH / 12 , HEIGHT / 9, "WHILE", colorPickerPage);
    addButton(WIDTH / 10 + (WIDTH / 12 + WIDTH / 20) * 5, HEIGHT / 1.2, WIDTH / 12 , HEIGHT / 9, "REPEAT", colorPickerPage);
    addButton(WIDTH / 10 + (WIDTH / 12 + WIDTH / 20) * 6, HEIGHT / 1.2, WIDTH / 12 , HEIGHT / 9, "IF", colorPickerPage);
    addButton(WIDTH / 1.1, HEIGHT / 8, WIDTH / 12, HEIGHT / 9, "Exit", colorPickerPage);
    addButton(WIDTH / 3, HEIGHT / 1.05, WIDTH / 4, HEIGHT / 12, "Confirm", colorPickerPage);
    addButton(WIDTH / 3 + WIDTH / 4 + WIDTH / 20, HEIGHT / 1.05, WIDTH / 4, HEIGHT / 12, "Reset", colorPickerPage);
}

char* getColorType(int id) {
    switch (id) {
    case 0:
        return "MAIN_BG";
        break;
    case 1:
        return "DIAGRAM_BG";
        break;
    case 2:
        return "SIMPLE_BLOCK";
        break;
    case 3:
        return "FOR_BLOCK";
        break;
    case 4:
        return "WHILE_BLOCK";
        break;
    case 5:
        return "REPEAT_UNTIL_BLOCK";
        break;
    case 6:
        return "IF_BLOCK";
        break;
    default:
        return "NONE";
        break;
    }
}

void resetColors() {
    DEFAULT_COLOR = DEFAULT_COLORS.DEFAULT_COLOR;
    IF_COLOR = DEFAULT_COLORS.IF_COLOR;
    TEST_AFTER_COLOR = DEFAULT_COLORS.TEST_AFTER_COLOR;
    TEST_BEFORE_COLOR = DEFAULT_COLORS.TEST_BEFORE_COLOR;
    FOR_COLOR = DEFAULT_COLORS.FOR_COLOR;
    SIMPLE_COLOR = DEFAULT_COLORS.SIMPLE_COLOR;
    BG_COLOR = DEFAULT_COLORS.BG_COLOR;
    MAIN_MENU_COLOR = DEFAULT_COLORS.MAIN_MENU_COLOR;
}


void changeColor(int colorIndex, int blockIndex) {
    int color = COLOR_LIST[colorIndex];
    switch (blockIndex) {
    case 0:
        MAIN_MENU_COLOR = color;
        break;
    case 1:
        BG_COLOR = color;
        break;
    case 2:
        SIMPLE_COLOR = color;
        break;
    case 3:
        FOR_COLOR = color;
        break;
    case 4:
        TEST_BEFORE_COLOR = color;
        break;
    case 5:
        TEST_AFTER_COLOR = color;
        break;
    case 6:
        IF_COLOR = color;
        break;
    }
}
