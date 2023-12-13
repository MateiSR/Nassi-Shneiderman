/* Librarii predefinite */
#include <iostream>
#include <stdio.h>
#include <graphics.h>
#include <winbgim.h>
#include <stdio.h>
#include <string.h>
/* Header files */
#include "include/UI.h"
#include "include/Constants.h"
#include "include/Structures.h"

using namespace std;

/* Function headers */
void processFile(FILE *pseudocode);
void run();

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
    // Se elimina ultima paranteza
    rawInstruction[strlen(rawInstruction) - 2] = '\0';
}
void addBlock(block newBlock)
{
    blockVector.blockCount++;
    blockVector.Block[blockVector.blockCount]=newBlock;
    blockVector.Block[blockVector.blockCount].index = blockVector.blockCount;
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
    cout<<"Number of blocks: "<<blockVector.blockCount<<'\n';
        for(int i=1; i<=blockVector.blockCount; i++)
        {
            cout<< "Block number "<<i<<": "<<blockVector.Block[i].rawLine<<", of Type: "<<blockVector.Block[i].lineType<<", situated at line: "<<blockVector.Block[i].lineNum<<" has priority: "<<blockVector.Block[i].priority<<'\n';
        }
}

void deleteAllBlocks()
{
    blockVector.blockCount=0;
}

int main()
{
    FILE *pseudocode = fopen(INPUT_FILE, "r");
    if (!pseudocode)
    {
        printf("Could not open input code file");
        return -1;
    }

    // Process input file
    processFile(pseudocode);
    fclose(pseudocode);

    run();

    return 0;
}

void processFile(FILE *pseudocode)
{
    char rawCode[61];
    analyzeCode(pseudocode, rawCode);
}

void run()
{
    createWindow(blockVector);
}

