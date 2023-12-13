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

// Obtine tipul liniei curente
LineType getLineType(char codeLine[])
{
    if(strstr(codeLine,"if(")) return ifStatement;
    else if(strstr(codeLine,"else")) return elseStatement;
    else if(strstr(codeLine,"while(")) return whileStatement;
    else if(strstr(codeLine,"repeat")) return repeatUntilStatementBegin;
    else if(strstr(codeLine,"until")) return repeatUntilStatementEnd;
    else if(strstr(codeLine,"for(")) return forStatement;
    else if(strstr(codeLine,"{")) return braceBeggining;
    else if(strstr(codeLine,"}")) return braceEnd;
    else return otherStatement;
}

// Block struct
struct block
{
    int priority;
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
