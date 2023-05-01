#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <test_util.h>

static const char *home;

int main(int argc, char *argv[])
{
    FILE *file;
    WT_CONNECTION *conn = NULL;
    WT_CURSOR *cursor = NULL;
    WT_SESSION *session = NULL;
    const char *key, *value, *skey, *filename;
    char line[1500], command[10], table_name[20], rkey[100], rvalue[1300];

    home = example_setup(argc, argv);
    filename = "outWorkloadA.txt"; // Declare filename as const

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
        sscanf(line, "%6s %s %[^[][ field0=%[^]]", command, table_name, rkey, rvalue);
        if (strcmp(command, "INSERT") == 0)
        {
            cursor->set_key(cursor, rkey); /* Insert a record. */
            cursor->set_value(cursor, rvalue);
            error_check(cursor->insert(cursor));
            printf("Inserted key '%s'\n", rkey);
        }
    }

    error_check(cursor->reset(cursor));
    skey = "user412164360235391016 ";
    cursor->set_key(cursor, skey);
    error_check(cursor->search(cursor));

    error_check(cursor->get_key(cursor, &key));
    error_check(cursor->get_value(cursor, &value));
    printf("Got record: %s : %s\n", key, value);

    error_check(conn->close(conn, NULL)); /* Close all handles. */

    return (EXIT_SUCCESS);
}
