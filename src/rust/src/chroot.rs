#![allow(non_snake_case)]

mod ProcessM;

use ProcessM::*;

fn main()
{
    let processes: Vec<Process> = vec![
        Process::Init("ln", "-sf /usr/share/zoneinfo/Europe/Berlin /etc/localtime"),
        Process::Init("hwclock", "--systohc"),
        Process::Init("pacman", "-S nano"),
        Process::InitInfo("nano", "/etc/locale.gen", "unmark en_US.UTF-8 UTF-8 and save"),
        Process::Init("locale-gen", ""),
        Process::InitInfo("nano", "/etc/locale.conf", "LANG=en_US.UTF-8"),
        Process::Init("locale-gen", ""),
        Process::InitInfo("nano", "/etc/hostname", "Your custom hostname (vyx for me)"),
        Process::InitInfo("nano", "/etc/hosts", "2 lines should already be there\n127.0.0.1\tlocalhost\n::1\t\tlocalhost\n127.0.1.1\tvyx.localdomain <hostname>"),
        Process::InitInfo("passwd", "", "Set root password"),
        Process::Init("useradd", "-m vyx"),
        Process::InitInfo("passwd", "vyx", "Set password for user vyx"),
        Process::Init("usermod", "-aG wheel,audio,video,optical,storage vyx"),
        Process::Init("pacman", "-S sudo"),
        Process::InitInfo("nano", "/etc/sudoers", "Find:\n'# Uncomment to allow members of group wheel to execute any command'\n'# %wheel ALL=(ALL) ALL'\nUncomment the second line"),
        Process::Init("pacman", "-S grub"),
        Process::Init("pacman", "-S efibootmgr dosfstools os-prober mtools"),
        Process::Init("mkdir", "/boot/EFI"),
        Process::Init("mount", "/dev/sda1 /boot/EFI"),
        Process::Init("grub-install", "--target=x86_64-efi --bootloader-id=grub_uefi --recheck"),
        Process::Init("grub-mkconfig", "-o /boot/grub/grub.cfg"),
        Process::Init("pacman", "-S networkmanager"),
        Process::Init("pacman", "-S git"),
        Process::Init("systemctl", "enable NetworkManager"),
        Process::Init("exit", ""),
    ];

    processes.iter().for_each(|p| p.Exec());
}
