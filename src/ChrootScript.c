#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Process.h"
#include "Output.h"

bool LinesInFile(const char* filePath, int32_t* const linesCount, int32_t* const longestLine)
{
    FILE *fp = fopen(filePath, "r");
    if (fp == NULL)
    {
        PrintFileOpenError(filePath);
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
  
    if(fclose(fp) != 0) {
        PrintFileCloseError(filePath);
        return false;
    }

    return true;
}


// if fpw == NULL fp = fpw
bool ReplaceLineInFile(const char* const filePath, const char* const filePathWrite, const char* const toRep, const char* const repWith)
{
    int32_t linesCount;
    int32_t longestLine;
    if(LinesInFile(filePath, &linesCount, &longestLine) == false) return false;
    longestLine += 1; // null termination char
    
    // allocate array to hold lines
    char** lines = (char**)malloc(linesCount * sizeof(char*));
    if(lines == NULL) return false;

    FILE* fp = fopen(filePath, "r");
    if(fp == NULL) {
        PrintFileOpenError(filePath);
        return false;
    }

    for(int32_t i = 0; i < linesCount; ++i)
    {
        lines[i] = (char*)malloc(longestLine * sizeof(char));
        if(lines[i] == NULL) return false;
        memset(lines[i], '\0', longestLine);
        fgets(lines[i], longestLine, fp);

        int ret = strcmp(lines[i], toRep);
        if(ret == 0 && strnlen(repWith, longestLine) <= longestLine) {
            strncpy(lines[i], repWith, longestLine);
        }
    }
    if(fclose(fp) != 0) {
        PrintFileCloseError(filePath);
        return false;
    }
   
    if(filePathWrite == NULL) {
        fp = fopen(filePath, "w");
        if(fp == NULL) {
            PrintFileOpenError(filePath);
            return false;
        }
    }
    else {
        fp = fopen(filePathWrite, "w");
        if(fp == NULL) {
            PrintFileOpenError(filePathWrite);
            return false;
        }
    }

    for(int32_t i = 0; i < linesCount; ++i)
    {
        fprintf(fp, "%s", lines[i]);
        free((void*)lines[i]);
    }

    free((void*)lines);
    
    if(filePathWrite == NULL) 
    {
        if(fclose(fp) != 0) {
            PrintFileCloseError(filePath);
            return false;
        }
    }
    else 
    {
        if(fclose(fp) != 0) {
            PrintFileCloseError(filePathWrite);
            return false;
        }
    }

    return true;
}


bool WriteLineToFile(const char* filePath, const char* line)
{
    FILE* fp = fopen(filePath, "w");
    if(fp == NULL) {
        PrintFileOpenError(filePath);
        return false;
    }

    fprintf(fp, "%s", line);

    if(fclose(fp) != 0) {
        PrintFileCloseError(filePath);
        return false;
    }
    return true;
}


char* CreateStr(const char* str1, const char* str2)
{
    const size_t dest_len = strlen(str1);
    const size_t src_len = strlen(str2);

    char* strn = calloc(dest_len + src_len + 1, sizeof(char));
    strcpy(strn, str1);
    strcpy(strn + dest_len, str2);

    return strn;
}


bool WriteHostsFile(const char* hostname)
{
    #define HOST_PATH "/etc/hosts"
    FILE* fp = fopen(HOST_PATH, "a");
    if(fp == NULL) {
        PrintFileOpenError(HOST_PATH);
        return false;
    }

    fprintf(fp, "\n");
    fprintf(fp, "127.0.0.1\tlocalhost\n");
    fprintf(fp, "::1\t\tlocalhost\n");
    fprintf(fp, "127.0.1.1\t%s.localdomain %s", hostname, hostname);
    
    if(fclose(fp) != 0) {
        PrintFileCloseError(HOST_PATH);
        return false;
    }
    return true;
}



bool EditSudoersFile()
{
    #define SUDOERS_TMP_FILE "/etc/init_script_sudo_tmp"
    #define SUDOERS_PATH "/etc/sudoers"
    #define SUDOERS_LOCK_PATH "/etc/sudoers.tmp"

    FILE* fp;
    while((fp = fopen(SUDOERS_LOCK_PATH, "r")) != NULL){ // file exists
        if(fclose(fp) != 0) {
            PrintError("file handle [" SUDOERS_LOCK_PATH "] couldn't be closed (Current task: Trying if it still exists)");
            return false;
        }
    }
    fp = fopen(SUDOERS_LOCK_PATH, "w");
    if(fp == NULL) {
        PrintFileOpenError(SUDOERS_LOCK_PATH);
        return false;
    }

    if(fclose(fp) != 0) {
        PrintFileCloseError(SUDOERS_LOCK_PATH);
        return false;
    }

    ReplaceLineInFile(SUDOERS_PATH, SUDOERS_TMP_FILE, "# %wheel ALL=(ALL:ALL) ALL\n", "%wheel ALL=(ALL:ALL) ALL\n");

    int status = RunProcess("visudo -c -f " SUDOERS_TMP_FILE, NULL);
    if(status != 0)
    {
        PrintError("visudo checking didn't pass [" SUDOERS_TMP_FILE "]");
    }
    else
    {
        RunProcess("cp " SUDOERS_TMP_FILE " " SUDOERS_PATH, NULL);
    }

    if(remove(SUDOERS_TMP_FILE) != 0) {
        PrintRemoveError(SUDOERS_TMP_FILE);
    }
    if(remove(SUDOERS_LOCK_PATH) != 0) {
        PrintRemoveError(SUDOERS_LOCK_PATH);
    }
    return true;
}


char* concatPw(const char* pw)
{
    const size_t pw_size = strlen(pw);
    char* new_pw = calloc(pw_size * 2 + 2, sizeof(char));
    strcpy(new_pw, pw);
    strcpy(new_pw + strlen(pw), "\n");
    strcpy(new_pw + strlen(pw) + 1, pw);

    return new_pw;
}



int main(int argc, char** argv)
{
    char* root_pw = concatPw(argv[1]);
    char* user_pw = concatPw(argv[3]);
    const char* user_name = argv[2];
    const size_t user_size = strlen(user_name);

    char* useradd_cmd = CreateStr("useradd -m ", user_name);
    char* pw_cmd = CreateStr("passwd ", user_name);
    char* usermod_cmd = CreateStr("usermod -aG wheel,audio,video,optical,storage ", user_name);

    RunProcess("ln -sf /usr/share/zoneinfo/Europe/Berlin /etc/localtime", NULL);
    RunProcess("hwclock --systohc", NULL);
    RunProcess("pacman -S nano", "y");
    ReplaceLineInFile("/etc/locale.gen", NULL, "#en_US.UTF-8 UTF-8  \n", "en_US.UTF-8 UTF-8  \n");
    RunProcess("locale-gen", NULL);
    WriteLineToFile("/etc/locale.conf", "LANG=en_US.UTF-8");
    RunProcess("locale-gen", NULL);
    RunProcess("passwd", root_pw); // 1234\n1234
    WriteLineToFile("/etc/hostname", user_name);
    WriteHostsFile(user_name);
    RunProcess(useradd_cmd, NULL);
    RunProcess(pw_cmd, user_pw); // 1234\n1234
    RunProcess(usermod_cmd, NULL);
    RunProcess("pacman -S sudo grub efibootmgr dosfstools os-prober mtools", "y");
    EditSudoersFile();
    RunProcess("mkdir /boot/EFI", NULL);
    RunProcess("mount /dev/sda1 /boot/EFI", NULL);
    RunProcess("grub-install --target=x86_64-efi --bootloader-id=grub_uefi --recheck", NULL);
    RunProcess("grub-mkconfig -o /boot/grub/grub.cfg", NULL);
    RunProcess("pacman -S networkmanager git", "y");
    RunProcess("systemctl enable NetworkManager", NULL);
    
    free(root_pw);
    free(user_pw);
    free(useradd_cmd);
    free(pw_cmd);
    free(usermod_cmd);
    RunProcess("exit", NULL);
}
