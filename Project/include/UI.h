#include <graphics.h>
#include <winbgim.h>
#define TITLE "Diagrame Nassi-Shneiderman"

// Variables
int WIDTH = 900, HEIGHT = 900;

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
    // setbkcolor(COLOR(45,45,45));
    setfillstyle(SOLID_FILL, COLOR(45,45,45));
    bar(0, 0, getmaxx(), getmaxy());  // Draw a rectangle covering background of entire window

    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, H3); // Default font is too thin, use sans serif
    settextjustify(CENTER_TEXT, CENTER_TEXT);
    outTextMiddle(WIDTH/6, TITLE);
    /*
    * cleardevice(); -- clears window, to be used later
    * clearviewport(); -- clears bg color
    */
}

void createWindow() {
    initwindow(WIDTH, HEIGHT);
    setWindowTitle(TITLE);
    generateWindowContent();
    getch(); // Keep window open
    closegraph();
}
