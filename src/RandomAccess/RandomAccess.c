#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100
#define RECORD_SIZE sizeof(Record)

typedef struct
{
    int id;
    char name[50];
    char email[50];
} Record;

void addRecord(const char *filename, int position, Record record)
{
    FILE *file = fopen(filename, "ab");
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    fseek(file, position * RECORD_SIZE, SEEK_SET);
    fwrite(&record, RECORD_SIZE, 1, file);

    fclose(file);
}

void readRecords(const char *filename, int position)
{
    // 1. Open your file / Create your file if it does not exists already
    FILE *file = fopen(filename, "rb");
    // 2. Make sure you have a workable file pointer, if file pointer is null exit failure
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    fseek(file, position * sizeof(Record), SEEK_SET);
    Record temp;
    fread(&temp, sizeof(Record), 1, file);
    printf("%s\n", temp.email);

    while (fread(&temp, sizeof(Record), 1, file))
    {
        if (temp == NULL)
        {
            continue;
        }

        printf("%s\n", temp.email);
    }

    fclose(file);
}

void updateRecords(const char *filename, int position, Record record)
{
    // 1. Open your file / Create your file if it does not exists already
    FILE *file = fopen(filename, "rb");
    // 2. Make sure you have a workable file pointer, if file pointer is null exit failure
    if (file == NULL)
    {
        exit(EXIT_FAILURE);
    }

    fseek(file, position * sizeof(Record), SEEK_SET);
    fwrite(&record, sizeof(Record), 1, file);

    fclose(file);
}

int main()
{
    const char *filename = "sequential_db.dat";

    Record record1 = {1, "John Doe", "john@example.com"};
    Record record2 = {2, "Jane Smith", "jane@example.com"};

    addRecord(filename, 100, record1);
    addRecord(filename, 78, record2);

    printf("Reading records:\n");
    readRecords(filename, 2);

    printf("Update Record with Id 1: \n");
    updateRecords(filename, 1, record2);

    return 0;
}
