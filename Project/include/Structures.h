#pragma once

// Tipuri de statement-uri posibile in pseudocod
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
} ;

// Block struct
struct block
{
    int lineNum;
    char rawLine[101];
    char rawInstruction[101];
    LineType lineType;
};

struct blockChain
{
    block Block[101];
    unsigned int blockCount=0;
} blockVector;
