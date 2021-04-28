#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  int prev_idle_time_ {0}; 
  int prev_nonidle_time_ {0}; 
};

#endif