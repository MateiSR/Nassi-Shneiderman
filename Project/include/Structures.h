#pragma once


// Variables
const int WIDTH = 1200, HEIGHT = 900;
const int MAX_WIDTH = WIDTH * 0.95, MAX_HEIGHT = HEIGHT * 0.95;
int DIAGRAM_SIZE = 0;

struct {
    int DEFAULT_COLOR = COLOR(255,255,255),
    IF_COLOR = COLOR(255, 0, 0),
    TEST_BEFORE_COLOR = COLOR(255,255,200),
    TEST_AFTER_COLOR = COLOR(200,200,0),
    FOR_COLOR = COLOR(173,216,230),
    SIMPLE_COLOR = COLOR(200,200,200),
    BG_COLOR = 10,
    MAIN_MENU_COLOR = COLOR(51,136,255);
} DEFAULT_COLORS;

const int COLOR_LIST[] = {
        COLOR(255, 0, 0),    // Red
        COLOR(0, 255, 0),    // Green
        COLOR(0, 0, 255),    // Blue
        COLOR(255, 255, 0),  // Yellow
        COLOR(255, 0, 255),  // Magenta
        COLOR(0, 255, 255),  // Cyan
        COLOR(128, 0, 128),  // Purple
        COLOR(255, 165, 0),  // Orange
        COLOR(0, 128, 0),    // Dark Green
        COLOR(128, 0, 0),    // Maroon
        0
    };

// Block colors
int DEFAULT_COLOR = COLOR(255,255,255),
    IF_COLOR = COLOR(255, 0, 0),
    TEST_BEFORE_COLOR = COLOR(255,255,200),
    TEST_AFTER_COLOR = COLOR(200,200,0),
    FOR_COLOR = COLOR(173,216,230),
    SIMPLE_COLOR = COLOR(200,200,200),
    BG_COLOR = 10,
    MAIN_MENU_COLOR = COLOR(51,136,255);

// Tipuri de statement-uri posibile in pseudocod
enum LineType
{
    invalidStatement = -1,
    otherStatement = 0,
    ifStatement = 1,
    elseStatement = 2,
    whileStatement = 3,
    repeatUntilStatementBegin = 4,
    repeatUntilStatementEnd = 5,
    forStatement = 6,
    braceBeggining = 7,
    braceEnd = 8
} ;

enum pages {
    mainPage = 0,
    diagramPage = 1,
    syntaxRulesPage = 2,
    editorPage = 3,
    colorPickerPage = 4
};

// Button struct
struct button {
    int x, y;
    int width, height;
    char *label;
    enum pages page;
} buttonList[101];
int buttonCount = 0;

// Obtine tipul liniei curente
LineType getLineType(char codeLine[])
{
    if(strstr(codeLine,"if (")) return ifStatement;
    else if(strstr(codeLine,"else")) return elseStatement;
    else if(strstr(codeLine,"while (")) return whileStatement;
    else if(strstr(codeLine,"repeat")) return repeatUntilStatementBegin;
    else if(strstr(codeLine,"until (")) return repeatUntilStatementEnd;
    else if(strstr(codeLine,"for (")) return forStatement;
    else if(strstr(codeLine,"{")) return braceBeggining;
    else if(strstr(codeLine,"}")) return braceEnd;
    else return otherStatement;
}

// Block struct
struct block
{
    int index;
    int priority;
    int lineNum;
    char rawLine[101];
    char rawInstruction[101];
    struct {unsigned int indexes[101], num = 0;} children;
    LineType lineType;
};

struct blockChain
{
    block Block[101];
    unsigned int blockCount=0;
} blockVector;


