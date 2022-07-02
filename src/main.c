#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "Process.h"

#define PRINT_ERROR(message) printf(RED AIS "<Error: %s>" NRM_ENDL, message);
#define FDISK_PATH "__fdiskInstaller__.sh"

bool CreateFDiskInstaller()
{
    FILE *f = fopen(FDISK_PATH, "w");
    if (f == NULL)
    {
        PRINT_ERROR("couldn't open " FDISK_PATH);
        return false;
    }

    const char *text = "#!/bin/bash\nfdisk /dev/sda <<EOF\ng\nn\n1\n\n+550M\nn\n2\n\n+2G\nn\n3\n\n\nt\n1\n1\nt\n2\n19\nw\nEOF";
    if (fprintf(f, "%s", text) < 0) {
        PRINT_ERROR("writing to file [" FDISK_PATH "] wasn't successfull");
        return false;
    }
    printf(GRN AIS "<Successfully created [%s]" NRM_ENDL, FDISK_PATH);

    if (fclose(f) != 0) {
        PRINT_ERROR("file handle [" FDISK_PATH "] couldn't be closed");
        return false;
    }

    RunProcess("chmod +x " FDISK_PATH, NULL);
    return true;
}


void DeleteFDiskInstaller()
{
    if (remove(FDISK_PATH) != 0)
        PRINT_ERROR("unable to remove file [" FDISK_PATH "]");
}


int main()
{
    const char* commands[] = 
    {
        "pacman -Syy",
        "pacman -Sy",
        "loadkeys de-latin1",
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
        "chmod +x /mnt/root/chroot_script",
        "arch-chroot /mnt /root/chroot_script", // /mnt/root/chroot real path
        "umount -l /mnt",
        "reboot"
    };
    uint32_t commandsLength = sizeof(commands) / sizeof(const char*);

    if(!CreateFDiskInstaller())
        return EXIT_FAILURE;

    for(uint32_t i = 0; i < commandsLength; ++i)
    {
        RunProcess(commands[i], NULL);
    }

    DeleteFDiskInstaller();
    return EXIT_SUCCESS;
}
