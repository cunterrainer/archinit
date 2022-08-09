#ifndef ARCH_INSTALLER_H
#define ARCH_INSTALLER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "Process.h"
#include "Output.h"

#define FDISK_PATH "__fdiskInstaller__.sh"

static bool CreateFDiskInstaller()
{
    FILE *f = fopen(FDISK_PATH, "w");
    if (f == NULL)
    {
        PrintFileOpenError(FDISK_PATH);
        return false;
    }

    const char *text = "#!/bin/bash\nfdisk /dev/sda <<EOF\ng\nn\n1\n\n+550M\nn\n2\n\n+2G\nn\n3\n\n\nt\n1\n1\nt\n2\n19\nw\nEOF";
    if (fprintf(f, "%s", text) < 0) {
        PrintError("writing to file [" FDISK_PATH "]");
        return false;
    }
    PrintSuccess("created [" FDISK_PATH "]");

    if (fclose(f) != 0) {
        PrintFileCloseError(FDISK_PATH);
        return false;
    }

    RunProcess("chmod +x " FDISK_PATH, NULL);
    return true;
}


static void DeleteFDiskInstaller()
{
    if (remove(FDISK_PATH) != 0)
        PrintRemoveError(FDISK_PATH);
}


static int InstallArch(char** argv)
{
    const char* chroot_cmd = "arch-chroot /mnt /root/chroot_script ";
    const size_t chroot_size = strlen(chroot_cmd);
    const size_t arg0 = strlen(argv[1]);
    const size_t arg1 = strlen(argv[2]);
    const size_t arg2 = strlen(argv[3]);

    char* chroot = calloc(chroot_size + arg0 + arg1 + arg2 + 3, sizeof(char));
    strcpy(chroot, chroot_cmd);
    strcpy(chroot + chroot_size, argv[1]);
    strcpy(chroot + chroot_size + arg0, " ");
    strcpy(chroot + chroot_size + arg0 + 1, argv[2]);
    strcpy(chroot + chroot_size + arg0 + 1 + arg1, " ");
    strcpy(chroot + chroot_size + arg0 + 1 + arg1 + 1, argv[3]);

    const char* commands[17] = 
    {
        "pacman -Syy",
        "pacman -Sy",
        "timedatectl set-ntp true",
        "./" FDISK_PATH,
        "mkfs.fat -F32 /dev/sda1",
        "mkswap /dev/sda2",
        "swapon /dev/sda2",
        "mkfs.ext4 /dev/sda3",
        "mount /dev/sda3 /mnt",
        "pacstrap /mnt base linux linux-firmware",
        "genfstab -U /mnt >> /mnt/etc/fstab",
        "cp archinit/chroot_script /mnt/root/chroot_script",
        "chmod +x /mnt/root/chroot_script"

        //"arch-chroot /mnt /root/chroot_script", // /mnt/root/chroot real path
        //"rm /root/chroot_script",
        //"umount -l /mnt",
        //"reboot"
    };
    commands[13] = chroot;
    commands[14] = "rm /root/chroot_script";
    commands[15] = "rm /mnt/root/chroot_script";
    commands[16] = "umount -l /mnt";
    //commands[16] = "reboot";
    uint32_t commandsLength = sizeof(commands) / sizeof(const char*);

    if(!CreateFDiskInstaller())
    {
        free(chroot);
        return EXIT_FAILURE;
    }

    for(uint32_t i = 0; i < commandsLength; ++i)
    {
        RunProcess(commands[i], NULL);
    }

    DeleteFDiskInstaller();
    free(chroot);
    return EXIT_SUCCESS;
}

#endif
