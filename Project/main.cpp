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
    int selectedColor, selectedBtnId;
    int y = MAX_HEIGHT * 0.05;
    int left = MAX_WIDTH * 0.05, right = MAX_WIDTH * 0.95;
    //printf("\n\n\n%d\n\n\n", getBlockSize(blockVector.Block[1]));
    const int diagramHeight = getDiagramHeight(blockVector);
    const int diagramLeft = MAX_WIDTH * 0.05;
    const int diagramRight = MAX_WIDTH * 0.95;
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
        printf("---\nCurrent zoom is ZOOM=%.3f\n---\n", ZOOM);
        generateWindowContent();
        createDiagram(blockVector, y, left, right);
        // Double buffering to avoid screen tearing
        swapbuffers();
        ch = getch();
        printf("got character %c - ascii: %d\n", (char)ch, ch);
        if (ch == 27 || ch == 13) {
            currentPage = mainPage;
            y = MAX_HEIGHT * 0.05;
            ZOOM = ZOOM_DEFAULT;
            left = MAX_WIDTH * 0.05, right = MAX_WIDTH * 0.95;
        } //running = false; // Esc button to exit
        else if (tolower(ch) == 'i') ZOOM += ZOOM_INCREMENT;
        else if (tolower(ch) == 'o') {if(ZOOM > ZOOM_DEFAULT) ZOOM -= ZOOM_INCREMENT;}
        else {
            ch = getch();
            printf("got 2nd character %c - ascii: %d\n", (char)ch, ch);
            switch (ch) {
            case 72: // up
                //if (y - SCROLL_UNIT >= top_border)
                y += SCROLL_UNIT;
                break;
            case 80: // down
                cout << y + DIAGRAM_SIZE << '\n';
                //if (y + SCROLL_UNIT <= bottom_border)
                y -= SCROLL_UNIT;
                break;
            case 75: // left
                left += SCROLL_UNIT;
                right -= SCROLL_UNIT;
                break;
            case 77: // right
                right += SCROLL_UNIT;
                left -= SCROLL_UNIT;
                break;
/*            default:
                running = false;
                break;*/
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
                                    else if (currBtn == 5) {
                                        printf("-------Running: Color picker page------\n\n");
                                        selectedColor = -1;
                                        selectedBtnId = -1;
                                        currentPage = colorPickerPage;
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
        else if (currentPage == colorPickerPage) {
            runColorPickerPage(selectedColor, selectedBtnId);
            drawButtons(buttonList, buttonCount, colorPickerPage);
            swapbuffers();
            if (ismouseclick(WM_LBUTTONDOWN)) { // Handle mouse input;
                    int mouseX, mouseY;
                    int buttonsBeforeCurrPage = 16;
                    getmouseclick(WM_LBUTTONDOWN, mouseX, mouseY);
                    printf("Got mouse click at x=%d, y=%d\n", mouseX, mouseY);
                    for (int currBtn = 1; currBtn <= buttonCount; currBtn++) {
                        button btn = buttonList[currBtn];
                        //if (buttonList[currBtn].page != currentPage) buttonsBeforeCurrPage++;
                        if (buttonList[currBtn].page == currentPage) {
                        if (!strlen(btn.label)) { // este culoarea selectabila
                            if (isInsideButton(mouseX, mouseY,
                                btn.x, btn.y, btn.width, btn.height)) {
                            int colorIndex = currBtn - buttonsBeforeCurrPage;
                            printf("Clicked button id %d with assigned color %d\n\n", currBtn, colorIndex);
                            selectedColor = colorIndex;
                        }
                        }
                        else
                            if (isInsideButton(mouseX, mouseY,
                                btn.x - btn.width / 2, btn.y - btn.height / 2, btn.width, btn.height)) {
                                    printf("Clicked button id %d\n", currBtn);
                                    if (strcmp(btn.label, "Confirm") == 0) {
                                            if (selectedColor == -1 || selectedBtnId == -1) Beep(200, 200);
                                            else {
                                            changeColor(selectedColor, selectedBtnId);
                                            selectedColor = selectedBtnId = -1;
                                            }
                                    }
                                    else if (strcmp(btn.label, "Reset") == 0) {
                                        resetColors();
                                        currentPage = mainPage;
                                    }
                                    else selectedBtnId = currBtn - 6;
                                    if (strcmp(btn.label, "Exit") == 0) currentPage = mainPage;

                                }
                        }
                    }
                }
                delay(10);
        }
        cleardevice();
    }
    closegraph();
}
