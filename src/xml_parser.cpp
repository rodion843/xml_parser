#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
//<?xml version="1.0" encoding="UTF-8" standalone="no" ?>

struct xml_header
{
  std::string version;
  std::string encoding;
  bool standalone;
};

auto parseHeader(const auto &line)
{
  std::stringstream ss{line};
  char dump[100];
  ss.getline(dump, 100, '\"');

  char version[10];
  ss.getline(version, 10, '\"');

  ss.getline(dump, 100, '\"');
  char encoding[10];
  ss.getline(encoding, 10, '\"');

  ss.getline(dump, 100, '\"');
  char standalone[10];
  ss.getline(standalone, 10, '\"');

  xml_header hdr {{version}, 
	          {encoding}, 
		  std::string{standalone} == std::string{"no"}
                 };
}
int main()
{
  std::string path {"./sample.xml"};
  std::fstream fs(path);
  if(!fs.is_open()) std::cerr << "could not open file " << path;
  std::string str;
  getline(fs, str);
  parseHeader(str);
  return 0;
}
