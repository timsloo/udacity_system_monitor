#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

#include <cassert> 

#include <iostream> 

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization: /proc/meminfo
float LinuxParser::MemoryUtilization() { 
  int mem_total, mem_free; 
  string line; 
  std::ifstream filestream(kProcDirectory + kMeminfoFilename); 
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) { 
      std::istringstream linestream(line);
      string key; 
      int val; 
      
      linestream >> key >> val; 
      if (key == "MemTotal:") 
        mem_total = val; 
      else if (key == "MemFree:") 
        mem_free = val; 
    }
  }
  return (mem_total - mem_free) *1.0F / mem_total; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::ifstream filestream(kProcDirectory + kUptimeFilename); 
  string line; 
  string system_time, idle_time; 
  
  if (filestream.is_open()) {
  	std::getline(filestream, line); 
    std::istringstream linestream(line); 
    linestream >> system_time >> idle_time; 
  }
  return std::stol(system_time); 
}

// Not using those template functions in my implementation 

/*
// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

*/ 

// Changed output type from vector<string> to float 
float LinuxParser::CpuUtilization() { 
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename); 
  std::string first_line;
  std::getline(filestream, first_line); 
  std::istringstream linestream(first_line); 
  std::string cpu; 
  int user, nice, sys, idle, iowait, irq, softirq, steal, guest, guest_nice; 
  linestream >> cpu >> user >> nice >> sys >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice; 
  
  int idle_time = idle + iowait; 
  int nonidle_time = user + nice + sys + irq + softirq + steal; 
  int total = idle_time + nonidle_time; 
  
  float cpu_utilization = nonidle_time * 1.0F / total; 
  return cpu_utilization; 
}

// Added function for process specific Cpu utilization 
float LinuxParser::CpuUtilization(int pid){
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename); 
  string line, tmp; 
  long utime, stime, cutime, cstime, starttime;
  if (filestream.is_open()) {
    std::getline(filestream, line); 
    std::istringstream linestream(line); 
   
    
    for(int i=0; i<22; i++) {
      linestream >> tmp; 
      switch(i) {
        case 13: 
          utime = std::stol(tmp); 
          break; 
        case 14: 
          stime = std::stol(tmp); 
          break; 
        case 15: 
          cutime = std::stol(tmp); 
          break; 
        case 16: 
          cstime = std::stol(tmp); 
          break; 
        case 21: 
          starttime = std::stol(tmp); 
          break; 
      }
     
    }

  }
  long uptime = LinuxParser::UpTime(pid); 
  float hz = sysconf(_SC_CLK_TCK) * 1.0F; 
  
  long total_time = utime + stime + cutime + cstime;    // active jiffies
  float elapsed_time = uptime - (starttime / hz); 
  float cpu_usage = 100.0 * (total_time / hz) / elapsed_time; 
  return cpu_usage; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::ifstream filestream(kProcDirectory + kStatFilename); 
  string line; 
  int total_procs; 
  
  if (filestream.is_open()) {
  	while(std::getline(filestream, line)) {
    	std::istringstream linestream(line); 
        string key;
        int val; 
      	linestream >> key >> val; 
        if (key == "processes") {
          total_procs = val; 
        }
    }
  }
  return total_procs; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream filestream(kProcDirectory + kStatFilename); 
  string line; 
  int running_procs; 
  
  if (filestream.is_open()) {
  	while(std::getline(filestream, line)) {
    	std::istringstream linestream(line); 
        string key;
        int val; 
      	linestream >> key >> val; 
        if (key == "procs_running") {
          running_procs = val; 
        }
    }
  }
  return running_procs; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kCmdlineFilename); 
  string command; 
  if (filestream.is_open()) {
  	std::getline(filestream, command); 
  }
  return command; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatusFilename); 
  string line; 
  int ram; 
  if (filestream.is_open()) {
  	while(std::getline(filestream, line)) {
      std::istringstream linestream(line); 
      string key, value; 
      linestream >> key >> value; 
      if (key == "VmSize:") {
      	ram = std::stoi(value) / 1024; // convert from kB to MB
        break; 
      }
    }
  }
  return std::to_string(ram);  
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatusFilename); 
  string line; 
  string uid; 
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line); 
      string key, first_val; 
      linestream >> key >> first_val; 
      if (key == "Uid:") {
        uid = first_val; 
        break; 
      }
    }
  }
  return uid; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string uid = LinuxParser::Uid(pid); 
  std::ifstream filestream(kPasswordPath); 
  string line; 
  string user; 
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line); 
      string cur_user, x, cur_uid; 

      linestream >> cur_user >> x >> cur_uid; 
      if (cur_uid == uid) {
      	user = cur_user; 
        break; 
      }
    }
  }
  return user; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename); 
  string line; 
  string uptime_string; 
  if (filestream.is_open()) {
    std::getline(filestream, line); 
    std::istringstream linestream(line); 
    for (int i = 0; i < 22; i++)
      linestream >> uptime_string; 
  }
  long uptime = stol(uptime_string); 
  return uptime; 
}
