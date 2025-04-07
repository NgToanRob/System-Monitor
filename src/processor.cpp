#include "processor.h"

#include <string>

#include "linux_parser.h"

using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  long curr_active = LinuxParser::ActiveJiffies();
  long curr_total = LinuxParser::Jiffies();

  long delta_active = curr_active - this->prev_active;
  long delta_total = curr_total - this->prev_total;

  this->prev_active = curr_active;
  this->prev_total = curr_total;

  if (delta_total == 0) return 0.0;
  return static_cast<float>(delta_active) / delta_total;
}