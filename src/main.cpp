#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include "linux_parser.h"
int main() {
  //Testing clone
  for (auto v : LinuxParser::CpuUtilization()) std::cout <<  v << " ";
  std::cout << std::endl;
  System system;
  //NCursesDisplay::Display(system);
}