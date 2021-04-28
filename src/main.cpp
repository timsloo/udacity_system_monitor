#include "ncurses_display.h"
#include "system.h"

#include <iostream> // TODO; 

int main() {
  System system;
  // std::cout << system.Cpu().Utilization() << std::endl;  //TODO
  NCursesDisplay::Display(system);
}