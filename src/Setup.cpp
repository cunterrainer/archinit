#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>

#include "Process.h"
#include "Output.h"

const std::vector<std::string> g_Programs = {
    "i3-gaps",
    "i3blocks",
    "i3status",
    "i3lock",
    "dmenu",
    "xorg",
    "xorg-xinit",
    "kitty"
};

namespace util
{
    std::string VectorToString(const std::vector<std::string>& vec)
    {
        std::string ret;
        for(std::size_t i = 0; i < vec.size(); ++i)
            ret += vec[i] + " ";
        return ret;
    }


    std::string GetEnv(const char* var)
    {
        const char* val = std::getenv(var);
        if(val == nullptr)
            return "";
        return val;
    }


    std::vector<std::string> ReadFileLineByLine(const std::string& file_path)
    {
        std::vector<std::string> vec;
        std::ifstream input(file_path); 
        if(!input)
        {
            input.close();
            PrintError("Failed to open file [%s]\n", file_path.c_str());
            return vec;
        }

        std::string line;
        while(std::getline(input, line))
            vec.push_back(line);
        return vec;
    }


    void WriteStringToFile(const std::string& file_path, const std::string content)
    {
        std::ofstream output(file_path);
        if(!output)
        {
            output.close();
            PrintError("Failed to open output file [%s]\n", file_path.c_str());
            return;
        }
        output << content;
        output.close();
    }
}


void CopyXinitFile()
{
    const std::string home_dir = util::GetEnv("HOME");
    const std::string user = util::GetEnv("USER");
    const std::string copy_command = "sudo cp /etc/X11/xinit/xinitrc " + home_dir + "/.xinitrc";
    const std::string chown_command = "sudo chown " + user + ":" + user + " " + home_dir + "/.xinitrc";
    RunProcess(copy_command.c_str(), nullptr);
    RunProcess(chown_command.c_str(), nullptr);
}


void ConfigureXinitFile()
{
    const std::string home_dir = util::GetEnv("HOME");
    const std::string file_path = home_dir + "/.xinitrc";
    const std::vector<std::string> lines = util::ReadFileLineByLine(file_path);

    std::string new_content;
    for(std::size_t i = 0; i < lines.size(); ++i)
    {
        // if its in the string
        if(lines[i].find("xclock -geometry") != std::string::npos)
            continue;
        if(lines[i].find("xterm -geometry") != std::string::npos)
            continue;
        if(lines[i].find("twm &") != std::string::npos)
            continue;
        new_content += lines[i] + std::string("\n");
    }

    new_content += "exec i3";
    util::WriteStringToFile(file_path, new_content);
    const std::string chown_command = "sudo chown root:root " + home_dir + "/.xinitrc";
    RunProcess(chown_command.c_str(), nullptr);
}


void InstallI3()
{
    const std::string programsCommand = "sudo pacman -S " + util::VectorToString(g_Programs);
    RunProcess(programsCommand.c_str(), "\ny");
    CopyXinitFile();
    ConfigureXinitFile();
}


int main()
{
    if(geteuid() == 0) {
        PrintColor(YEL, "Warning: You're trying to run this program as root, hence the files will be copied into the root home dir\nAre you sure you want to proceede? [y|n]:");
        return EXIT_FAILURE;
    }

    InstallI3();
}
