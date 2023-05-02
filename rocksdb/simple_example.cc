// Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>

#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include "rocksdb/slice.h"

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

void parse_line(const char *line, char *command, char *table_name, char *key, char *value)
{
    sscanf(line, "%6s %s %[^[][ field0=%[^]]", command, table_name, key, value);
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

    FILE *file;
    char *filename = argv[1]; // Replace with your desired file name
    char line[1500];          // Buffer to store each line read from the file

    char command[10];    // Buffer to store the command
    char table_name[20]; // Buffer to store the table name
    char key[100];       // Buffer to store the key
    char value[1300];    // Buffer to store the value

    // Open the file
    file = fopen(filename, "r");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return 1;
    }

    // open DB
    Status s = DB::Open(options, kDBPath, &db);
    assert(s.ok());

    std::cout << "Starting writes..." << std::endl;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        parse_line(line, command, table_name, key, value);
        if (strcmp(command, "INSERT") == 0)
        {
            // convert to string
            std::string key_str(key);
            std::string value_str(value);

            // insert
            s = db->Put(WriteOptions(), key, value);
            assert(s.ok());

            count++;
            if (count % 1000 == 0)
            {
                printf("Inserted %d records\n", count);
            }
        }
    }

    // get value
    std::string rvalue;
    s = db->Get(ReadOptions(), "user412164360235391016 ", &rvalue);
    assert(s.ok());
    std::cout << rvalue << std::endl;

    delete db;

    return 0;
}
