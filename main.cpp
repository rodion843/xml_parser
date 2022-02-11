#include <iostream>
#include <string>
#include <fstream>

int main()
{
  std::string path {"./sample.xml"};
  std::fstream fs(path);
  if(!fs.is_open()) std::cerr << "could not open file " << path;
  char output[200];
  while(fs.getline(output, 200))
  {
	  std::cout << output << '\n';
  }
  std::cout << "hello android\n";
  return 0;
}
