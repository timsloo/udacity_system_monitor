#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int hours = seconds / (60 * 60); 
  seconds = seconds - hours * 60 * 60; 
  int minutes = seconds / 60; 
  seconds = seconds - minutes * 60; 
  
  string time = Format::FillLeadingZero(std::to_string(hours)) + ":" + 
    Format::FillLeadingZero(std::to_string(minutes)) + ":" + 
    Format::FillLeadingZero(std::to_string(seconds)); 
  return time; 
}

string Format::FillLeadingZero(string in) {
    string out = in; 
	if (in.size() < 2) { 
      out = "0" + in; 
    } 
    return out; 
}