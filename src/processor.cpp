#include <string>
#include <vector>
#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() { 
  long idle, active, total, diff_total, diff_idle;
  static long prev_idle, prev_total;
  std::vector <long> data;
  for (auto v : LinuxParser::CpuUtilization()) data.emplace_back(stol(v));
  // index  0     1     2       3     4       5     6         7       8       9
  // value  user  nice  system  idle  iowait  irq   softirq   steal   guest   guest_nice
  
  idle = data[3] + data[4];

  active = data[0] + data[1] + data[2] + data[5] + data[6] + data[7];

  total = idle + active;

  //differentiate: actual value minus the previous one
  diff_total = total - prev_total;
  diff_idle = idle - prev_idle;

  prev_idle = idle;
  prev_total = total;

  return (float)(diff_total - diff_idle) / diff_total;
}