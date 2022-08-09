#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <unistd.h>

#include "Process.h"
#include "Output.h"


std::string GetVMwareToolsName(const std::string& path)
{
    for(const auto& entry : std::filesystem::directory_iterator(path))
    {
        const std::string entry_str = std::move(entry.path().string());
        const std::string start = entry_str.substr(0, 16);
        const std::string end = entry_str.substr(entry_str.size() - 7, 7);

        if(start == "/mnt/VMwareTools" && end == ".tar.gz")
            return entry_str;
    }
    return "";
}


int main()
{
    if(getuid() != 0)
    {
        PrintError("You don't have root priviliges!\nDon't run this as sudo but as root user!");
        return 0;
    }
    else
    {
        PrintColor(YEL, "Are you logged in a root user (not sudo!)?\nIs the VMware tools installer already mounted?[Y|N]:");
        char yn;
        std::cin >> yn;
        if(yn != 'Y' && yn != 'y')
            return 0;
    }
    RunProcess("pacman -S perl xf86-video-vmware", "y");

    RunProcess("for x in {0..6}; do mkdir -p /etc/init.d/rc${x}.d; done", nullptr);
    RunProcess("mount /dev/cdrom /mnt", nullptr);
    const std::string vmwaretools = GetVMwareToolsName("/mnt");
    if(vmwaretools == "")
    {
        PrintError("Couldn't find VMwareTools*.tar.gz!\n");
        return 0;
    }

    const std::string process = "tar xf " + vmwaretools + " -C /root";
    RunProcess(process.c_str(), nullptr);
    RunProcess("perl /root/vmware-tools-distrib/vmware-install.pl", nullptr); // "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"


    std::ofstream output("/etc/systemd/system/vmwaretools.service");
    if(!output.is_open())
    {
        PrintError("Failed to open file [%s]\n", "/etc/systemd/system/vmwaretools.service");
        return 0;
    }


    const std::string content = "[Unit]\n"
                                "Description=VMWare Tools Daemon\n"
                                "\n"
                                "[Service]\n"
                                "ExecStart=/etc/init.d/vmware-tools start\n"
                                "ExecStop=/etc/init.d/vmware-tools stop\n"
                                "PIDFile=/var/lock/subsys/vmware\n"
                                "TimeoutSec=0\n"
                                "RemainAfterExit=yes\n"
                                "\n"
                                "[Install]\n"
                                "WantedBy=multi-user.target";
    output << content;
    output.close();

    RunProcess("systemctl start vmwaretools.service", nullptr);
    RunProcess("systemctl enable vmwaretools.service", nullptr);
    RunProcess("rm -r /root/vmware-tools-distrib", nullptr);
    RunProcess("reboot", nullptr);
}
