//  Copyright (c) 2011-present, Facebook, Inc.  All rights reserved.
//  This source code is licensed under both the GPLv2 (found in the
//  COPYING file in the root directory) and Apache 2.0 License
//  (found in the LICENSE.Apache file in the root directory).

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "rocksdb/c.h"

#if defined(OS_WIN)
#include <Windows.h>
#else
#include <unistd.h> // sysconf() - get CPU count
#endif

#if defined(OS_WIN)
const char DBPath[] = "C:\\Windows\\TEMP\\rocksdb_c_simple_example";
const char DBBackupPath[] =
    "C:\\Windows\\TEMP\\rocksdb_c_simple_example_backup";
#else
const char DBPath[] = "/tmp/rocksdb_c_simple_example";
const char DBBackupPath[] = "/tmp/rocksdb_c_simple_example_backup";
#endif

void parse_line(const char *line, char *command, char *table_name, char *key, char *value)
{
    sscanf(line, "%6s %s %[^[][ field1=%[^ ]]", command, table_name, key, value);
}

int main(int argc, char **argv)
{
    rocksdb_t *db;
    rocksdb_backup_engine_t *be;
    rocksdb_options_t *options = rocksdb_options_create();
    // Optimize RocksDB. This is the easiest way to
    // get RocksDB to perform well.
#if defined(OS_WIN)
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    long cpus = system_info.dwNumberOfProcessors;
#else
    long cpus = sysconf(_SC_NPROCESSORS_ONLN);
#endif

    FILE *file;
    char *filename = "outWorkloadA.txt"; // Replace with your desired file name
    char line[1500];                     // Buffer to store each line read from the file

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

    // Set # of online cores
    rocksdb_options_increase_parallelism(options, (int)(cpus));
    rocksdb_options_optimize_level_style_compaction(options, 0);
    // create the DB if it's not already present
    rocksdb_options_set_create_if_missing(options, 1);

    // open DB
    char *err = NULL;
    db = rocksdb_open(options, DBPath, &err);
    assert(!err);

    // open Backup Engine that we will use for backing up our database
    be = rocksdb_backup_engine_open(options, DBBackupPath, &err);
    assert(!err);

    // Put key-value
    // Read the file line by line
    rocksdb_writeoptions_t *writeoptions = rocksdb_writeoptions_create();
    while (fgets(line, sizeof(line), file) != NULL)
    {
        parse_line(line, command, table_name, key, value);
        if (strcmp(command, "INSERT") == 0)
        {
            rocksdb_put(db, writeoptions, key, strlen(key), value, strlen(value) + 1,
                        &err);
            printf("Inserted key '%s'\n", key);
            assert(!err);
        }
    }

    size_t len;
    printf("Get('user412164360235391016')\n");
    const char skey[] = "user412164360235391016 ";
    rocksdb_readoptions_t *readoptions = rocksdb_readoptions_create();
    char *returned_value = rocksdb_get(db, readoptions, skey, strlen(skey), &len, &err);
    assert(!err);
    printf("Get('%s') = '%s'\n", skey, returned_value);
    free(returned_value);

    // create new backup in a directory specified by DBBackupPath
    rocksdb_backup_engine_create_new_backup(be, db, &err);
    assert(!err);

    rocksdb_close(db);

    // If something is wrong, you might want to restore data from last backup
    rocksdb_restore_options_t *restore_options = rocksdb_restore_options_create();
    rocksdb_backup_engine_restore_db_from_latest_backup(be, DBPath, DBPath,
                                                        restore_options, &err);
    assert(!err);
    rocksdb_restore_options_destroy(restore_options);

    db = rocksdb_open(options, DBPath, &err);
    assert(!err);

    // cleanup
    rocksdb_writeoptions_destroy(writeoptions);
    rocksdb_options_destroy(options);
    rocksdb_backup_engine_close(be);
    rocksdb_close(db);
    fclose(file);

    return 0;
}