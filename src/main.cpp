#include "ncurses_display.h"
#include "system.h"
#include "process.h"
#include "linux_parser.h"
#include <iostream>

int main() {
  // Process process(1070);
  // std::cout << process.Pid() << "\n";
  // std::cout << process.User() << "\n";
  // std::cout << process.Command() << "\n";
  // std::cout << process.CpuUtilization() << "\n";
  // std::cout << process.UpTime() << "\n";
  // std::cout << LinuxParser::OperatingSystem() << "\n";
  // std::cout << LinuxParser::Kernel() << "\n";

  // std::cout << LinuxParser::MemoryUtilization() << "\n";
  // std::cout << LinuxParser::TotalProcesses() << "\n";
  // std::cout << LinuxParser::RunningProcesses() << "\n";
  // std::cout << LinuxParser::UpTime() << "\n";

  System system;

  // std::cout << system.Cpu().Utilization() << "\n";
  NCursesDisplay::Display(system);
}