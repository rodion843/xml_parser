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
  uint32_t name_pos;
  std::set<std::string> param;  //map in fututre
  std::string value;
  tag* child;

  bool setName(auto &tn, const std::string& name)
  {
    name_pos = tn.pos;
    tn.add_name(name);
    return true;
  }
  tag* getNext(const auto &name){
    return new tag{name, {}, {}, {}};
  }

};
void PrintTag(const auto &t)
{
  std::cout << t.name << '\n';
  if (t.child)
  {
    PrintTag(*t.child);
  }
}
bool hasValue(auto &name)
{
  if(name.find(' ') != std::string::npos) return true;
  else                                    return false;
}
void eatValue(auto &name)
{
  name.erase(name.find(' '), name.find('>'));
}
//between open and closed tags = content
//how to make it linear?
bool eatCorrespondingTag(const auto &name, auto &ss)
{
  auto et1 = ss.find("</" + name + ">");
  if(et1 != std::string::npos)
  {
    auto et2 = ss.find('>', et1);
    ss.erase(et1, et2 - et1 + 1);
    return true;
  }
  else
  {
    return false;
  }
}
tag file;
auto count = 0ul;
void findTag(auto &ss)
{
  //ignore header
  //find first tag
  //loop through all tags until you found closing bracket of the first
  char buf[32]{}; 
  auto bt1 = ss.find('<', 0);
  auto bt2 = ss.find('>', bt1);
  if(bt1 != std::string::npos && bt2 != std::string::npos)
  {
    ss.copy(buf, bt2 - bt1 - 1, bt1 + 1);
  }
  else
  {
    return;
  }
  std::string sbuf{buf};
  file.setName(tag_name, sbuf);
  
  if (hasValue(sbuf)) eatValue(sbuf);

  ss.erase(bt1, bt2 - bt1 + 1);
  if(!eatCorrespondingTag(sbuf, ss))
  {
    std::cerr << "didnt found corresponding tag\n";
    exit(1);
  }
  findTag(ss);
}
void eatHeader(auto &ss)
{
  ss.erase(ss.find("<?xml version"), ss.find("?>") + 2);
}
void Parse(auto &ss)
{
  eatHeader(ss);
  findTag(ss);
    //file.child = file.getNext(findTag(ss));
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
static std::string static_string(TAGS_CAPACITY, '\0');
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
