#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <errno.h>

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

            if (_type == "echo" || _type == "exit" || _type == "type")
            {
                std::cout << _type << " is a shell builtin" << std::endl;
            }
            else
            {
                char* PATH = std::getenv("PATH");
                std::istringstream pathStream(PATH);
                std::string pathSplit;
                bool isValid{false};

                while (std::getline(pathStream, pathSplit, ':'))
                {
                    std::string commandPath = pathSplit + '/' + _type;
                    if (access(commandPath.c_str(), X_OK) == 0)
                    {
                        std::cout << _type << " is " << commandPath << std::endl;
                        isValid = true;
                        break;
                    }
                }

                if (!isValid)
                {
                    std::cout << _type << ": not found" << std::endl;
                }
            }
        }
        else
        {
            std::cout << command << ": command not found" << std::endl;
        }
    }
}
