/*
* FILE : a3.c
* PROJECT : Movie Rental System
* PROGRAMMERS : Group 7; Justin, Valentyn, Ahmed, Tian
* FIRST VERSION : 2024-11-29
* DESCRIPTION :
* This program implements a movie rental system that connects to a MySQL database.
* It provides options for users to add rental transactions, update customer information,
* view rental history, and delete customer records. The system interacts with a database
* to manage rentals and customer information.
*/

#include "crud.h"

// Function Declarations
void menu();
bool getValidInput(const char* prompt, const char* format, void* variable);
void addNewRentalTransaction(MYSQL* object);
void updateCustomerInfo(MYSQL* object);
void viewRentalHistory(MYSQL* object);
void deleteCustomerRecord(MYSQL* object);

/*
* FUNCTION : main
* DESCRIPTION : The entry point of the program. Establishes a connection to the MySQL database
*               and presents a menu to interact with the movie rental system.
* PARAMETERS : int argc, char* argv[]
* RETURNS : int - exit status (0 on success, EXIT_FAILURE on failure)
*/
int main(int argc, char* argv[]) {
    const char* server = "localhost";
    const char* username = "root";
    const char* password = "constantine";
    if (argc > 1)
    {
        password = argv[1];
    }
    const char* defaultDatabase = "booklibrary";

    // 1.Initialize a database connection object
    MYSQL* databaseObject = mysql_init(NULL);

	if (databaseObject == NULL)
	{
		fprintf(stderr, "Failed to initialize the database object");
		return EXIT_FAILURE;
	}

    // Establish connection to the database
    if (!mysql_real_connect(databaseObject, server, username, password, defaultDatabase, 0, NULL, 0)) {
        fprintf(stderr, "Failed to connect to the database: Error: %s\n", mysql_error(databaseObject));
        return EXIT_FAILURE;
    }

    int choice;
    do {
        menu();  // Display the menu
        getValidInput("Enter your choice: ", "%d", &choice);  // Get user input for menu choice

        // Handle the user's choice
        switch (choice) {
        case 1:
            create(databaseObject, "User");
            break;
        case 2:
            update(databaseObject, "User");
            break;
        case 3:
            delete(databaseObject, "User");
            break;
        case 4:
            deleteCustomerRecord(databaseObject);
            break;
        case 0:
            printf("Exiting the program.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);  // Repeat until user chooses to exit

    mysql_close(databaseObject);  // Close the database connection
    return 0;
}

/*
* FUNCTION : addNewRentalTransaction
* DESCRIPTION : Adds a new rental transaction to the database by gathering input from the user.
* PARAMETERS : MYSQL* object - the database connection object
* RETURNS : void
*/
void addNewRentalTransaction(MYSQL* object) {
    int customerID, inventoryID, staffID;
    char rentalDate[20], returnDate[20];

    // Collect rental transaction information from the user
    getValidInput("Enter Customer ID: ", "%d", &customerID);
    getValidInput("Enter Inventory ID: ", "%d", &inventoryID);
    getValidInput("Enter Staff ID: ", "%d", &staffID);
    getValidInput("Enter Return Date (YYYY-MM-DD): ", "%19s", returnDate);

    // Check film availability
    char checkQuery[MAX_CHARS];
    snprintf(checkQuery, sizeof(checkQuery),
        "SELECT COUNT(*) FROM rental WHERE inventory_id=%d AND return_date IS NULL", inventoryID);
    if (mysql_query(object, checkQuery) != 0) {
        printf("Failed to check film availability: %s\n", mysql_error(object));
        return;
    }

    MYSQL_RES* result = mysql_store_result(object);
    MYSQL_ROW row = mysql_fetch_row(result);
    if (atoi(row[0]) > 0) {
        printf("The film is currently unavailable. Adding customer to waitlist.\n");
        mysql_free_result(result);
        return;
    }
    mysql_free_result(result);

    // Insert rental transaction into the database
    char query[MAX_CHARS];
    snprintf(query, sizeof(query),
        "INSERT INTO rental (inventory_id, customer_id, return_date, staff_id) VALUES (%d, %d, '%s', %d)",
        inventoryID, customerID, returnDate, staffID);

    if (mysql_query(object, query) == 0) {
        printf("Rental transaction added successfully.\n");
    }
    else {
        printf("Failed to add rental transaction: %s\n", mysql_error(object));
    }
}

/*
* FUNCTION : updateCustomerInfo
* DESCRIPTION : Updates a customer's information in the database.
* PARAMETERS : MYSQL* object - the database connection object
* RETURNS : void
*/
void updateCustomerInfo(MYSQL* object) {
    int customerID, addressID;
    char firstName[50], lastName[50], email[100];

    // Collect updated customer information from the user
    getValidInput("Enter Customer ID: ", "%d", &customerID);
    getValidInput("Enter New First Name: ", "%49s", firstName);
    getValidInput("Enter New Last Name: ", "%49s", lastName);
    getValidInput("Enter New Email: ", "%99s", email);
    getValidInput("Enter New Address ID: ", "%d", &addressID);

    // Update customer information in the database
    char query[MAX_CHARS];
    snprintf(query, sizeof(query),
        "UPDATE customer SET first_name='%s', last_name='%s', email='%s', address_id=%d WHERE customer_id=%d",
        firstName, lastName, email, addressID, customerID);

    if (mysql_query(object, query) == 0) {
        printf("Customer information updated successfully.\n");
    }
    else {
        printf("Failed to update customer information: %s\n", mysql_error(object));
    }
}

/*
* FUNCTION : viewRentalHistory
* DESCRIPTION : Displays the rental history for a customer between a specified date range.
* PARAMETERS : MYSQL* object - the database connection object
* RETURNS : void
*/
void viewRentalHistory(MYSQL* object) {
    int customerID;
    char startDate[20], endDate[20];

    // Collect rental history parameters from the user
    getValidInput("Enter Customer ID: ", "%d", &customerID);
    getValidInput("Enter Start Date (YYYY-MM-DD): ", "%19s", startDate);
    getValidInput("Enter End Date (YYYY-MM-DD): ", "%19s", endDate);

    // Retrieve rental history from the database
    char query[MAX_CHARS];
    snprintf(query, sizeof(query),
        "SELECT rental_id, rental_date, return_date, inventory_id FROM rental WHERE customer_id=%d AND rental_date BETWEEN '%s' AND '%s'",
        customerID, startDate, endDate);

    if (mysql_query(object, query) != 0) {
        printf("Failed to retrieve rental history: %s\n", mysql_error(object));
        return;
    }

    MYSQL_RES* resultSet = mysql_store_result(object);
    if (resultSet == NULL) {
        printf("No results found.\n");
        return;
    }

    // Display the rental history
    MYSQL_ROW row;
    printf("\nRental History:\n");
    while ((row = mysql_fetch_row(resultSet)) != NULL) {
        printf("Rental ID: %s, Rental Date: %s, Return Date: %s, Inventory ID: %s\n", row[0], row[1], row[2], row[3]);
    }

    mysql_free_result(resultSet);
}

/*
* FUNCTION : deleteCustomerRecord
* DESCRIPTION : Deletes a customer record from the database if they have no unreturned rentals.
* PARAMETERS : MYSQL* object - the database connection object
* RETURNS : void
*/
void deleteCustomerRecord(MYSQL* object) {
    int customerID;

    // Collect customer ID to be deleted
    getValidInput("Enter Customer ID to delete: ", "%d", &customerID);

    // Check if customer has unreturned rentals
    char checkQuery[MAX_CHARS];
    snprintf(checkQuery, sizeof(checkQuery),
        "SELECT COUNT(*) FROM rental WHERE customer_id=%d AND return_date IS NULL", customerID);
    if (mysql_query(object, checkQuery) != 0) {
        printf("Failed to check dependencies: %s\n", mysql_error(object));
        return;
    }

    MYSQL_RES* result = mysql_store_result(object);
    MYSQL_ROW row = mysql_fetch_row(result);
    if (atoi(row[0]) > 0) {
        printf("Customer has unreturned rentals. Cannot delete record.\n");
        mysql_free_result(result);
        return;
    }
    mysql_free_result(result);

    // Delete customer record from the database
    char query[MAX_CHARS];
    snprintf(query, sizeof(query), "DELETE FROM customer WHERE customer_id=%d", customerID);

    if (mysql_query(object, query) == 0) {
        printf("Customer record deleted successfully.\n");
    }
    else {
        printf("Failed to delete customer record: %s\n", mysql_error(object));
    }
}

/*
* FUNCTION : menu
* DESCRIPTION : Displays the main menu to the user.
* PARAMETERS : None
* RETURNS : void
*/
void menu() {
    printf("\n*** Book Library System Menu ***\n");
    printf("1. Create a user\n");
    printf("2. Update a user\n");
    printf("3. Delete a user\n");
    printf("4. Create a loan\n");
    printf("5. Print loans by a user\n");
    printf("0. Exit\n");
}
