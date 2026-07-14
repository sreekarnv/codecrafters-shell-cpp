#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

std::vector<std::string> split(const std::string& str, char delimiter = ' ')
{
    std::vector<std::string> tokens;
    int startIdx = 0;
    int endIdx = str.find(delimiter);

    while (endIdx != std::string::npos)
    {
        tokens.push_back(str.substr(startIdx, endIdx - startIdx));
        startIdx = endIdx + 1;
        endIdx = str.find(delimiter, startIdx);
    }

    tokens.push_back(str.substr(startIdx));

    return tokens;
}

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

            if (_type == "echo" || _type == "exit" || _type == "type" || _type == "pwd")
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
        else if (command == "pwd")
        {
            std::cout << std::filesystem::current_path().c_str() << std::endl;
        }
        else if (command.substr(0, 3) == "cd ")
        {
            std::string dirPath = command.substr(3);

            try
            {
                std::filesystem::current_path(dirPath);
            }
            catch (const std::filesystem::filesystem_error& e)
            {
                std::cout << "cd: " << dirPath << ": No such file or directory" << std::endl;
            }
        }
        else
        {
            char* PATH = std::getenv("PATH");
            std::istringstream pathStream(PATH);
            std::string pathSplit;
            bool isValid{false};

            std::vector<std::string> cmdItems = split(command);

            while (std::getline(pathStream, pathSplit, ':'))
            {
                std::string commandPath = pathSplit + '/' + cmdItems[0];
                if (access(commandPath.c_str(), X_OK) == 0)
                {
                    std::system(("cd " + pathSplit).c_str());
                    std::system(command.c_str());
                    isValid = true;
                    break;
                }
            }

            if (!isValid)
            {
                std::cout << command << ": command not found" << std::endl;
            }
        }
    }
}
