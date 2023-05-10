// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include "rocksdb/slice.h"

using namespace std::chrono;
using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::Options;
using ROCKSDB_NAMESPACE::PinnableSlice;
using ROCKSDB_NAMESPACE::ReadOptions;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::WriteBatch;
using ROCKSDB_NAMESPACE::WriteOptions;

#if defined(OS_WIN)
std::string kDBPath = "C:\\Windows\\TEMP\\rocksdb_simple_example";
#else
std::string kDBPath = "rocksdb_simple_example";
#endif

std::mutex mtx; // Mutex for synchronizing access to shared resources

void parse_line(const char *line, char *command, char *table_name, char *key, char *value)
{
    sscanf(line, "%6s %s %[^[][ field0=%[^]]", command, table_name, key, value);
}

void process_lines(DB *db, char *filename, int i)
{
    FILE *file;
    int count = 0;
    char line[1500];     // Buffer to store each linead read from the file
    char command[10];    // Buffer to store the command
    char table_name[20]; // Buffer to store the table name
    char key[100];       // Buffer to store the key
    char value[1300];    // Buffer to store the value

    file = fopen(filename, "r");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        parse_line(line, command, table_name, key, value);

        if (strcmp(command, "INSERT") == 0)
        {
            std::string key_str(key);
            std::string value_str(value);

            Status s = db->Put(WriteOptions(), key_str, value_str);
            assert(s.ok());
            count++;
        }

        if (count % 1000000 == 0)
        {
            std::cout << "Thread " << i << " has written " << count << " records" << std::endl;
        }
    }
}

int main(int argc, char **argv)
{
    DB *db;
    int count = 0;
    Options options;
    // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();
    // create the DB if it's not already present
    options.create_if_missing = true;

    // open DB
    Status s = DB::Open(options, kDBPath, &db);
    assert(s.ok());

    // start timer here
    auto start = high_resolution_clock::now();
    std::cout << "Starting writes..." << std::endl;

    int num_threads = atoi(argv[2]);

    std::vector<std::thread> threads;

    for (int i = 0; i < num_threads; ++i)
    {
        std::cout << "Starting thread " << i << std::endl;
        char filename_c[100];
        snprintf(filename_c, sizeof(filename_c), "xa%c", 'a' + i);
        threads.emplace_back(std::thread(process_lines, db, filename_c, i));
    }

    for (auto &t : threads)
    {
        t.join();
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);

    std::cout << "Time takes to run " << num_threads << " threads:\t" << duration.count() << std::endl;

    // get value
    std::string rvalue;
    s = db->Get(ReadOptions(), "user9138490946712082907 ", &rvalue);
    std::cout << rvalue << std::endl;

    delete db;

    return 0;
}
