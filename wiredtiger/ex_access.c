#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <test_util.h>

static const char *home;

void parse_line(const char *line, char *command, char *table_name, char *key, char *value)
{
    sscanf(line, "%s %s %[^[][ field1=%[^field]]]", command, table_name, key, value);
}

int main(int argc, char *argv[])
{
    home = example_setup(argc, argv);

    /*! [access example connection] */
    WT_CONNECTION *conn;
    WT_CURSOR *cursor;
    WT_SESSION *session;
    const char *key, *value;

    FILE *file;
    char *filename = "outWorkloadA.txt"; // Replace with your desired file name
    char line[1500];                     // Buffer to store each line read from the file

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
    /*! [access example connection] */

    /*! [access example table create] */
    error_check(session->create(session, "table:access", "key_format=S,value_format=S"));
    /*! [access example table create] */

    /*! [access example cursor open] */
    error_check(session->open_cursor(session, "table:access", NULL, NULL, &cursor));
    /*! [access example cursor open] */

    while (fgets(line, sizeof(line), file) != NULL)
    {
        parse_line(line, command, table_name, rkey, rvalue);
        cursor->set_key(cursor, rkey); /* Insert a record. */
        cursor->set_value(cursor, rvalue);
        error_check(cursor->insert(cursor));
        printf("Inserted key '%s'\n", rkey);
    }

    error_check(cursor->reset(cursor));

    char *skey = "user412164360235391016 ";
    cursor->set_key(cursor, skey);
    error_check(cursor->search(cursor));

    error_check(cursor->get_key(cursor, &key));
    error_check(cursor->get_value(cursor, &value));
    printf("Got record: %s : %s\n", key, value);

    /*! [access example cursor list] */
    // error_check(cursor->reset(cursor)); /* Restart the scan. */
    // while ((ret = cursor->next(cursor)) == 0)
    // {
    //     error_check(cursor->get_key(cursor, &key));
    //     error_check(cursor->get_value(cursor, &value));

    //     printf("Got record: %s : %s\n", key, value);
    // }
    // scan_end_check(ret == WT_NOTFOUND); /* Check for end-of-table. */
    // /*! [access example cursor list] */

    /*! [access example close] */
    error_check(conn->close(conn, NULL)); /* Close all handles. */
                                          /*! [access example close] */

    return (EXIT_SUCCESS);
}