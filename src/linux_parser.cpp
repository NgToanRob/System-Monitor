#include "linux_parser.h"

#include <dirent.h>
#include <pwd.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

// SYSTEM FUNCTION

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key;
  long value;
  long mem_total = 0;
  long mem_free = 0;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;

      if (key == "MemTotal:") {
        mem_total = value;
      } else if (key == "MemFree:") {
        mem_free = value;
      }

      if (mem_total != 0 && mem_free != 0) break;
    }
  }

  if (mem_total == 0.0) return 0.0;
  return static_cast<float>(mem_total - mem_free) / mem_total;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  long uptime = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);

  if (filestream.is_open()) {
    // Read first value from file
    filestream >> uptime;
  } else {
    // std::cerr << "Error: Unable to open " << kProcDirectory + kUptimeFilename
    //           << '\n';
  }

  return uptime;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        try {
          int pid = std::stoi(filename);
          pids.push_back(pid);
        } catch (const std::invalid_argument& e) {
          // std::cerr << "Invalid PID (non-numeric): " << filename << "\n";
          continue;
        } catch (const std::out_of_range& e) {
          // std::cerr << "PID out of range: " << filename << "\n";
          continue;
        }
      }
    }
  }
  closedir(directory);
  return pids;
}


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string line, key;
  int total_processes = 0;

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      linestream >> key;
      if (key == "processes") {
        linestream >> total_processes;
        break;
      }
    }
  } else {
    // std::cerr << "Error: Unable to open " << kProcDirectory + kStatFilename
    //           << '\n';
  }
  return total_processes;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string line, key;
  int procs_running = 0;

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      linestream >> key;
      if (key == "procs_running") {
        linestream >> procs_running;
        break;
      }
    }
  } else {
    // std::cerr << "Error: Unable to open " << kProcDirectory + kStatFilename
    //           << '\n';
  }
  return procs_running;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// CPU FUCNTION

// TODO: Read and return CPU utilization
/**
 * @brief Retrieves CPU utilization data from the /proc/stat file.
 *
 * This function reads the first line of the /proc/stat file, which contains
 * aggregate CPU statistics. It extracts the values following the "cpu" key
 * and returns them as a vector of strings. Each value corresponds to a specific
 * CPU time category, as described below:
 *
 * Field Index | Name        | Description
 * ------------|-------------|-------------------------------------------------
 * 0           | user        | Time spent in user mode.
 * 1           | nice        | Time spent in user mode with low priority (nice).
 * 2           | system      | Time spent in system mode.
 * 3           | idle        | Time spent idle.
 * 4           | iowait      | Time spent waiting for I/O operations to
 *             |             |complete.
 * 5           | irq         | Time spent servicing hardware interrupts.
 * 6           | softirq     | Time spent servicing software interrupts.
 * 7           | steal       | Time spent in other operating systems when
 *             |             | running in a virtualized environment.
 * 8           | guest       | Time spent running a virtual CPU for guest
 *             |             | operating systems.
 * 9           | guest_nice  | Time spent running a virtual CPU for guest
 *             |             | operating systems with low priority.
 *
 * @return A vector of strings containing CPU utilization values. If the file
 *         cannot be opened, an error message is printed to standard error,
 *         and an empty vector is returned.
 */
