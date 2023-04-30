/*-
 * Public Domain 2014-present MongoDB, Inc.
 * Public Domain 2008-2014 WiredTiger, Inc.
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ex_access.c
 * 	demonstrates how to create and access a simple table.
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <test_util.h>

static const char *home;

void parse_line(const char *line, char *command, char *table_name, char *key, char *value)
{
    sscanf(line, "%s %s %[^[][ field1=%[^field]]]", command, table_name, key, value);
}

static void access_example(void)
{
    /*! [access example connection] */
    WT_CONNECTION *conn;
    WT_CURSOR *cursor;
    WT_SESSION *session;
    const char *key, *value;
    int ret;

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
        parse_line(line, command, table_name, key, value);
        cursor->set_key(cursor, key); /* Insert a record. */
        cursor->set_value(cursor, value);
        error_check(cursor->insert(cursor));
        printf("Inserted key '%s'\n", key);
    }

    error_check(cursor->reset(cursor));

    const char *skey = "user412164360235391016 ";
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
}

int main(int argc, char *argv[])
{
    home = example_setup(argc, argv);

    access_example();

    return (EXIT_SUCCESS);
}