#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include <iostream> // TODO

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() :  cpu_(Processor()), kernel_(LinuxParser::Kernel()), os_(LinuxParser::OperatingSystem()) {
  std::vector<int> pids = LinuxParser::Pids();
  for (int pid: pids) {
  	processes_.emplace_back(Process(pid)); 
  }
}

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string): /proc/version
std::string System::Kernel() { return kernel_; }

// TODO: Return the system's memory utilization: /proc/meminfo
float System::MemoryUtilization() { return  LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name: /ets/os-release
std::string System::OperatingSystem() { return os_; }

// TODO: Return the number of processes actively running on the system: /proc/meminfo
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system: /proc/meminfo
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running: /proc/uptime
long int System::UpTime() { return LinuxParser::UpTime(); } // 0; 