#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <array>
#include <set>
#include <vector>
//<?xml version="1.0" encoding="UTF-8" standalone="no" ?>

struct xml_header
{
  std::string version;
  std::string encoding;
  bool standalone;
};
struct tag
{
  std::string name;
  std::string value;
  tag* childs;
};
std::set<std::string> tags;
auto whatInside(auto &ss)
{
  //std::stringstream ss{str};
  std::string buffer(1024 * 1, '\0');
  while(ss.get(&buffer.front(), buffer.size(), '>'))
  {
    printf("%s\n", buffer.c_str());
    buffer.erase(0, buffer.find('<'));
    auto pos = buffer.find(' ');
    if (pos != std::string::npos)
    {
      printf("%s\n", buffer.c_str());
      tags.insert(buffer.substr(1, pos));
    }
    else
    {
      printf("%s\n", buffer.c_str());
      tags.insert(buffer.substr(1, buffer.size()));
    }
    //std::cout << buffer << '\n';
    ss.ignore(1);
  }

}
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
static std::string static_array(1024*32, '\0');
int main()
{
  try{
       std::string path {"./sample.xml"};
       std::fstream fs(path);
       if(!fs.is_open()) std::cerr << "could not open file " << path;
       auto pos = 0ul;
       while(fs.getline(&static_array[pos], static_array.size()))
       {
          pos += fs.gcount();
       }
       std::remove_if(static_array.begin(), static_array.begin() + pos, [](auto c){return (c == ' ') || (c == '\0');});
//       whatInside(fs);
//       std::string buf(64, '\0');
//       printTags(fs, buf);
       auto count = 0ul;
       for(const auto &c : static_array)
       {
          std::cout << c;
          ++count;
          if (c=='\0') break;
       }
       std::cout << "chars used: " << count;
  }
  catch(const std::exception &e)
  {
    std::cout << "Unhandled exception in main: " << e.what();
  }
  return 0;
}
