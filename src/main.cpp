#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include "linux_parser.h"
int main() {
  //Testing clone
  std::cout << LinuxParser::RunningProcesses() << std::endl;
  System system;
  //NCursesDisplay::Display(system);
}