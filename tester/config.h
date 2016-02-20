#pragma once

#include <string>
#include <map>

class Config : public std::map< std::string, std::string > {
public:
  Config();
  Config(const std::string&);
  void read(const std::string& = "tester.conf");
};
