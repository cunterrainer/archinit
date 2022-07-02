#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Process.h"

bool LinesInFile(const char* filePath, int32_t* const linesCount, int32_t* const longestLine)
{
    FILE *fp = fopen(filePath, "r");
    if (fp == NULL)
    {
        printf(RED AIS "<Error: failed to open file [%s] for counting lines" NRM_ENDL, filePath);
        return false;
    }
  
    char c;
    int32_t currentLine = 0;
    *linesCount = 0;
    *longestLine = 0;
    while((c = getc(fp)) && c != EOF)
    {
        if (c == '\n')
        {
            ++*linesCount;
            if(currentLine > *longestLine)
                *longestLine = currentLine;
            currentLine = 0;
        }
        ++currentLine;
    }
  
    fclose(fp);
    return true;
}


// if fpw == NULL fp = fpw
void ReplaceLineInFile(const char* const filePath, const char* const filePathWrite, const char* const toRep, const char* const repWith)
{
    int32_t linesCount;
    int32_t longestLine;
    if(LinesInFile(filePath, &linesCount, &longestLine) == false) return;
    longestLine += 1; // null termination char
    
    // allocate array to hold lines
    char** lines = (char**)malloc(linesCount * sizeof(char*));
    if(lines == NULL) return;

    FILE* fp = fopen(filePath, "r");
    if(fp == NULL) return;

    for(int32_t i = 0; i < linesCount; ++i)
    {
        lines[i] = (char*)malloc(longestLine * sizeof(char));
        if(lines[i] == NULL) return;
        memset(lines[i], '\0', longestLine);
        fgets(lines[i], longestLine, fp);

        int ret = strcmp(lines[i], toRep);
        if(ret == 0 && strnlen(repWith, longestLine) <= longestLine) {
            strncpy(lines[i], repWith, longestLine);
        }
    }
    fclose(fp);
   
    if(filePathWrite == NULL)
        fp = fopen(filePath, "w");
    else
        fp = fopen(filePathWrite, "w");

    for(int32_t i = 0; i < linesCount; ++i)
    {
        fprintf(fp, "%s", lines[i]);
        free((void*)lines[i]);
    }

    free((void*)lines);
    fclose(fp);
}


void WriteLineToFile(const char* filePath, const char* line)
{
    FILE* fp = fopen(filePath, "w");
    fprintf(fp, "%s", line);
    fclose(fp);
}


void WriteHostsFile()
{
    FILE* fp = fopen("/etc/hosts", "a");
    fprintf(fp, "\n");
    fprintf(fp, "127.0.0.1\tlocalhost\n");
    fprintf(fp, "::1\t\tlocalhost\n");
    fprintf(fp, "127.0.1.1\tvyx.localdomain vyx");
    fclose(fp);
}


#define SUDOERS_TMP_FILE "/etc/init_script_sudo_tmp"
#define SUDOERS_PATH "/etc/sudoers"
#define SUDOERS_LOCK_PATH "/etc/sudoers.tmp"
void EditSudoersFile()
{
    FILE* fp;
    while((fp = fopen(SUDOERS_LOCK_PATH, "r")) != NULL){ // file exists
        fclose(fp);
    }
    fp = fopen(SUDOERS_LOCK_PATH, "w");
    fclose(fp);

    ReplaceLineInFile(SUDOERS_PATH, SUDOERS_TMP_FILE, "# %wheel ALL=(ALL:ALL) ALL\n", "%wheel ALL=(ALL:ALL) ALL\n");

    int status = RunProcess("visudo -c -f " SUDOERS_TMP_FILE, NULL);
    if(status != 0)
    {
        printf(RED AIS "<Error: visudo checking didn't pass [" SUDOERS_TMP_FILE "]" NRM_ENDL);
    }
    else
    {
        RunProcess("cp " SUDOERS_TMP_FILE " " SUDOERS_PATH, NULL);
    }

    remove(SUDOERS_TMP_FILE);
    remove(SUDOERS_LOCK_PATH);
}


int main()
{
    RunProcess("ln -sf /usr/share/zoneinfo/Europe/Berlin /etc/localtime", NULL);
    RunProcess("hwclock --systohc", NULL);
    RunProcess("pacman -S nano", "y");
    ReplaceLineInFile("/etc/locale.gen", NULL, "#en_US.UTF-8 UTF-8  \n", "en_US.UTF-8 UTF-8  \n");
    RunProcess("locale-gen", NULL);
    WriteLineToFile("/etc/locale.conf", "LANG=en_US.UTF-8");
    RunProcess("locale-gen", NULL);
    RunProcess("passwd", "1234\n1234");
    WriteLineToFile("/etc/hostname", "vyx");
    WriteHostsFile();
    RunProcess("useradd -m vyx", NULL);
    RunProcess("passwd vyx", "1234\n1234");
    RunProcess("usermod -aG wheel,audio,video,optical,storage vyx", NULL);
    RunProcess("pacman -S sudo grub efibootmgr dosfstools os-prober mtools", "y");
    EditSudoersFile();
    RunProcess("mkdir /boot/EFI", NULL);
    RunProcess("mount /dev/sda1 /boot/EFI", NULL);
    RunProcess("grub-install --target=x86_64-efi --bootloader-id=grub_uefi --recheck", NULL);
    RunProcess("grub-mkconfig -o /boot/grub/grub.cfg", NULL);
    RunProcess("pacman -S networkmanager git", "y");
    RunProcess("systemctl enable NetworkManager", NULL);
    RunProcess("exit", NULL);
}
