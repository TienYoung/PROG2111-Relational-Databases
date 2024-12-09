#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 100
#define RECORD_SIZE sizeof(Record) // 104

typedef struct {
    int id;
    char name[50];
    char email[50];
} Record;

void addRecord(const char *filename, Record record) {
    FILE* file = fopen(filename, "ab+");

    fwrite(&record, RECORD_SIZE, 1, file);

    fclose(file);
}

void readRecords(const char *filename) {
    FILE* file = fopen(filename, "rb"); 
    Record read = {};
    while(fread(&read, RECORD_SIZE, 1, file))
    {
        printf("%d, %s, %s\n", read.id, read.name, read.email);
    }
    fclose(file);
}

void updateRecords(const char *filename, int id, Record record){
    FILE* file = fopen(filename, "rb+");
    
    Record read = {};
    while(fread(&read, RECORD_SIZE, 1, file))
    {
        if(read.id == id)
        {
            fseek(file, -RECORD_SIZE, SEEK_CUR);
            fwrite(&record, RECORD_SIZE, 1, file);
            break;
        }
    }
    
    fclose(file);
}

int main() {
    const char *filename = "sequential_db.dat";
    
    Record record1 = {1, "John Doe", "john@example.com"};
    Record record2 = {2, "Jane Smith", "jane@example.com"};
    
    addRecord(filename, record1);
    addRecord(filename, record2);
    
    printf("Reading records:\n");
    readRecords(filename);

    printf("Update Record with Id 1: \n");
    Record update = {1, "Foo Bar", "foo@bar.com"};
    updateRecords(filename, 1, update);
    readRecords(filename);
    
    return 0;
}
