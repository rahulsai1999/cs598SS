#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <test_util.h>

static const char *home;

void *write_per_thread(void *arg);

typedef struct thread_data
{
    char filename[256];
    WT_SESSION *session;
} thread_data;

void *write_per_thread(void *arg)
{
    thread_data *data = (thread_data *)arg;
    char *filename = data->filename;
    WT_SESSION *session = data->session;
    WT_CURSOR *cursor;

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

    error_check(session->open_cursor(session, "table:access", NULL, NULL, &cursor));

    while (fgets(line, sizeof(line), file) != NULL)
    {
        sscanf(line, "%6s %s %[^[][ field0=%[^]]", command, table_name, key, value);
        if (strcmp(command, "INSERT") == 0)
        {
            cursor->set_key(cursor, key);
            cursor->set_value(cursor, value);
            error_check(cursor->insert(cursor));
        }
    }

    fclose(file);
    error_check(cursor->close(cursor));
    return NULL;
}

int main(int argc, char *argv[])
{
    int rc;
    int NUM_THREADS = atoi(argv[1]);

    WT_CONNECTION *conn = NULL;
    WT_SESSION *session = NULL;

    pthread_t threads[NUM_THREADS];
    thread_data thread_data_array[NUM_THREADS];

    home = example_setup(argc, argv);

    error_check(wiredtiger_open(home, NULL, "create,statistics=(all)", &conn));
    error_check(conn->open_session(conn, NULL, NULL, &session));
    error_check(session->create(session, "table:access", "key_format=S,value_format=S"));

    for (int i = 0; i < NUM_THREADS; i++)
    {
        snprintf(thread_data_array[i].filename,
                 sizeof(thread_data_array[i].filename),
                 "xa%c",
                 'a' + i);
        thread_data_array[i].session = session;
        rc = pthread_create(
            &threads[i], NULL, write_per_thread, (void *)&thread_data_array[i]);
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

    error_check(conn->close(conn, NULL));

    return EXIT_SUCCESS;
}
