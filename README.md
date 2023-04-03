# CS598SS


## Installation/Basic Examples

```bash
bash install.sh
```

**Running RocksDB**

1. Edit simple_example

```bash
vi simple_example.cc
```

2.    compile simple_example

```bash
make simple_example.cc
```

3.   execute simple_example

```bash
./simple_example
```

**Running WiredTiger**
1. List commands
``` bash
wt
```
2. Creating a table
``` bash
wt create -c key_format=<type>,value_format=<type> <tablename>
```
3. Writing to a table
``` bash
wt write <tablename> -a <key> <value>
```
4. Reading from table
``` bash
wt read <tablename> <key>
```

## Project Checkpoint Requirements:
- Mount more NVME partitions on m510 and install a filesystem
- 100M insert records 12GB data
- 4GB memory
- YCSB uniform dist
- xl170 machine for SATA (CGROUP settings same on both m510 and xl170)