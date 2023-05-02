// Copyright 2022 VMware, Inc.
// SPDX-License-Identifier: Apache-2.0
/*
 * SplinterDB "Hello World" example program. Demonstrate use of:
 *  - Basic SplinterDB configuration and create() interface
 *  - Insert, lookup and iterator interfaces
 *  - Close and reopen a SplinterDB db (instance)
 */

#include <stdio.h>
#include <string.h>

#include "splinterdb/default_data_config.h"
#include "splinterdb/splinterdb.h"

#define DB_FILE_NAME "splinterdb_intro_db"
#define DB_FILE_SIZE_MB 2000 // Size of SplinterDB device; Fixed when created
#define CACHE_SIZE_MB 512    // Size of cache; can be changed across boots

/* Application declares the limit of key-sizes it intends to use */
#define USER_MAX_KEY_SIZE ((int)100)

void parse_line(const char *line, char *command, char *table_name, char *key, char *value)
{
    sscanf(line, "%6s %s %[^[][ field0=%[^]]", command, table_name, key, value);
}

int main(int argc, char **argv)
{
    int count = 0;
    printf("     **** SplinterDB Basic example program ****\n\n");

    // Initialize data configuration, using default key-comparison handling.
    data_config splinter_data_cfg;
    default_data_config_init(USER_MAX_KEY_SIZE, &splinter_data_cfg);

    // Basic configuration of a SplinterDB instance
    splinterdb_config splinterdb_cfg;
    memset(&splinterdb_cfg, 0, sizeof(splinterdb_cfg));
    splinterdb_cfg.filename = DB_FILE_NAME;
    splinterdb_cfg.disk_size = (DB_FILE_SIZE_MB * 1024 * 1024);
    splinterdb_cfg.cache_size = (CACHE_SIZE_MB * 1024 * 1024);
    splinterdb_cfg.data_cfg = &splinter_data_cfg;

    splinterdb *spl_handle = NULL; // To a running SplinterDB instance

    int rc = splinterdb_create(&splinterdb_cfg, &spl_handle);
    printf("Created SplinterDB instance, dbname '%s'.\n\n", DB_FILE_NAME);

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

    printf("Starting writes...\n");

    while (fgets(line, sizeof(line), file) != NULL)
    {
        parse_line(line, command, table_name, key, value);
        if (strcmp(command, "INSERT") == 0)
        {
            slice skey = slice_create((size_t)strlen(key), key);
            slice svalue = slice_create((size_t)strlen(value), value);
            rc = splinterdb_insert(spl_handle, skey, svalue);
            count++;
            if (count % 1000 == 0)
            {
                printf("Inserted %d records\n", count);
            }
        }
    }

    // Retrieve a key-value pair.
    splinterdb_lookup_result result;
    splinterdb_lookup_result_init(spl_handle, &result, 0, NULL);

    slice gkey;
    slice gvalue;
    const char *fruit = "user412164360235391016 "; // random key from workload
    gkey = slice_create((size_t)strlen(fruit), fruit);
    rc = splinterdb_lookup(spl_handle, gkey, &result);
    rc = splinterdb_lookup_result_value(spl_handle, &result, &gvalue);
    if (!rc)
    {
        printf("Found key: '%s', value: '%.*s'\n", fruit,
               (int)slice_length(gvalue),
               (char *)slice_data(gvalue));
    }

    // making sure it is consistent
    printf("Shutdown and reopen SplinterDB instance ...\n");
    splinterdb_close(&spl_handle);

    rc = splinterdb_open(&splinterdb_cfg, &spl_handle);
    if (rc)
    {
        printf("Error re-opening SplinterDB instance, dbname '%s' (rc=%d).\n", DB_FILE_NAME, rc);
        return (rc);
    }

    splinterdb_close(&spl_handle);
    printf("Shutdown SplinterDB instance, dbname '%s'.\n\n", DB_FILE_NAME);

    return rc;
}