#include <graphics.h>
#include <winbgim.h>

void createWindow() {
  initwindow(800,600); int x = 400, y = 300, r;
  for (r = 25; r <= 125; r += 20)
      circle(x, y, r);
  getch(); closegraph();
}
