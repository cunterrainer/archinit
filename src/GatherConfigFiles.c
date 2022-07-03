#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "Process.h"
#include "Output.h"

#define CONFIG_FOLDER "config_files/"
#define WP_FOLDER_HOME "/Pictures/wallpaper"
#define WP_FOLDER_CONF CONFIG_FOLDER "wallpaper"
#define ROFI_THEMES_DR "/usr/share/rofi/themes"
#define ROFI_THEMES_CF CONFIG_FOLDER "rofi_themes"


char* const GetConfigFolder(const char* const homePath)
{
    const char* const cfg = "/.config/";
    // homePath is null terminated
    size_t hpLength = strlen(homePath);
    size_t cfgLength = strlen(cfg);
    char* const cfgFolder = calloc(hpLength + cfgLength + 1, sizeof(char));

    memcpy(cfgFolder, homePath, hpLength);
    memcpy(cfgFolder + hpLength, cfg, cfgLength);
    return cfgFolder;
}


char* const GetLastSubStrCfg(const char* const str, char c)
{
    int32_t cfgLen = strlen(CONFIG_FOLDER);

    int32_t strLen = strlen(str); // is null terminated
    for(int32_t i = strLen - 1; i >= 0; --i)
    {
        if(str[i] == c) {
            char* const substr = calloc(cfgLen + strLen - i, sizeof(char));
            memcpy(substr, CONFIG_FOLDER, cfgLen);
            memcpy(substr + cfgLen, str + i + 1, strLen - i);
            return substr;
        }
    }

    char* const substr = calloc(strLen + cfgLen + 1, sizeof(char));
    memcpy(substr, CONFIG_FOLDER, cfgLen);
    memcpy(substr + cfgLen, str, strLen);
    return substr;
}


void CopyWallpaper(const char* const homePath)
{
    const char* const cpCmdRaw = "sudo cp -r ";
    size_t cpCmdRawLen = strlen(cpCmdRaw);
    size_t hpLen = strlen(homePath);
    size_t wpfhLen = strlen(WP_FOLDER_HOME);
    size_t wpfcLen = strlen(WP_FOLDER_CONF);
    char* const wpDir = calloc(cpCmdRawLen + hpLen + wpfhLen + wpfcLen + 2, sizeof(char));
    memcpy(wpDir, cpCmdRaw, cpCmdRawLen);
    memcpy(wpDir + cpCmdRawLen, homePath, hpLen);
    memcpy(wpDir + cpCmdRawLen + hpLen, WP_FOLDER_HOME, wpfhLen);
    wpDir[cpCmdRawLen + hpLen + wpfhLen] = ' ';
    memcpy(wpDir + cpCmdRawLen + hpLen + wpfhLen + 1, WP_FOLDER_CONF, wpfcLen);

    RunProcess(wpDir, NULL);
    free(wpDir);
}


int main()
{
    const char* const files[] = 
    {
        "i3/config",
        "i3blocks/i3blocks.conf",
        "i3status/i3status.conf",
        "kitty",
        "scripts",
        "rofi/config.rasi",
        "nvim/init.vim",
        "nvim/themes",
        "nvim/lua"
    };
    const size_t filesCount = sizeof(files) / sizeof(const char*);

    RunProcess("sudo rm -r " CONFIG_FOLDER, NULL);
    mkdir(CONFIG_FOLDER, 0777);
    const char* const homePath = getenv("HOME");
    
    char* const cfgFolder = GetConfigFolder(homePath);
    size_t cfgFolderLen = strlen(cfgFolder);

    for(size_t i = 0; i < filesCount; ++i) 
    {
        size_t fileLen = strlen(files[i]);
        char* const substrSrc = calloc(cfgFolderLen + fileLen + 1, sizeof(char));
        memcpy(substrSrc, cfgFolder, cfgFolderLen);
        memcpy(substrSrc + cfgFolderLen, files[i], fileLen);

        char* const substrDest = GetLastSubStrCfg(files[i], '/');

        const char* const cpCommandRaw = "sudo cp -r ";
        size_t cpCommandLen = strlen(cpCommandRaw);
        size_t substrDestLen = strlen(substrDest);
        size_t substrSrcLen = strlen(substrSrc);

        char* const cpCmd = calloc(cpCommandLen + substrDestLen + substrSrcLen + 2, sizeof(char));
        memcpy(cpCmd, cpCommandRaw, cpCommandLen);
        memcpy(cpCmd + cpCommandLen, substrSrc, substrSrcLen);
        cpCmd[cpCommandLen + substrSrcLen] = ' ';
        memcpy(cpCmd + cpCommandLen + substrSrcLen + 1, substrDest, substrDestLen);

        RunProcess(cpCmd, NULL);
        free(substrDest);
        free(substrSrc);
        free(cpCmd);
    }
    free(cfgFolder);

    CopyWallpaper(homePath);


    const char* const cpCmdRaw = "sudo cp -r ";
    size_t cpCmdRawLen = strlen(cpCmdRaw);
    size_t rofiThDirLen = strlen(ROFI_THEMES_DR);
    size_t rofiThCfgLen = strlen(ROFI_THEMES_CF);

    char* const cpCmd = calloc(cpCmdRawLen + rofiThDirLen + rofiThCfgLen + 1, sizeof(char));
    memcpy(cpCmd, cpCmdRaw, cpCmdRawLen);
    memcpy(cpCmd + cpCmdRawLen, ROFI_THEMES_DR, rofiThDirLen);
    cpCmd[cpCmdRawLen + rofiThDirLen] = ' ';
    memcpy(cpCmd + cpCmdRawLen + rofiThDirLen + 1, ROFI_THEMES_CF, rofiThCfgLen);

    RunProcess(cpCmd, NULL);
    free(cpCmd);
}
