#include <iostream>
#include <graphics.h>
#include <winbgim.h>
/*
while (x) do
{
cod
}
*/

int main()
{
    FILE *pseudocode = fopen("input.txt", "r");
    if (!pseudocode) { printf("Could not open file"); return -1; }
    /*
//  int gd = DETECT, gm;
//  initgraph(&gd, &gm, "");
  initwindow(800,600); int x = 400, y = 300, r;
  for (r = 25; r <= 125; r += 20)
      circle(x, y, r);
  getch(); closegraph(); return 0;
*/
    return 0;
}
