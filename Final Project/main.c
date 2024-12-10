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

void mainMenu();
void createMenu();
void readMenu();

MYSQL* databaseObject = NULL;

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
    databaseObject = mysql_init(NULL);

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

	mainMenu();

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

void mainMenu() 
{
	int choice = 0;
	do
	{
		printf("*** Book Library System Main Menu***\n");
		printf("1. Create\n");
		printf("2. Read\n");
		printf("3. Update\n");
		printf("4. Delete\n");
		printf("0. Exit\n");

		getValidInput("Enter your choice: ", "%d", &choice);  // Get user input for menu choice
		// Handle the user's choice
		switch (choice) {
		case 1:
			createMenu();
			break;
		case 2:
			readMenu();
			break;
		case 3:
			delete(databaseObject, "User");
			break;
		case 4:
			create(databaseObject, "Loan");
			break;
		case 0:
			printf("Exiting the program.\n");
			break;
		default:
			printf("Invalid choice. Please try again.\n");
			break;
		}
	} while (choice != 0);
}

void createMenu() {
	int choice = 0;
	do
	{
		printf("*** Book Library System Create Menu***\n");
		printf("1. Author\n");
		printf("2. Book\n");
		printf("3. Building\n");
		printf("4. Employee\n");
		printf("5. Genre\n");
		printf("6. Publisher\n");
		printf("7. Reservation\n");
		printf("8. User\n");
		printf("0. Return\n");

		getValidInput("Enter your choice: ", "%d", &choice);  // Get user input for menu choice
		// Handle the user's choice
		switch (choice)
		{
		case 1:
			insert(selectAll(databaseObject, "Author"), databaseObject);
			break;
		case 2:
			insert(selectAll(databaseObject, "Book"), databaseObject);
			break;
		case 3:
			insert(selectAll(databaseObject, "Building"), databaseObject);
			break;
		case 4:
			insert(selectAll(databaseObject, "Employee"), databaseObject);
			break;
		case 5:
			insert(selectAll(databaseObject, "Genre"), databaseObject);
			break;
		case 6:
			insert(selectAll(databaseObject, "Publisher"), databaseObject);
			break;
		case 7:
			insert(selectAll(databaseObject, "Reservation"), databaseObject);
			break;
		case 8:
			insert(selectAll(databaseObject, "User"), databaseObject);
			break;
		case 0:
			putchar('\n');
			break;
		default:
			printf("Invalid choice.\n");
			break;
		}
	} while (choice != 0);
}

void readMenu() {
	int choice = 0;
	do
	{
		printf("*** Book Library System Read Menu***\n");
		printf("1. Author\n");
		printf("2. Book\n");
		printf("3. Building\n");
		printf("4. Employee\n");
		printf("5. Genre\n");
		printf("6. Publisher\n");
		printf("7. Reservation\n");
		printf("8. User\n");
		printf("0. Return\n");

		getValidInput("Enter your choice: ", "%d", &choice);  // Get user input for menu choice
		// Handle the user's choice
		switch (choice)
		{
		case 1:
			printResult(selectAll(databaseObject, "Author"));
			break;
		case 2:
			printResult(selectAll(databaseObject, "Book"));
			break;
		case 3:
			printResult(selectAll(databaseObject, "Building"));
			break;
		case 4:
			printResult(selectAll(databaseObject, "Employee"));
			break;
		case 5:
			printResult(selectAll(databaseObject, "Genre"));
			break;
		case 6:
			printResult(selectAll(databaseObject, "Publisher"));
			break;
		case 7:
			printResult(selectAll(databaseObject, "Reservation"));
			break;
		case 8:
			printResult(selectAll(databaseObject, "User"));
			break;
		case 0:
			putchar('\n');
			break;
		default:
			printf("Invalid choice.\n");
			break;
		}
	} while (choice != 0);
}
