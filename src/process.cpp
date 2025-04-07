#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stof;

Process::Process(int pid) {
    this->pid = pid;
    this->user = LinuxParser::User(pid);
    this->command = LinuxParser::Command(pid);
    this->cpu_usage = LinuxParser::CpuUsage(pid);
    this->uptime = LinuxParser::UpTime(pid);
    try {
      this->ram = stof(LinuxParser::Ram(pid));
    } catch (const std::invalid_argument& e) {
      // std::cerr << "Invalid RAM value: " << this->ram << " for PID: " << this->pid << "\n";
      this->ram =  0.0;
    } catch (const std::out_of_range& e) {
      // std::cerr << "Out of range RAM value: " << this->ram << " for PID: " << this->pid << "\n";
      this->ram =  0.0;
    }
}

// TODO: Return this process's ID
int Process::Pid() { return this->pid; }

// TODO: Return the user (name) that generated this process
string Process::User() { return this->user; }

// TODO: Return the command that generated this process
string Process::Command() { return this->command; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return this->cpu_usage; }


std::string floatToStringWithTwoDecimal(float value) {
  std::ostringstream stream;
  stream << std::fixed << std::setprecision(2) << value;
  return stream.str();
}

// TODO: Return this process's memory utilization
string Process::Ram() { 

  try {
    float ram_value = this->ram;
    return floatToStringWithTwoDecimal(ram_value / 1024);
  } catch (const std::invalid_argument& e) {
    // std::cerr << "Invalid RAM value: " << this->ram << " for PID: " << this->pid << "\n";
    return "0.0";
  } catch (const std::out_of_range& e) {
    // std::cerr << "Out of range RAM value: " << this->ram << " for PID: " << this->pid << "\n";
    return "Out of range RAM";
  }
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return this->uptime; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a ) const {
  return this->ram < a.ram;
}
