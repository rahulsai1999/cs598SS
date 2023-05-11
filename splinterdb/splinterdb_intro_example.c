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
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#include "splinterdb/default_data_config.h"
#include "splinterdb/splinterdb.h"

#define Kilo (1024UL)
#define Mega (1024UL * Kilo)
#define Giga (1024UL * Mega)

#define DB_FILE_NAME "splinterdb_intro_db"

/* Application declares the limit of key-sizes it intends to use */
#define USER_MAX_KEY_SIZE ((int)100)

void parse_line(const char *line,
                char *command,
                char *table_name,
                char *key,
                char *value)
{
    sscanf(line, "%6s %s %[^[][ field0=%[^]]", command, table_name, key, value);
}

typedef struct thread_data
{
    char filename[256];
    splinterdb *spl_handle;
} thread_data;

void *
writePerThread(void *arg)
{
    thread_data *data = (thread_data *)arg;
    char *filename = data->filename;
    splinterdb *spl_handle = data->spl_handle;

    FILE *file;
    char line[1500];

    char command[10];
    char table_name[20];
    char key[100];
    char value[1300];

    file = fopen(filename, "r");
    printf("filename: %s\n", filename);

    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return NULL;
    }

    printf("Starting writes...\n");

    splinterdb_register_thread(spl_handle);

    while (fgets(line, sizeof(line), file) != NULL)
    {
        parse_line(line, command, table_name, key, value);
        if (strcmp(command, "INSERT") == 0)
        {
            slice skey = slice_create((size_t)strlen(key), key);
            slice svalue = slice_create((size_t)strlen(value), value);
            splinterdb_insert(spl_handle, skey, svalue);
        }
    }

    fclose(file);
}

int main(int argc, char *argv[])
{
    int NUM_THREADS = atoi(argv[1]);

    printf("**** SplinterDB Basic example program ****\n\n");

    // Initialize data configuration, using default key-comparison handling.
    data_config splinter_data_cfg;
    default_data_config_init(USER_MAX_KEY_SIZE, &splinter_data_cfg);

    // Basic configuration of a SplinterDB instance
    splinterdb_config splinterdb_cfg;
    memset(&splinterdb_cfg, 0, sizeof(splinterdb_cfg));
    splinterdb_cfg.filename = DB_FILE_NAME;
    splinterdb_cfg.disk_size = (12 * Giga);
    splinterdb_cfg.cache_size = (1250 * Mega); // always just more than 25% of disk size
    splinterdb_cfg.data_cfg = &splinter_data_cfg;

    splinterdb *spl_handle = NULL; // To a running SplinterDB instance

    int rc = splinterdb_create(&splinterdb_cfg, &spl_handle);
    printf("Created SplinterDB instance, dbname '%s'.\n\n", DB_FILE_NAME);

    // threading stuff here
    pthread_t threads[NUM_THREADS];
    thread_data thread_data_array[NUM_THREADS];

    fprintf(stdout, "%lu\n", (unsigned long)time(NULL));

    for (int i = 0; i < NUM_THREADS; i++)
    {
        snprintf(thread_data_array[i].filename,
                 sizeof(thread_data_array[i].filename),
                 "xa%c",
                 'a' + i);
        thread_data_array[i].spl_handle = spl_handle;
        int rc = pthread_create(
            &threads[i], NULL, writePerThread, (void *)&thread_data_array[i]);
        if (rc)
        {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
        }
    }

    printf("Waiting for threads to finish...\n");

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // end thread stuff here
    // Retrieve a key-value pair.
    splinterdb_lookup_result result;
    splinterdb_lookup_result_init(spl_handle, &result, 0, NULL);

    slice gkey;
    slice gvalue;
    const char *fruit = "user412164360235391016 "; // random key from workload
    gkey = slice_create((size_t)strlen(fruit), fruit);
    rc = splinterdb_lookup(spl_handle, gkey, &result);
    rc = splinterdb_lookup_result_value(&result, &gvalue);
    if (!rc)
    {
        printf("Found key: '%s', value: '%.*s'\n",
               fruit,
               (int)slice_length(gvalue),
               (char *)slice_data(gvalue));
    }

    fprintf(stdout, "%lu\n", (unsigned long)time(NULL));

    // making sure it is consistent
    printf("Shutdown and reopen SplinterDB instance ...\n");
    splinterdb_close(&spl_handle);

    rc = splinterdb_open(&splinterdb_cfg, &spl_handle);
    if (rc)
    {
        printf("Error re-opening SplinterDB instance, dbname '%s' (rc=%d).\n",
               DB_FILE_NAME,
               rc);
        return (rc);
    }

    splinterdb_close(&spl_handle);
    printf("Shutdown SplinterDB instance, dbname '%s'.\n\n", DB_FILE_NAME);

    return EXIT_SUCCESS;
}