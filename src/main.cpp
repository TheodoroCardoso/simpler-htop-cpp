#include "ncurses_display.h"
#include "system.h"

int main() {
  //Testing clone
  System system;
  NCursesDisplay::Display(system);
}