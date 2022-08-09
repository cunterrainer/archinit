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


const std::vector<std::string> g_Packages = {
    "pacman-contrib",
    "sysstat",
    "lm_sensors",
    "pulseaudio-alsa",
    "pulseaudio-bluetooth",
    "pulseaudio-equalizer",
    "alsa-utils",
    "pavucontrol",
    "feh",
    "ttf-font-awesome",
    "materia-gtk-theme",
    "papirus-icon-theme",
    "lxappearance",
    "rofi",
    "ttf-ubuntu-font-family",
    "curl",
    "neovim",
    "npm",
    "yarn",
    "clang",
    "nodejs",
    "xclip"
};


const std::vector<std::string> g_Parser = {
    "c",
    "cpp",
    "python",
    "cmake",
    "make"
};


const std::vector<std::string> g_CocLangServ = {
    "coc-clangd",
    "coc-pyright",
    "coc-cmake"
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


    void CreateFolder(const std::string& str)
    {
        const std::string process = "sudo mkdir -p " + str;
        RunProcess(process.c_str(), nullptr);
    }


    void CopyFile(const std::string& src, const std::string& dest)
    {
        const std::string process = "sudo cp " + src + " " + dest;
        RunProcess(process.c_str(), nullptr);
    }


    void CopyFolder(const std::string& src, const std::string& dest)
    {
        const std::string process = "sudo cp -RT " + src + " " + dest;
        RunProcess(process.c_str(), nullptr);
    }


    void Chown(const std::string& file, const std::string& owner)
    {
        const std::string process = "sudo chown " + owner + ":" + owner + " " + file;
        RunProcess(process.c_str(), nullptr);
    }


    void ChownFolder(const std::string& folder, const std::string& owner)
    {
        const std::string process = "sudo chown -R " + owner + " " + folder;
        RunProcess(process.c_str(), nullptr);
    }


    void Chmod(const std::string& file, const std::string& mode)
    {
        const std::string process = "sudo chmod " + mode + " " + file;
        RunProcess(process.c_str(), nullptr);
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


void InstallYay()
{
    RunProcess("sudo pacman -S git base-devel", "\ny");
    RunProcess("sudo git clone https://aur.archlinux.org/yay-git.git", nullptr);
    
    const std::string user_name = util::GetEnv("USER");
    const std::string chown_command = "sudo chown -R " + user_name + ":" + user_name + " ./yay-git";
    RunProcess(chown_command.c_str(), nullptr);
    RunProcess("(cd yay-git && makepkg -si)", "y");
    RunProcess("yay -Syu", nullptr);
    RunProcess("sudo rm -r yay-git", nullptr);
}


void InstallI3()
{
    std::string programsCommand = "sudo pacman -S " + util::VectorToString(g_Programs);
    RunProcess(programsCommand.c_str(), "\ny");
    CopyXinitFile();
    ConfigureXinitFile();

    programsCommand = "sudo pacman -S " + util::VectorToString(g_Packages);
    RunProcess(programsCommand.c_str(), "y");
    RunProcess("pulseaudio --check && pulseaudio -D", nullptr);
    InstallYay();
    RunProcess("yay -S google-chrome", nullptr);
    RunProcess("yay -S visual-studio-code-bin", nullptr);
    RunProcess("yay -S i3lock-color", nullptr);

    const std::string home = util::GetEnv("HOME");
    const std::string user = util::GetEnv("USER");
    const std::string config_folder = "config_files/";

    // i3status
    util::CreateFolder(home + "/.config/i3status");
    util::CopyFile(config_folder + "i3status.conf", home + "/.config/i3status/i3status.conf");
    util::Chown(home + "/.config/i3status/i3status.conf", user);

    // i3blocks
    util::CreateFolder(home + "/.config/i3blocks");
    util::CopyFile(config_folder + "i3blocks.conf", home + "/.config/i3blocks/i3blocks.conf");
    util::Chown(home + "/.config/i3blocks/i3blocks.conf", user);

    // i3
    util::CreateFolder(home + "/.config/i3");
    util::CopyFile(config_folder + "config", home + "/.config/i3/config");
    util::Chown(home + "/.config/i3/config", user);

    // Rofi
    util::CreateFolder(home + "/.config/rofi");
    util::CreateFolder(home + "/usr/share/rofi/themes");
    util::CopyFile(config_folder + "config.rasi", home + "/.config/rofi/config.rasi");
    util::CopyFolder(config_folder + "rofi_themes", "/usr/share/rofi/themes");

    // Kitty
    util::CopyFolder(config_folder + "kitty", home + "/.config/kitty");

    // Scripts
    util::CopyFolder(config_folder + "scripts", home + "/.config/scripts");
    util::Chmod(home + "/.config/scripts/*", "+x -R");

    // Wallpaper
    util::CreateFolder(home + "/Pictures/wallpaper");
    util::CopyFolder(config_folder + "wallpaper", home + "/Pictures/wallpaper");

    // Neovim
    util::CreateFolder(home + "/.config/nvim/themes");
    util::CreateFolder(home + "/.config/nvim/lua");
    //util::CreateFolder(home + "/.config/nvim/autoload");
    util::ChownFolder(home + "/.config/nvim", user);

    util::CopyFile(config_folder + "init.vim", home + "/.config/nvim/init.vim");
    util::CopyFolder(config_folder + "themes", home + "/.config/nvim/themes");
    util::CopyFolder(config_folder + "lua", home + "/.config/nvim/lua");

    //programsCommand = "sudo curl https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim -o " + home + "/.config/nvim/autoload/plug.vim";
    //RunProcess(programsCommand.c_str(), nullptr);

    std::string nvim_cmd = "nvim +PlugInstall +CocUpdateSync +CocUpdate \"+CocInstall ";
    for(auto& i : g_CocLangServ)
        nvim_cmd += i + " ";

    nvim_cmd += "\" +TSUpdate \"+TSInstall ";
    for(auto& i : g_Parser)
        nvim_cmd += i + " ";
    nvim_cmd += "\"";
    RunProcess(nvim_cmd.c_str(), nullptr);
}


int main()
{
    if(geteuid() == 0) {
        PrintColor(YEL, "Warning: You're trying to run this program as root, hence the files will be copied into the root home dir\nAre you sure you want to continue? [y|n]:");
        char yn;
        std::cin >> yn;
        if (yn != 'y' && yn != 'Y')
            return 0;
    }

    InstallI3();
}
