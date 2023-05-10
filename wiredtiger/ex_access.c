#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "test_util.h"

static const char *home;

#define NUM_THREADS 2

typedef struct thread_data
{
    char filename[256];
    WT_CONNECTION *connection;
} thread_data;

static WT_THREAD_RET scan_thread(void *conn_arg)
{
    FILE *file;
    char *filename;
    char line[1500];
    char command[10];
    char table_name[20];
    char key[100];
    char value[1300];
    int count = 0;

    WT_CONNECTION *conn;
    WT_CURSOR *cursor;
    WT_SESSION *session;

    thread_data *data = (thread_data *)conn_arg;

    conn = data->connection;
    filename = data->filename;

    error_check(conn->open_session(conn, NULL, NULL, &session));
    error_check(session->open_cursor(session, "table:access", NULL, NULL, &cursor));

    file = fopen(filename, "r");
    printf("filename: %s\n", filename);

    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return NULL;
    }

    while (fgets(line, sizeof(line), file) != NULL)
    {
        sscanf(line, "%6s %s %[^[][ field0=%[^]]", command, table_name, key, value);
        if (strcmp(command, "INSERT") == 0)
        {
            cursor->set_key(cursor, key);
            cursor->set_value(cursor, value);
            error_check(cursor->insert(cursor));
            count++;
        }

        if (count % 1000000 == 0)
            printf("Thread %s: %d records inserted\n", filename, count);
    }

    return (WT_THREAD_RET_VALUE);
}

int main(int argc, char *argv[])
{
    int i;
    const char *key, *value, *skey;

    WT_CONNECTION *conn;
    WT_SESSION *session;
    WT_CURSOR *cursor;
    wt_thread_t threads[NUM_THREADS];
    thread_data args[NUM_THREADS];

    home = example_setup(argc, argv);

    error_check(wiredtiger_open(home, NULL, "create", &conn));
    error_check(conn->open_session(conn, NULL, NULL, &session));
    error_check(session->create(session, "table:access", "key_format=S,value_format=S"));
    error_check(session->open_cursor(session, "table:access", NULL, "overwrite", &cursor));

    fprintf(stdout, "%lu\n", (unsigned long)time(NULL));

    for (i = 0; i < NUM_THREADS; i++)
    {
        snprintf(args[i].filename, sizeof(args[i].filename), "xa%c", 'a' + i);
        args[i].connection = conn;

        printf("Creating thread %d\n", i);
        error_check(__wt_thread_create(NULL, &threads[i], scan_thread, &args[i]));
    }

    for (i = 0; i < NUM_THREADS; i++)
        error_check(__wt_thread_join(NULL, &threads[i]));

    fprintf(stdout, "%lu\n", (unsigned long)time(NULL)); 

    error_check(cursor->reset(cursor));

    skey = "user412164360235391016 ";
    cursor->set_key(cursor, skey);

    error_check(cursor->search(cursor));
    error_check(cursor->get_key(cursor, &key));
    error_check(cursor->get_value(cursor, &value));
    printf("Got record: %s : %s\n", key, value);

    error_check(conn->close(conn, NULL));
    return (EXIT_SUCCESS);
}