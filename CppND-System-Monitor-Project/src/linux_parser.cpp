#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

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
  string os, version, kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  
  string line, key;
  float value, MemTotal, MemFree;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if(key == "MemTotal:") MemTotal = value;
        else if (key == "MemFree:") MemFree = value;
      }
    }
  }
  return (MemTotal - MemFree) / MemTotal;
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  
  string line;
  long upTime, idleTime;

  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  return upTime;
 }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies();}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  
  string line, value;
  vector<string> proc;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()){
  while (std::getline(stream, line)){
    std::istringstream linestream(line);
    while (linestream >> value) proc.push_back(value);
  }
 }
 return std::stol(proc[utime_]) + std::stol(proc[stime_]) + 
        std::stol(proc[cutime_]) + std::stol(proc[cstime_]);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  
  vector<long> cpu = LinuxParser::CpuLong();
  return cpu[kUser_] + cpu[kNice_] + cpu[kSystem_] + 
          cpu[kIRQ_] + cpu[kSoftIRQ_] + cpu[kSteal_];
 }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  
  vector<long> cpu = LinuxParser::CpuLong();
  return cpu[kIdle_] + cpu[kIOwait_];
 }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  
  string line, key, value;
  vector<string> cpuVal;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        while (linestream >> value) cpuVal.push_back(value);
      }
    }
  }
  return cpuVal;
 }

// Function for converting all CpuUtil to long
 vector<long> LinuxParser::CpuLong(){

   vector<string> cpuUtil = LinuxParser::CpuUtilization();
   vector<long> longCpuUtil;

   for (string cpu : cpuUtil) longCpuUtil.push_back(std::stol(cpu));
   return longCpuUtil;
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  
  string line, key;
  int value, totalProcess;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "processes") totalProcess = value;
      }
    }
  }
  return totalProcess;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  
  string line, key;
  int value, runningProcess;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "procs_running") runningProcess = value;
      }
    }
  }
  return runningProcess;
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  
  string line;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) std::getline(stream, line);
  return line;
 }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  
  string line, key, value;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "VmSize:") {return std::to_string(std::stol(value) / 1024);} // KB -> MB
      }
    }
  }
  return "0";
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  
  string line, key, value;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "Uid:") return value;
      }
    }
  }
  return "value";
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  
  string line, username, x, uid;
  
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> x >> uid){
        if (uid == LinuxParser::Uid(pid)) return username;
      }
    }
  }
  return username;
 }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  
  string line, value;
  vector<string> allValues;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value){
      allValues.push_back(value);
    }
  }
  return LinuxParser::UpTime() - (std::stol(allValues[starttime_]) / 100);
 }

float LinuxParser::ProcCpuUtilization(int pid){

  string line, value;
  vector<string> proc;
  float uptime = LinuxParser::UpTime();
  float totalTime, elapsedTime, seconds, CPU;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()){
  while (std::getline(stream, line)){
    std::istringstream linestream(line);
    while (linestream >> value) proc.push_back(value);
  }
 }

 totalTime = std::stof(proc[utime_]) + std::stof(proc[stime_]) + std::stof(proc[cutime_]) + std::stof(proc[cstime_]);
 elapsedTime = std::stof(proc[starttime_]) / sysconf(_SC_CLK_TCK);
 seconds = uptime - elapsedTime;

 return CPU = (totalTime / sysconf(_SC_CLK_TCK)) / seconds;

}
