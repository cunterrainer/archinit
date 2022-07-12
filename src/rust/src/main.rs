#![allow(non_snake_case)]

mod ProcessM;

use std::io::Write;
use colored::Colorize;
use ProcessM::*;


fn CreateFDiskInstaller()
{
    let file = std::fs::File::create("__fdiskInstaller__.sh");
    if !file.is_ok() {
        println!("{}", format!("[Arch install script]<Error trying to create [__fdiskInstaller__.sh]>").red());
        return()
    }
    let data = "#!/bin/bash\nfdisk /dev/sda <<EOF\ng\nn\n1\n\n+550M\nn\n2\n\n+2G\nn\n3\n\n\nt\n1\n1\nt\n2\n19\nw\nEOF";
    let err = file.unwrap().write_all(data.as_bytes());
    if !err.is_ok() {
        println!("{}", format!("[Arch install script]<Error trying to write to [__fdiskInstaller__.sh]>").red());
        return()
    }
    
    let process: Process = Process::Init("chmod", "+x __fdiskInstaller__.sh");
    process.Exec();
}


fn CopyFile(from: &str, to: &str) -> bool {
    let err = std::fs::copy(from, to);
    if err.is_ok() {
        println!("{}", format!("[Arch install script]<Copied [{}] to [{}]>", from, to).green());
        let process = Process::Init("chmod", String::from(String::from("+x ") + to).as_str());
        process.Exec();
        return true
    }
    println!("{}", format!("[Arch install script]<Failed to copy [{}] to [{}]>", from, to).red());
    return false
}


fn main() {
    //println!("{}", currentDir.display());
    //CreateFDiskInstaller();
    //std::fs::remove_file("__fdiskInstaller__.sh");
    
    //let cmds = vec!("1\\nn | yay -S spotify");
    //let mut process = std::process::Command::new("printf")
    //    .args(cmds)
    //    .stdin(std::process::Stdio::piped())
    //    .spawn()
    //    .unwrap();

    ////printf "o\nn\np\n1\n\n\nw\n" | sudo fdisk "$dev"
    ////process.stdin.as_mut().unwrap().write_all(b"1\n1\n");

    //process.wait();
    //std::process::exit(1);

    if !nix::unistd::Uid::effective().is_root()
    {
        println!("{}", format!("You must run this executable with root permissions").red());
        std::process::exit(1);
    }
    println!("Running as root");
    

    let processes: Vec<Process> = vec![
        //Process::Init("loadkeys", "de-latin1"),
        Process::Init("timedatectl", "set-ntp true"),
        //Process::Init("timedatectl", "status"),
        Process::Init("./__fdiskInstaller__.sh", ""),
        //Process::Init("fdisk", "-l"),
        //Process::InitInfo("fdisk", "/dev/sda", "Manual user inputs:\n\tg-n-1-default-+550M-n-2-default-+2G-n-3-default-default-t-1-1-t-2-19-w"),
        Process::Init("mkfs.fat", "-F32 /dev/sda1"),
        Process::Init("mkswap", "/dev/sda2"),
        Process::Init("swapon", "/dev/sda2"),
        Process::Init("mkfs.ext4", "/dev/sda3"),
        Process::Init("mount", "/dev/sda3 /mnt"),
        Process::Init("pacstrap", "/mnt base linux linux-firmware"),
        Process::Init("genfstab", "-U /mnt >> /mnt/etc/fstab"),
        //Process::Init("pacman", "-S arch-install-scripts"),
        Process::Init("cp", "archinit/chroot_script /mnt/root/chroot_script"),
        Process::Init("chmod", "+x /mnt/root/chroot_script"),
        Process::Init("arch-chroot", "/mnt /root/chroot_script"),
        //Process::Init("ln", "-sf /usr/share/zoneinfo/Europe/Berlin /etc/localtime"),
        //Process::Init("hwclock", "--systohc"),
        //Process::Init("pacman", "-S nano"),
        //Process::InitInfo("nano", "/etc/locale.gen", "unmark en_US.UTF-8 UTF-8 and save"),
        //Process::Init("locale-gen", ""),
        //Process::InitInfo("nano", "/etc/locale.conf", "LANG=en_US.UTF-8"),
        //Process::Init("locale-gen", ""),
        //Process::InitInfo("nano", "/etc/hostname", "Your custom hostname (vyx for me)"),
        //Process::InitInfo("nano", "/etc/hosts", "2 lines should already be there\n127.0.0.1\tlocalhost\n::1\t\tlocalhost\n127.0.1.1\tvyx.localdomain <hostname>"),
        //Process::InitInfo("passwd", "", "Set root password"),
        //Process::Init("useradd", "-m vyx"),
        //Process::InitInfo("passwd", "vyx", "Set password for user vyx"),
        //Process::Init("usermod", "-aG wheel,audio,video,optical,storage vyx"),
        //Process::Init("pacman", "-S sudo"),
        //Process::InitInfo("nano", "/etc/sudoers", "Find:\n'# Uncomment to allow members of group wheel to execute any command'\n'# %wheel ALL=(ALL) ALL'\nUncomment the second line"),
        //Process::Init("pacman", "-S grub"),
        //Process::Init("pacman", "-S efibootmgr dosfstools os-prober mtools"),
        //Process::Init("mkdir", "/boot/EFI"),
        //Process::Init("mount", "/dev/sda1 /boot/EFI"),
        //Process::Init("grub-install", "--target=x86_64-efi --bootloader-id=grub_uefi --recheck"),
        //Process::Init("grub-mkconfig", "-o /boot/grub/grub.cfg"),
        //Process::Init("pacman", "-S networkmanager"),
        //Process::Init("pacman", "-S git"),
        //Process::Init("systemctl", "enable NetworkManager"),
        //Process::Init("exit", ""),
        Process::Init("umount", "-l /mnt"),
        Process::Init("reboot", ""),
    ];

    CreateFDiskInstaller();
    //CopyFile("archinit/chroot_script", "mnt/root/chroot_script");
    processes.iter().for_each(|p| p.Exec());
    if !std::fs::remove_file("__fdiskInstaller__.sh").is_ok() {
        println!("{}", format!("[Arch installer script]<Failed to remove __fdiskInstaller__.sh>").red());
    }
}
