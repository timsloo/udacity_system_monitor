#include "processor.h"
#include "linux_parser.h"

#include <string> 
#include <sstream> 

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  /*
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
  
  float cpu_utilization = (nonidle_time - prev_nonidle_time_) *1.0F / (total - prev_idle_time_ - prev_nonidle_time_); 
    
  prev_idle_time_ = idle_time; 
  prev_nonidle_time_ = nonidle_time; 
  
  
  return cpu_utilization;  
  */ 
  return LinuxParser::CpuUtilization(); 
}