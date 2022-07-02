#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define NRM "\x1B[0m"
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"

#define FDISK_PATH "__fdiskInstaller__.sh"
#define INM "[Arch installer script]"

#define PRINT_ERROR(message) printf(RED INM "<Error: %s>\n" NRM, message);


void RunProcess(const char* process)
{
    fflush(stdout);
    FILE* p = popen(process, "w");
    if(p == NULL)
    {
        printf(RED INM "<Error: couldn't start subprocess [%s]\n" NRM, process);
        return;
    }

    //fprintf(p, "hello"); // write to stdin

    if(pclose(p) == -1) {
        printf(RED INM "<Error: failed to close subprocess [%s]\n" NRM, process);
    }
}


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
        PRINT_ERROR("writing to file wasn't successfull");
        return false;
    }

    if (fclose(f) != 0) {
        PRINT_ERROR("file handle couldn't be closed");
        return false;
    }

    RunProcess("chmod +x " FDISK_PATH);
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
        "loadkeys de-latin1",
        "timedatectl set-ntp true",
        "./test.sh", // replace with fdisk
        "mkfs.fat -F32 /dev/sda1",
        "mkswap /dev/sda2",
        "swapon /dev/sda2",
        "mkfs.ext4 /dev/sda3",
        "mount /dev/sda3 /mnt",
        "pacstrap /mnt base linux linux-firmware",
        "genfstab -U /mnt >> /mnt/etc/fstab",
        "arch-chroot /mnt /root/chroot" // /mnt/root/chroot real path
    };
    uint32_t commandsLength = sizeof(commands) / sizeof(const char*);

    if(!CreateFDiskInstaller())
        return EXIT_FAILURE;

    DeleteFDiskInstaller();
    RunProcess("python t.py");
    return EXIT_SUCCESS;
}
