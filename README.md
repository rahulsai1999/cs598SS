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
