/* Librarii predefinite */
#include <iostream>
#include <stdio.h>
#include <graphics.h>
#include <winbgim.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <windows.h>
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

    // Create predefined buttons
    addButtons();

    // Process input file
    processFile(pseudocode);
    fclose(pseudocode);

    //runMainMenu();
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
    bool running = true;
    int y = MAX_HEIGHT * 0.05;
    //printf("\n\n\n%d\n\n\n", getBlockSize(blockVector.Block[1]));
    const int diagramHeight = getDiagramHeight(blockVector);
    const int top_border = MAX_HEIGHT * 0.05, bottom_border = MAX_HEIGHT;
    initwindow(WIDTH, HEIGHT);
    setWindowTitle(TITLE);
    generateWindowContent();
    enum pages currentPage = mainPage;
    const int SCROLL_UNIT = 25;
    for (int i = 1; i <= blockVector.blockCount; i++)
        printf("i=%d, index=%d, blockSize=%d\n", i, blockVector.Block[i].index, getBlockSize(blockVector.Block[i]));
    char ch;
    while (running) {
        if (currentPage == diagramPage) {
        printf("---\nCurrent top is at y=%d\n---\n", y);
        generateWindowContent();
        createDiagram(blockVector, y);
        // Double buffering to avoid screen tearing
        swapbuffers();
        ch = getch();
        printf("got character %c - ascii: %d\n", (char)ch, ch);
        if (ch == 27 || ch == 13) currentPage = mainPage; //running = false; // Esc button to exit
        else {
            ch = getch();
            printf("got 2nd character %c - ascii: %d\n", (char)ch, ch);
            switch (ch) {
            case 72:
                //if (y - SCROLL_UNIT >= top_border)
                y += SCROLL_UNIT;
                break;
            case 80:
                cout << y + DIAGRAM_SIZE << '\n';
                //if (y + SCROLL_UNIT <= bottom_border)
                y -= SCROLL_UNIT;
                break;
            default:
                running = false;
                break;
            }
        }
        }
        else if (currentPage == mainPage) {
                runMainMenu();
                //if (ch == 27 || ch == 13) running = false;
                swapbuffers();
                if (ismouseclick(WM_LBUTTONDOWN)) { // Handle mouse input;
                    int mouseX, mouseY;
                    getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);
                    printf("Got mouse click at x=%d, y=%d\n", mouseX, mouseY);
                    for (int currBtn = 1; currBtn <= buttonCount; currBtn++) {
                        button btn = buttonList[currBtn];
                        if (buttonList[currBtn].page == currentPage && isInsideButton(mouseX, mouseY,
                                btn.x - btn.width / 2, btn.y - btn.height / 2, btn.width, btn.height))
                                    if (currBtn == 1) {
                                    printf("-------Running diagram generation------\n\n");
                                    currentPage = diagramPage;
                                    }
                                    else if (currBtn == 2) {
                                        printf("-------Exiting due to button press------\n\n");
                                        running = false;
                                        break;
                                        exit(0);
                                    }
                                    else if (currBtn == 3) {
                                        printf("-------Running: Syntax rules------\n\n");
                                        currentPage = syntaxRulesPage;
                                    }
                                    else if (currBtn == 4) {
                                        printf("-------Running: Diagram editor------\n\n");
                                        currentPage = editorPage;
                                    }

                    }
                }
                delay(10);
        }
        else if (currentPage == syntaxRulesPage) {
            //
            runSyntaxRulesPage();
            swapbuffers();
            ch = getch();
            printf("got character %c - ascii: %d\n", (char)ch, ch);
            if (ch == 27 || ch == 13) currentPage = mainPage; //running = false; // Esc button to exit
        }
        else if (currentPage == editorPage) {
            //
            printf("Opening notepad\n");
            string command = "notepad input.txt";
                STARTUPINFO si = {};
                PROCESS_INFORMATION pi = {};
                 if (CreateProcess(NULL, const_cast<char*>(command.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                             WaitForSingleObject(pi.hProcess, INFINITE);

                            // Close process handles
                            CloseHandle(pi.hProcess);
                            CloseHandle(pi.hThread);
                            printf("Notepad has been closed\n");
                            FILE *pseudocode = fopen(INPUT_FILE, "r");
                            if (!pseudocode)
                            {
                                printf("Could not reopen input code file\n");
                            }
                            deleteAllBlocks();
                            processFile(pseudocode);
                            fclose(pseudocode);
                            currentPage = mainPage;
                 }
                 else printf("Failed to create process\n");
        }
        cleardevice();
    }
    closegraph();
}
