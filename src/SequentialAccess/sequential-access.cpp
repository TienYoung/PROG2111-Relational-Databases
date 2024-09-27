/*
* FILE : sequential-access.cpp
* PROJECT : PROG2111 - Assignment # 1
* PROGRAMMER : Justin Blake
* FIRST VERSION : 2024-09-24
* DESCRIPTION :
* The functions in this file are used to interact with a file representing a database. It allows the main CRUD functions.
* It currently works with binary files, but can be modified to work with text files.
*/
#pragma warning(disable: 4996)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define statusFailure false
#define statusSuccess true

typedef struct Record {
    int productId;
    char name[50];
    char category[50];
    int quantity;
    float price;
} Record;

void showMenu();
bool addRecord(const char* filename, Record record);
bool readRecords(const char* filename);
bool updateRecord(const char* filename, int inputId, Record record);
bool deleteRecord(const char* filename, int inputId);

int main(void)
{
    const char* filename = "sequential-access-initial.dat";
    int userChoice = 0;
    int id = 0;
    Record record;
    bool exitOption = false;

    while (!exitOption) {
        showMenu();
        if (scanf("%d", &userChoice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (userChoice) {
        case 1:  
            printf("Enter product ID: ");
            while (scanf("%d", &record.productId) != 1) 
            {
                printf("Invalid input. Please enter a number for the product ID: ");
                while (getchar() != '\n'); // Clear input buffer
            }
            printf("Enter product name: ");
            scanf("%s", record.name);
            printf("Enter product category: ");
            scanf("%s", record.category);
            printf("Enter quantity: ");
            while (scanf("%d", &record.quantity) != 1) 
            {
                printf("Invalid input. Please enter a number for the quantity: ");
                while (getchar() != '\n'); 
            }
            printf("Enter price: ");
            while (scanf("%f", &record.price) != 1) 
            {
                printf("Invalid input. Please enter a number for the price: ");
                while (getchar() != '\n'); 
            }
            if (addRecord(filename, record)) 
            {
                printf("Record added successfully!\n");
            }
            else 
            {
                printf("Failed to add record.\n");
            }
            break;

        case 2:  
            printf("Enter product ID to update: ");
            while (scanf("%d", &id) != 1) 
            {
                printf("Invalid input. Please enter a number for the product ID: ");
                while (getchar() != '\n'); // Clear input buffer
            }
            printf("Enter new product name: ");
            scanf("%s", record.name);
            printf("Enter new product category: ");
            scanf("%s", record.category);
            printf("Enter new quantity: ");
            while (scanf("%d", &record.quantity) != 1) 
            {
                printf("Invalid input. Please enter a number for the quantity: ");
                while (getchar() != '\n'); // Clear input buffer
            }
            printf("Enter new price: ");
            while (scanf("%f", &record.price) != 1) 
            {
                printf("Invalid input. Please enter a number for the price: ");
                while (getchar() != '\n'); 
            }
            record.productId = id;
            if (updateRecord(filename, id, record)) 
            {
                printf("Record updated successfully!\n");
            }
            else 
            {
                printf("Failed to update record.\n");
            }
            break;

        case 3:  
            if (!readRecords(filename)) 
            {
                printf("Failed to read records.\n");
            }
            break;

        case 4:  
            printf("Enter product ID to delete: ");
            while (scanf("%d", &id) != 1) 
            {
                printf("Invalid input. Please enter a number for the product ID: ");
                while (getchar() != '\n'); 
            }
            if (deleteRecord(filename, id)) 
            {
                printf("Record deleted successfully!\n");
            }
            else 
            {
                printf("Failed to delete record.\n");
            }
            break;

        case 5:
            exitOption = true;
            break;
        default:
            printf("Invalid choice, please try again.\n");
        }
        while (getchar() != '\n'); // Clear input buffer
    }
    return statusSuccess;
}

// FUNCTION : showMenu
// DESCRIPTION :
// Presents a menu for the user to interact with.
// PARAMETERS :
// void
// RETURNS :
// void
void showMenu()
{
    printf("\n\nMenu\n");
    printf("1. Create Record\n");
    printf("2. Update Record\n");
    printf("3. Read All Records\n");
    printf("4. Delete Record\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

// FUNCTION : addRecord
// DESCRIPTION :
// Writes a record into a file.
// PARAMETERS :
// filename: the file that will store the record.
// record: the entry that will be stored
// RETURNS :
// true: Success
// false: Failure
bool addRecord(const char* filename, Record record) 
{
    FILE* file = fopen(filename, "ab");
    if (file == NULL) 
    {
        return statusFailure;
    }
    fwrite(&record, sizeof(Record), 1, file);
    fclose(file);
    return statusSuccess;
}

// FUNCTION : readRecords
// DESCRIPTION :
// Reads through a file and prints every entry in the console.
// PARAMETERS :
// filename: the file that stores all the entries.
// RETURNS :
// true: Success
// false: Failure
bool readRecords(const char* filename)
{
    FILE* file = fopen(filename, "rb");
    if (file == NULL) 
    {
        return statusFailure;
    }
    Record buffer;
    while (fread(&buffer, sizeof(Record), 1, file))
    {
        printf("%d %s %s %d $%.2f\n", buffer.productId, buffer.name, buffer.category, buffer.quantity, buffer.price);
    }
    fclose(file);
    return statusSuccess;
}

// FUNCTION : updateRecord
// DESCRIPTION :
// Adds a record to a file.
// PARAMETERS :
// filename: the file where the current entry is stored.
// inputId: the id used to identify which record is to be updated
// record: the entry that will overwrite the old entry
// RETURNS :
// true: Success
// false: Failure
bool updateRecord(const char* filename, int inputId, Record record) 
{
    FILE* file = fopen(filename, "rb+");
    if (file == NULL) 
    {
        return statusFailure;
    }
    Record buffer;
    while (fread(&buffer, sizeof(Record), 1, file)) 
    {
        if (buffer.productId == inputId) 
        {
            fseek(file, -(long)sizeof(Record), SEEK_CUR);
            fwrite(&record, sizeof(Record), 1, file);
            break; 
        }
    }
    fclose(file);
    return statusSuccess;
}

// FUNCTION : deleteRecord
// DESCRIPTION :
// Deletes an entry from a file.
// PARAMETERS :
// filename: the file where the current entry is stored.
// inputId: the id used to identify which record is to be deleted
// RETURNS :
// true: Success
// false: Failure
bool deleteRecord(const char* filename, int inputId) 
{
    FILE* file = fopen(filename, "rb");
    FILE* tempFile = fopen("tempfile.dat", "wb");
    if (file == NULL || tempFile == NULL) 
    {
        return statusFailure;
    }

    Record buffer;
    while (fread(&buffer, sizeof(Record), 1, file)) 
    {
        if (buffer.productId != inputId) 
        {
            fwrite(&buffer, sizeof(Record), 1, tempFile); // Copy all except the record to be deleted
        }
    }
    fclose(file);
    fclose(tempFile);
    remove(filename); // Delete the original file
    rename("tempfile.dat", filename); // Rename temp file to original file name
    return statusSuccess;
}