vector<string> LinuxParser::CpuUtilization() {
  std::ifstream filestream(kProcDirectory + kStatFilename);
  string line, key;
  vector<string> values;

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    linestream >> key;
    if (key == "cpu") {
      string value;
      while (linestream >> value) {
        values.push_back(value);
      }
    }
  } else {
    // std::cerr << "Error: Unable to open " << kProcDirectory + kStatFilename
    //           << '\n';
  }
  return values;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> cpu_ulti = CpuUtilization();
  long total = 0;
  for (const auto& value : cpu_ulti) {
    try {
      total += stol(value);
    } catch (const std::exception& e) {
      // std::cerr << "Error parsing jiffy value: " << e.what() << '\n';
    }
  }
  return total;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> cpu_ulti = CpuUtilization();
  long user = stol(cpu_ulti[0]);
  long nice = stol(cpu_ulti[1]);
  long system = stol(cpu_ulti[2]);
  long irq = stol(cpu_ulti[5]);
  long softirq = stol(cpu_ulti[6]);
  long steal = stol(cpu_ulti[7]);
  return user + nice + system + irq + softirq + steal;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  long active_jifies = 0;
  string line, value;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    for (int i = 0; i < 13; i++) {
      linestream >> value;
    }
    // #13 utime: Thời gian (tính bằng jiffies) mà quá trình đã sử dụng CPU
    // trong trạng thái user.
    long utime;
    linestream >> utime;
    // #14 stime: Thời gian (tính bằng jiffies) mà quá trình đã sử dụng CPU
    // trong trạng thái kernel.
    long stime;
    linestream >> stime;

    active_jifies = utime + stime;
  } else {
    // std::cerr << "Error: Unable to open "
    //           << kProcDirectory + std::to_string(pid) + kStatFilename << '\n';
  }
  return active_jifies;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> cpu_ulti = CpuUtilization();
  long idle = stol(cpu_ulti[3]);
  long iowait = stol(cpu_ulti[4]);
  return idle + iowait;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line, command;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if (filestream.is_open()) {
    std::getline(filestream, line);
    // std::replace(line.begin(), line.end(), "\0", " ");
    for (char& c : line) {
      if (c == '\0') c = ' ';
    }
    command = line;
  } else {
    // std::cerr << "Error: Unable to open "
    //           << kProcDirectory + std::to_string(pid) + kCmdlineFilename
    //           << '\n';
  }
  return command;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string key, value, ram;
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        ram = value;
        break;
      }
    }

  } else {
    // std::cerr << "Error: Unable to open "
    //           << kProcDirectory + std::to_string(pid) + kStatusFilename << '\n';
  }
  return ram;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string key, value, uid;
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        uid = value;
        break;
      }
    }

  } else {
    // std::cerr << "Error: Unable to open "
    //           << kProcDirectory + std::to_string(pid) + kStatusFilename << '\n';
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = Uid(pid);

  // Ensure uid is not empty and contains only digits before converting
  if (uid.empty() || !std::all_of(uid.begin(), uid.end(), ::isdigit)) {
    // std::cerr << "Invalid UID format: " << uid << std::endl;
    return "";
  }

  try {
    // Convert UID string to integer
    struct passwd* pw = getpwuid(std::stoi(uid));
    if (pw) {
      return pw->pw_name;
    } else {
      // std::cerr << "Unable to find user name for UID: " << uid << std::endl;
      return "";
    }
  } catch (const std::invalid_argument& e) {
    // std::cerr << "Invalid UID: " << uid << " (unable to convert to integer)" << std::endl;
    return "";
  } catch (const std::out_of_range& e) {
    // std::cerr << "UID out of range: " << uid << std::endl;
    return "";
  }
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, value;
  long start_time = 0;
  long uptime_system = UpTime();

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    for (int i = 0; i < 21; i++) {
      linestream >> value;
    }
    // uptime of process is #22
    linestream >> start_time;

  } else {
    // std::cerr << "Error: Unable to open "
    //           << kProcDirectory + std::to_string(pid) + kStatFilename << '\n';
  }
  long uptime_process = uptime_system - (start_time / sysconf(_SC_CLK_TCK));
  return uptime_process;
}

float LinuxParser::CpuUsage(int pid) {
  string line, value;
  long uptime_process = UpTime(pid);
  long total_time = 0;
  float cpu_usage = 0.0;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);

    for (int i = 0; i < 13; i++) {
      linestream >> value;
    }
    // 14 -> 17
    for (int i = 13; i < 17; i++) {
      linestream >> value;
      total_time += stol(value);
    }

  } else {
    // std::cerr << "Error: Unable to open "
    //           << kProcDirectory + std::to_string(pid) + kStatFilename << '\n';
  }
  cpu_usage = ((static_cast<float>(total_time)  / sysconf(_SC_CLK_TCK)) / uptime_process);
  return cpu_usage;
}