# cs598SS
**Installing RocksDB**

```bash
sudo apt-get install build-essential libsnappy-dev zlib1g-dev libbz2-dev libgflags-dev
git clone https://github.com/facebook/rocksdb.git
cd rocksdb
cd examples/; make all
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

**Installing WiredTiger**
```bash
sudo apt-get update
sudo apt-get install -y wiredtiger
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
