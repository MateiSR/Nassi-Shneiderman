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
#include "include/Functions.h"

using namespace std;

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
    for (int i = 1; i <= blockVector.blockCount; i++)
        printf("i=%d, index=%d\n", i, blockVector.Block[i].index);
}
