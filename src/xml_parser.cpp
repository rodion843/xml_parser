#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <array>
#include <set>
#include <vector>
//<?xml version="1.0" encoding="UTF-8" standalone="no" ?>
static constexpr auto TAGS_CAPACITY = 32768ul;
struct xml_header
{
  std::string version;
  std::string encoding;
  bool standalone;
};

struct tag_name
{
  void add_name(const std::string& name)
  {
    name.copy(&tags_names[pos], name.size());
    pos += name.size() + 1;
    tags_names[pos++] = '\n';
  }
  void print_all_names()
  {
    for (const auto &c : tags_names)
      std::cout << c;
    std::cout << '\n';
  }
  std::array<char, TAGS_CAPACITY> tags_names {0};
  std::size_t pos = 0;
}tag_name;

struct tag
{
  std::string name;
  std::set<std::string> param;  //map in fututre
  std::string value;
  tag* child;

  tag* getNext(const auto &name){
    return new tag{name, {}, {}, {}};
  }

};
std::vector<tag> vt;
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

  return xml_header {
    {version}, 
    {encoding}, 
    std::string{standalone} == std::string{"yes"}};
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
void PrintTag(const tag &t)
{
  std::cout << t.name << '\n';
  if (t.child)
  {
    PrintTag(*t.child);
  }
}
//between open and closed tags = content
//how to make it linear?
auto findTag(auto &ss)
{
  //ignore header
  //find first tag
  //loop through all tags until you found closing bracket of the first
  char buf[1024]{}; 
  auto bt1 = ss.find('<');
  auto bt2 = ss.find('>');
  auto et1 = ss.find('<', bt1);
  auto et2 = ss.find('>', bt2);
  ss.copy(buf, bt2 - bt1 - 1, bt1 + 1);
  ss.erase(bt1, bt2 - bt1);
  ss.erase(et1, et2 - et1);
  return std::string{buf};
}
void Parse(auto &ss)
{
  tag file;
  //file.name = findTag(ss);
  while(ss.size() > 50)
  {
    tag_name.add_name(findTag(ss));
    //file.child = file.getNext(findTag(ss));
  }
  tag_name.print_all_names();
  //find tag recurse until content
  //need to unroll. will be facing the same tags but closing
  //check them? skip? 
  //while(ss)
  //while(!content)
  //build node
  //find repeating tags
  //go to first while
}
static std::string static_string(1024 * 32, '\0');
int main()
{
  try{
    std::string path {"./sample.xml"};
    std::fstream fs(path);
    if(!fs.is_open()) std::cerr << "could not open file " << path;

    //parseHeader(fs);
    std::stringstream ss;
    ss << fs.rdbuf();
    static_string = ss.str();
    Parse(static_string);
    //static_string.erase(
    //  std::remove_if(static_string.begin(),
    //  static_string.end(), 
    //  [&](auto c){return (std::isspace(c) || (c == '\0'));}));
    //auto count = 0ul;
    //for(const auto &c : static_string)
    //{
    //   std::cout << c;
    //   ++count;
    //   if (c == '\0') break;
    //}
    //std::cout << "chars used: " << count;
  }
  catch(const std::exception &e)
  {
    std::cout << "Unhandled exception in main: " << e.what();
  }
  return 0;
}
