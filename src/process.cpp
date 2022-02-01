#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor 
Process::Process(int pid) : pid_ (pid) {
    pid_ = pid;
    user_ = LinuxParser::User(pid);
    command_ = LinuxParser::Command(pid);  
}

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { 
  float  busy_time;
  std::vector <long> data;
  for (auto v : LinuxParser::CpuUtilization(Pid())) data.emplace_back(stol(v));
/*  # idx 0 = utime - CPU time spent in user code, measured in clock ticks
    # idx 1 = stime - CPU time spent in kernel code, measured in clock ticks
    # idx 2 = cutime - Waited-for children's CPU time spent in user code (in clock ticks)
    # idx 3 = cstime - Waited-for children's CPU time spent in kernel code (in clock ticks) */  

  busy_time = (data[0] + data[1] + data[2] + data[3]) / sysconf(_SC_CLK_TCK);
  return busy_time / UpTime();
}

// Return the command that generated this process
string Process::Command() { return command_; }

// Return this process's memory utilization
string Process::Ram() { 
    return std::to_string((stol(LinuxParser::Ram(Pid())))/ 1024); 
}

// Return the user (name) that generated this process
string Process::User() { return user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(Pid());
 }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return stol(LinuxParser::Ram(pid_)) > stol(LinuxParser::Ram(a.pid_));
}