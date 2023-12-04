/* Libraries */
#include <stdio.h>
#include <graphics.h>
#include <winbgim.h>
#include <string.h>

/* Header files */
#include <UI.h>

/* Constant variables */
const int maxHeight = getmaxheight() * 0.75, maxWidth = getmaxwidth() * 0.75;

/* Function headers */
void run();


int main()
{
    FILE *pseudocode = fopen("input.txt", "r");
    if (!pseudocode) { printf("Could not open file"); return -1; }

    // Run process
    run();

    return 0;
}

void run() {
    createWindow();
}
