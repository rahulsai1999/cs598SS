# !/bin/bash

curl -O --location https://cytranet.dl.sourceforge.net/project/collectl/collectl/collectl-4.3.1/collectl-4.3.1.src.tar.gz
tar xfvz collectl-4.3.1.src.tar.gz
cd collectl-4.3.1
./INSTALL

#     1  ls
#     2  sudo lsblk
#     3  sudo mkfs.ext4 /dev/nvme0n1p4
#     4  sudo mkdir /mnt/newpartition
#     5  sudo mount /dev/nvme0n1p4 /mnt/newpartition
#     6  sudo lsblk
#     7  sudo fdisk -l
#     8  df -h
#     9  sudo fdisk -l
#    10  sudo fdisk /dev/nvme0n1
#    11  df -h
#    12  sudo lsblk
#    13  sudo apt install cgroup-tools
#    14  cd ~
#    15  sudo apt get update install cgroup-tools
#    16  sudo apt-get install -y cgroup-tools
#    17  sudo apt-get update -y
#    18  sudo apt-get install -y cgroup-tools
#    19  ls /sys/fs/cgroup/memory/
#    20  sudo cgcreate -t rsrw23:cs598cds-rsr4-PG -a rsrw23:cs598cds-rsr4-PG -g memory:rocksdb
#    21  ls /sys/fs/cgroup/memory/
#    22  sudo cgcreate -t rsrw23:cs598cds-rsr4-PG -a rsrw23:cs598cds-rsr4-PG -g memory:rocksdb
#    23  ls /sys/fs/cgroup/memory/
#    24  /sys/fs/cgroup/memory/rocksdb/memory.limit_in_bytes, it should output 4294967296.
#    25  sudo cat /sys/fs/cgroup/memory/rocksdb/memory.limit_in_bytes
#    26  clear
#    27  cd ..
#    28  ls
#    29  rm collectl-4.3.1.src.tar.gz
#    30  history