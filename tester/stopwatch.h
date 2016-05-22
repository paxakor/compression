#pragma once

#include <ctime>
#include <string>

class Stopwatch {
public:
  Stopwatch(const std::string& new_name)
  : name(new_name)
  , start(clock()) {
    std::cout << "Start " << name << std::endl;
  }
  void end() {
    double secs = (clock() - start) / CLOCKS_PER_SEC;
    std::cout << name << " ended in " << secs << " seconds" << std::endl;
    this->ended = true;
  }
  ~Stopwatch() {
    if (!ended) {
      this->end();
    }
  }
private:
  const std::string name;
  double start;
  float ended = false;
};
