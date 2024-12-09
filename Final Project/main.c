/*
* FILE : main.c
* PROJECT : PROG2111 - Final Project
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
            create(databaseObject, "Loan");
            break;
		case 5:
			read(databaseObject, "Loan");
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
