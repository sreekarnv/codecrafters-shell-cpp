#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  while (true)
  {
      std::cout << "$ ";
      std::string command;
      std::getline(std::cin, command);

      if (command == "exit") break;

      if (command.substr(0, 4) == "echo")
      {
          std::cout << command.substr(5) << std::endl;
      }
      else if (command.substr(0, 4) == "type")
      {
          std::string _type = command.substr(5);
          if (_type == "echo" || _type == "type" || _type == "exit")
          {
              std::cout << _type << " is a shell builtin" << std::endl;
          }
          else
          {
              std::cout << _type << ": not found" << std::endl;
          }
      }
      else
      {
          std::cout << command << ": command not found" << std::endl;
      }
  }
}
