#include "ArchInstaller.h"

int main()
{
    if(geteuid() != 0) {
        PrintError("You don't have root privileges");
        return EXIT_FAILURE;
    }

    return InstallArch();
}
