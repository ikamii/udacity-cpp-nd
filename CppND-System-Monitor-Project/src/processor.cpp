#include "processor.h"
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  vector<long> cpuVec = LinuxParser::CpuLong();  
  float Idle, NonIdle, Total, TotalD, IdleD;

  Idle = (float)cpuVec[LinuxParser::kIdle_] + (float)cpuVec[LinuxParser::kIOwait_];
  NonIdle = (float)cpuVec[LinuxParser::kUser_] + (float)cpuVec[LinuxParser::kNice_] + (float)cpuVec[LinuxParser::kSystem_] + 
            (float)cpuVec[LinuxParser::kIRQ_] + (float)cpuVec[LinuxParser::kSoftIRQ_] + (float)cpuVec[LinuxParser::kSteal_];
  
  Total = Idle + NonIdle;
  
  TotalD = Total - PrevTotal;
  IdleD= Idle - PrevIdle;

  PrevTotal = Total;
  PrevIdle = Idle;

  return (TotalD - IdleD) / TotalD;
  
}