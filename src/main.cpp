#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include "linux_parser.h"
int main() {
  //Testing clone
  for (auto v: LinuxParser::CpuUtilization(1)) std::cout << v << std::endl;
  System system;
  //NCursesDisplay::Display(system);
}