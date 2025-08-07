# ![PS4](https://img.shields.io/badge/-PS4-003791?style=flat&logo=PlayStation) Linux Payloads for FW 9.00 - 12.02 
(Southbridge: Aeolia & Belize(2) & Baikal)

**Linux-Payloads** kexec for PlayStation 4.

# Supported Firmwares

*   FW 9.00 ✅ 
*   FW 9.03 / 9.04 ✅ 
*   FW 9.50 / 9.51 / 9.60 ✅ 
*   FW 10.00 / 10.01 ✅ 
*   FW 10.50 / 10.70 / 10.71 ✅ 
*   FW 11.00 ✅ 
*   FW 11.50 / 11.52 ✅ (not tested yet) 
*   FW 12.00 / 12.02 ✅ 

Be sure you have saved your LUA Games license best is a 1:1 clone from your ps4 drive!! I take no responsibility if you lose your jailbreak!!!  


## Compile
    git clone https://github.com/ps4gentoo/ps4-linux-payloads
    cd ps4-linux-payloads/linux
    make

## Info
the internal path by default is at: /data/linux/boot  the rest is coming from the initramfs.cpio.gz init configuration
so you can go into the rescue shell without a usb stick just upload the bzImage and initramfs.cpio.gz over ftp to your PS4 Drive
/data/linux/boot/[bzimage,initramfs.cpio.gz] and of course it will work too with a USB / HDD Drive.  USB have highest prio so if a USB is connected he will  use this bzImage and initramfs.cpio.gz from there 

bootargs.txt you can also add a textfile for changing the cmdline.
vram.txt with this you can change the vram over a textfile.

## Note 
* Don`t use PRO Payloads for Phat or Slim. 

* if you need UART just add this to the cmdline i have disabled .... just for now on newer Kernel it doesnt work.
Aeolia/Belize: ``console=uart8250,mmio32,0xd0340000``
Baikal: ``console=uart8250,mmio32,0xC890E000``


## Credits
* marcan, shuffle2, eeply, rancido, valeryy, ethylamine, Joonie86 (Linux)
* sleirsgoevy (for the script and better exploit FW 672) 
* AlAzif / KiwiDog / Specter / Celesteblue / ChendoChap / zecoxao / SocraticBliss / ctn123 (Exploit and Fun Stuff for the Console)
* bestpig / EchoStretch / EinTim23 (Offsets)
* others ... 


# For Firmware 5.05 and 6.72 use the "old" Linux Loader Source.
1 Loader: https://github.com/ps4gentoo/PS4-Linux-Loader

2 Kexec: https://github.com/ps4gentoo/ps4-kexec

3 SDK: https://github.com/ps4boot/ps4-payload-sdk
