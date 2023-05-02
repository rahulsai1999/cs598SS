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
#include <stdlib.h>
#include <pthread.h>

#include "splinterdb/default_data_config.h"
#include "splinterdb/splinterdb.h"

#define DB_FILE_NAME "splinterdb_intro_db"
#define DB_FILE_SIZE_MB 2000 // Size of SplinterDB device; Fixed when created
#define CACHE_SIZE_MB 512    // Size of cache; can be changed across boots

/* Application declares the limit of key-sizes it intends to use */
#define USER_MAX_KEY_SIZE ((int)100)

#define NUM_THREADS 4

typedef struct
{
    int thread_id;
    char **lines;
    int start_line;
    int end_line;
    splinterdb *spl_handle;
} thread_data_t;

void parse_line(const char *line, char *command, char *table_name, char *key, char *value)
{
    sscanf(line, "%6s %s %[^[][ field0=%[^]]", command, table_name, key, value);
}

void *process_lines(void *arg)
{
    thread_data_t *data = (thread_data_t *)arg;
    int count = 0;

    char command[10];    // Buffer to store the command
    char table_name[20]; // Buffer to store the table name
    char key[100];       // Buffer to store the key
    char value[1300];    // Buffer to store the value

    for (int i = data->start_line; i < data->end_line; i++)
    {
        parse_line(data->lines[i], command, table_name, key, value);
        if (strcmp(command, "INSERT") == 0)
        {
            slice skey = slice_create((size_t)strlen(key), key);
            slice svalue = slice_create((size_t)strlen(value), value);
            int rc = splinterdb_insert(data->spl_handle, skey, svalue);
            count++;
            if (count % 1000 == 0)
            {
                printf("Thread %d: Inserted %d records\n", data->thread_id, count);
            }
        }
    }

    return NULL;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

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

    // Open the file
    file = fopen(filename, "r");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return 1;
    }

    printf("Starting writes...\n");

    // Read the file and store the lines in a buffer
    char **lines = malloc(0);
    int num_lines = 0;

    while (fgets(line, sizeof(line), file) != NULL)
    {
        num_lines++;
        lines = realloc(lines, num_lines * sizeof(char *));
        lines[num_lines - 1] = strdup(line);
    }
    fclose(file);

    // Create threads and divide the lines among them
    pthread_t threads[NUM_THREADS];
    thread_data_t thread_data[NUM_THREADS];
    int lines_per_thread = num_lines / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_data[i].thread_id = i;
        thread_data[i].lines = lines;
        thread_data[i].start_line = i * lines_per_thread;
        thread_data[i].end_line = (i == NUM_THREADS - 1) ? num_lines : (i + 1) * lines_per_thread;
        thread_data[i].spl_handle = spl_handle;

        if (pthread_create(&threads[i], NULL, process_lines, &thread_data[i]) != 0)
        {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    // Free memory allocated for the lines
    for (int i = 0; i < num_lines; i++)
    {
        free(lines[i]);
    }
    free(lines);

    // Rest of the code remains the same
    splinterdb_close(&spl_handle);
    printf("Shutdown SplinterDB instance, dbname '%s'.\n\n", DB_FILE_NAME);

    return 0;
}