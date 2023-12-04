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
/* Definitions */
#define INPUT_FILE "input.txt"

using namespace std;

/* Function headers */
void processFile(FILE *pseudocode);
void run();

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
} lineType;

// Obtine tipul liniei curente
LineType getLineType(char codeLine[])
{
    if(strstr(codeLine,"if")) return ifStatement;
    else if(strstr(codeLine,"else")) return elseStatement;
    else if(strstr(codeLine,"while")) return whileStatement;
    else if(strstr(codeLine,"repeat")) return repeatUntilStatementBegin;
    else if(strstr(codeLine,"until")) return repeatUntilStatementEnd;
    else if(strstr(codeLine,"for")) return forStatement;
    else if(strstr(codeLine,"{")) return braceBeggining;
    else if(strstr(codeLine,"}")) return braceEnd;
    else return otherStatement;
}

// Ia instructiune din parantezele unui statement
void getInstruction (char rawCodeLine[], char rawInstruction[]) {
    strcpy(rawInstruction, rawCodeLine);
    char *firstP = strchr(rawInstruction, '(');
    // Daca nu e instructiune valida, return
    if (!firstP) {
        rawInstruction[strlen(rawInstruction) - 1] = '\0'; // Se elimina '\n'
        return;
    }
    strcpy(rawInstruction, firstP + 1);
    // Se elimina ultima paranteza
    rawInstruction[strlen(rawInstruction) - 2] = '\0';
}

// Parcurge codul linie cu linie si stabileste tipul de statement
void analyzeCode(FILE *fptr, char rawCode[])
{
    unsigned short i = 0;
    char rawInstruction[61] = "\0";
    while(fgets(rawCode,61,fptr)){
        lineType=getLineType(rawCode);
        getInstruction(rawCode, rawInstruction);
        cout<<i++ << " (Type: " << lineType << ") " << rawInstruction << '\n';
    }

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

void processFile(FILE *pseudocode) {
    char rawCode[61];
    analyzeCode(pseudocode, rawCode);
}

void run() {
    createWindow();
}
