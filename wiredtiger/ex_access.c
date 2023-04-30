#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <test_util.h>

static const char *home;

int main(int argc, char *argv[])
{
    WT_CONNECTION *conn = NULL;
    WT_CURSOR *cursor = NULL;
    WT_SESSION *session = NULL;
    const char *key, *value;

    home = example_setup(argc, argv);

    FILE *file;
    const char *filename = "outWorkloadA.txt"; // Declare filename as const
    char line[1500];                           // Buffer to store each line read from the file

    char command[10];    // Buffer to store the command
    char table_name[20]; // Buffer to store the table name
    char rkey[100];      // Buffer to store the key
    char rvalue[1300];   // Buffer to store the value

    // Open the file
    file = fopen(filename, "r");

    // Check if the file was opened successfully
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return 1;
    }

    /* Open a connection to the database, creating it if necessary. */
    error_check(wiredtiger_open(home, NULL, "create,statistics=(all)", &conn));

    /* Open a session handle for the database. */
    error_check(conn->open_session(conn, NULL, NULL, &session));

    /* Create a cursor for the database. */
    error_check(session->open_cursor(session, "table:access", NULL, NULL, &cursor));

    while (fgets(line, sizeof(line), file) != NULL)
    {
        sscanf(line, "%s %s %[^[][ field1=%[^field]]]", command, table_name, rkey, rvalue);
        cursor->set_key(cursor, rkey); /* Insert a record. */
        cursor->set_value(cursor, rvalue);
        error_check(cursor->insert(cursor));
        printf("Inserted key '%s'\n", rkey);
    }

    error_check(cursor->reset(cursor));

    const char *skey = "user412164360235391016 ";
    cursor->set_key(cursor, skey);
    error_check(cursor->search(cursor));

    error_check(cursor->get_key(cursor, &key));
    error_check(cursor->get_value(cursor, &value));
    printf("Got record: %s : %s\n", key, value);

    error_check(conn->close(conn, NULL)); /* Close all handles. */

    return (EXIT_SUCCESS);
}
