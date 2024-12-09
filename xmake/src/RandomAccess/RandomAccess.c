#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHARS 50
#define RECORD_SIZE sizeof(Record)
#define DELETED_ID -1

typedef struct
{
    int id;
    char name[MAX_CHARS];
    char category[MAX_CHARS];
    int quantity;
    float price;
} Record;

FILE* loadData(const char *filename)
{
    FILE *file = fopen(filename, "rb+");
    if (file == NULL)
    {
        perror("ERROR: Open file failed!");
        exit(EXIT_FAILURE);
    }
    return file;
}

bool unloadData(FILE* file)
{
    if(fclose(file) == EOF)
    {
        perror("ERROR: Open file failed!");
        return false;
    }

    return true;
}

bool appendRecord(FILE* file, int* position, const Record* record)
{
    if(fseek(file, 0, SEEK_END) != 0)
    {
        perror("ERROR: Append position invalid!");
        return false;
    }

    *position = ftell(file) / RECORD_SIZE;

    if(fwrite(record, RECORD_SIZE, 1, file) < 1)
    {
        perror("ERROR: Write failed!");
        return false;
    }

    printf("APPEND - Position %d:\nID: %d\nName: %s\nCategory:%s\nQuantity:%d\nPrice:%.2f\n",
     *position, record->id, record->name, record->category, record->quantity, record->price);
    putchar('\n');

    return true;
}

bool getRecords(FILE* file, int position, Record* record)
{
    if(fseek(file, position * RECORD_SIZE, SEEK_SET) != 0)
    {
        perror("ERROR: Position invalid!");
        return false;
    }

    if (record->id == DELETED_ID)
    {
        perror("ERROR: Record has been deleted!");
        return false;
    }

    if(fread(record, RECORD_SIZE, 1, file) < 1)
    {
        perror("ERROR: Read failed!");
        return false;
    }

    printf("GET - Position %d:\nID: %d\nName: %s\nCategory:%s\nQuantity:%d\nPrice:%.2f\n",
     position, record->id, record->name, record->category, record->quantity, record->price);
    putchar('\n');

    return true;
}

bool updateRecord(FILE* file, int position, const Record* record)
{
    if(fseek(file, position * RECORD_SIZE, SEEK_SET) != 0)
    {
        perror("ERROR: Append position invalid!");
        return false;
    }

    if(fwrite(record, RECORD_SIZE, 1, file) < 1)
    {
        perror("ERROR: Write failed!");
        return false;
    }

    printf("UPDATED - Position %d:\nID: %d\nName: %s\nCategory:%s\nQuantity:%d\nPrice:%.2f\n",
     position, record->id, record->name, record->category, record->quantity, record->price);
    putchar('\n');

    return true;
}

bool deleteRecords(FILE* file, int position)
{
    Record record;
    getRecords(file, position, &record);
    record.id = DELETED_ID;
    updateRecord(file, position, &record);

    printf("DELETED - Position %d:\nID: %d\nName: %s\nCategory:%s\nQuantity:%d\nPrice:%.2f\n",
     position, record.id, record.name, record.category, record.quantity, record.price);
    putchar('\n');

    return true;
}

int main()
{
    const char *filename = "random_db.dat";

    FILE* database = loadData(filename);


    // Read records.
    Record record;
    getRecords(database, 0, &record);
    getRecords(database, 1, &record);
    getRecords(database, 2, &record);

    // Append record the end.
    int positon = 0;
    record.id = 4;
    strcpy_s(record.name, MAX_CHARS, "Watch");
    record.price = 100.0f;
    appendRecord(database, &positon, &record);

    // Delete position 3.
    deleteRecords(database, positon);

    // Update records.
    Record record1 = {1, "Smartphone", "Electronics", 50, 699.99f};
    Record record2 = {2, "Laptop", "Electronics", 30, 999.99f};
    Record record3 = {3, "Tablet", "Electronics", 20, 299.99f};
    int position = 0;
    updateRecord(database, 0, &record1);
    //printf("Append record to %d\n", position);
    updateRecord(database, 1, &record2);
    //printf("Append record to %d\n", position);
    updateRecord(database, 2, &record3);
    //printf("Append record to %d\n", position);



    unloadData(database);
    
    return 0;
}
