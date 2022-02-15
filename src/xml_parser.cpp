#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <array>
#include <set>
//<?xml version="1.0" encoding="UTF-8" standalone="no" ?>

struct xml_header
{
  std::string version;
  std::string encoding;
  bool standalone;
};

auto parseHeader(auto &ss)
{
  char dump[100];
  ss.getline(dump, 100, '\"');

  char version[10];
  ss.getline(version, 10, '\"');

  ss.getline(dump, 100, '\"');
  char encoding[10] {"empty"};
  ss.getline(encoding, 10, '\"');

  ss.getline(dump, 100, '\"');
  char standalone[10];
  ss.getline(standalone, 10, '\"');

  xml_header hdr {{version}, 
                  {encoding}, 
                  std::string{standalone} == std::string{"yes"}
                 };
  std::cout << "version:     " << hdr.version 
            << "\nencoding:  " << hdr.encoding 
            << "\nstandalonei: " << hdr.standalone; 
  std::cout << "\n";
}
std::set<std::string> tags;
void printTags(auto &fs, 
               std::string &str, 
               bool firstTime = true)
{
  std::string buffer{str};
  auto pos = 0ul;
  if (!firstTime)
  {
    buffer.resize(str.size() * 2);
    pos = str.size();
  }
  while(fs.read(&buffer[pos], buffer.size()))
  {
    std::cout << "buffer size :" << buffer.size() << '\n' 
              << buffer << '\n';
    if(buffer.end() != std::find(buffer.begin(), 
               buffer.end(), '<'))
    {
      std::cout << "going in\n";
      printTags(fs, buffer, false);
      std::cout << "going out\n";
    }
  }
}
int main()
{
  std::string path {"./sample.xml"};
  std::fstream fs(path);
  if(!fs.is_open()) std::cerr << "could not open file " << path;
  std::string buf(64, '\0');
  printTags(fs, buf);
  return 0;
}
