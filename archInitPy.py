import os

def ExecuteCommand(cmd: str) -> None:
    print("[Arch install script]: " + cmd)
    os.system(cmd)


def main():
    commands:list = [
            "loadkeys de-latin1",
            "timedatectl set-ntp true",
            "timedatectl status",
            "fdisk -l",
            "fdisk /dev/sda",
            "mkfs.fat -F32 /dev/sda1",
            "mkswap /dev/sda2",
            "swapon /dev/sda2",
            "mkfs.ext4 /dev/sda3",
            "mount /dev/sda3 /mnt",
            "pacstrap /mnt base linux linux-firmware",
            "genfstab -U /mnt >> /mnt/etc/fstab",
            "arch-chroot /mnt",
            "ln -sf /usr/share/zoneinfo/Europe/Berlin /etc/localtime"
            ]
    for cmd in commands:
        ExecuteCommand(cmd)
    

if __name__ == "__main__":
    main()
