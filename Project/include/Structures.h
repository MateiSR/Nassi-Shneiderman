#pragma once


// Variables
const int WIDTH = 1200, HEIGHT = 900;
const int MAX_WIDTH = WIDTH * 0.95, MAX_HEIGHT = HEIGHT * 0.95;
int DIAGRAM_SIZE = 0;

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
    editorPage = 3
};

// Button struct
struct button {
    int x, y;
    int width, height;
    char *label;
    enum pages page;
} buttonList[11];
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


