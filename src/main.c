#include "ArchInstaller.h"

int main(int argc, char** argv)
{
    if(geteuid() != 0) {
        PrintError("You don't have root privileges");
        return 0;
    }

    if(argc != 4)
    {
        PrintError("Please provide all information\n%s [root password] [admin user] [admin user password]\n", argv[0]);
        return 0;
    }

    return InstallArch(argv);
}
