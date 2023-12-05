#include <graphics.h>
#include <winbgim.h>
#include "Constants.h"
#define TITLE "Diagrame Nassi-Shneiderman"

// Variables
const int WIDTH = 900, HEIGHT = 900;
const int MAX_WIDTH = WIDTH * 0.9, MAX_HEIGHT = HEIGHT * 0.9;

// Text sizes
enum TEXT_SIZES {
    H1 = 6,
    H2 = 5,
    H3 = 3,
    P = 2,
};


void setWindowTitle(char* title) {
    HWND hwnd = GetFocus(); // Get the handle to the current window
    SetWindowText(hwnd, title);
}

void showCenteredText(char* text, int x, int y) {
    // Width & height of text
    int textWidth = textwidth(text);
    int textHeight = textheight(text);

    // Adjust coordinates so text is centered
    x -= textWidth / 2;
    y -= textHeight / 2;

    // Display text
    outtextxy(x, y, text);
}

// Center text on the x axis
void outTextMiddle(int y, char* s) {
    outtextxy(WIDTH/2, y, s);
}

void generateWindowContent() {

    setbkcolor(COLOR(45,45,45));
    setfillstyle(SOLID_FILL, COLOR(45,45,45));
    bar(0, 0, getmaxx(), getmaxy());  // Draw a rectangle covering background of entire window

    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, H3);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outTextMiddle(WIDTH/6, TITLE);
    /*
    * cleardevice(); -- clears window, to be used later
    * clearviewport(); -- clears bg color
    */
}

// Draw if block in NS diagram
void drawIfBlock(char *instruction, int top, int left, int blockSize = 60) {
    int bottom = top + blockSize,
        right = MAX_WIDTH;
    rectangle(left, top, right, bottom);
    int center = (left+right)/2;
    line(left, top, center, bottom);
    line(right, top, center, bottom);

    outtextxy(left + textwidth("TRUE") /2, bottom, "TRUE");
    outtextxy(right - textwidth("FALSE") / 2, bottom, "FALSE");
    outtextxy(center, top + textheight(instruction), instruction);

}


// Create diagram block by block
void createDiagram() {
    /* Merg prin blocuri, tin cont de prioritati
     * adaug blockSize la X pentru fiecare prioritate crescuta (>)
     * si scad blockSize din X pentru fiecare prioritate scazuta (<)
     * incepand de la x=0;
     * pt if-uri logica separata; de acoperit;
     * --- de adagaut structura asociata lineType-urilor:
     * --- daca lineType = if, drawIfBlock etc.
    */
}

void createWindow() {
    initwindow(WIDTH, HEIGHT);
    setWindowTitle(TITLE);
    generateWindowContent();
    getch(); // Keep window open
    closegraph();
}
