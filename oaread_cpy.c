#include <stdio.h>
#include <stdlib.h>

void parse_line(const char *line, char *command, char *table_name, char *key, char *value)
{
    sscanf(line, "%s %s %[^[][ field1=%[^field]]]", command, table_name, key, value);
}

int main()
{
    FILE *file;
    char *filename = "outWorkloadA.txt"; // Replace with your desired file name
    char line[1500];                     // Buffer to store each line read from the file
    
    char command[10];                    // Buffer to store the command
    char table_name[20];                 // Buffer to store the table name
    char key[100];                       // Buffer to store the key
    char value[1300];                    // Buffer to store the value

    // Open the file
    file = fopen(filename, "r");
    
    // Check if the file was opened successfully
    if (file == NULL)
    {
        fprintf(stderr, "Error: Unable to open file %s\n", filename);
        return 1;
    }

    // Read the file line by line
    while (fgets(line, sizeof(line), file) != NULL)
    {
        parse_line(line, command, table_name, key, value);
        // printf("%s %s %s %s \n", command, table_name, key, value);
        printf("%s %s \n", key, value);
    }

    // write using client of WiredTiger, RocksDB, SplinterDB etc.
    // TODO: Write your code here

    // Close the file
    fclose(file);

    return 0;
}
