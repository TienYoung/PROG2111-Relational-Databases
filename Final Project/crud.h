/*
* FILE : a3.h
* PROJECT : Movie Rental System
* PROGRAMMER : GROUP 7
* FIRST VERSION : 2024-11-29
* DESCRIPTION :
* This header file declares functions and constants used in the Movie Rental System program.
* The system allows users to manage rental transactions, update customer information,
* view rental history, and delete customer records from the MySQL database.
*/

/*need this library for the getValidinput func*/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <mysql.h>

#define VARCHAR    256
#define DATE_CHARS  11  // "YYYY-MM-DD\0"
#define MAX_CHARS 1024

bool getValidInput(const char* prompt, const char* format, void* variable);

void create(MYSQL* object, const char* table);
void read(MYSQL* object, const char* table);
void update(MYSQL* object, const char* table);
void delete(MYSQL* object, const char* table);

void insertUser(MYSQL* object)
{
	char firstname[VARCHAR] = "";
	char lastname[VARCHAR] = "";
	char membershidate[DATE_CHARS] = "";

	// Collect essential user information.
	getValidInput("Enter first name: ", "%s", &firstname);
	getValidInput("Enter last name: ", "%s", &lastname);

	time_t t = time(NULL); // Get current time
	struct tm* tm_info = localtime(&t); // Convert to local time structure
	strftime(membershidate, DATE_CHARS, "%Y-%m-%d", tm_info); // Format the date

	// Insert User into the database
	char query[MAX_CHARS] = "";
	snprintf(query, MAX_CHARS,
		"INSERT INTO `User` (firstname, lastname, membershipdate) VALUES ('%s', '%s', '%s')",
		firstname, lastname, membershidate);

	if (mysql_query(object, query) == 0) {
		printf("User added successfully.\n");
	}
	else {
		fprintf(stderr, "Failed to add User: %s\n", mysql_error(object));
	}
}

void create(MYSQL* object, const char* table)
{
	if (strcmp(table, "User") == 0)
	{
		insertUser(object);
	}
}

void read(MYSQL* object, const char* table)
{

}

void update(MYSQL* object, const char* table)
{
	char firstname[VARCHAR] = "";
	char lastname[VARCHAR] = "";
	char contactnumber[VARCHAR] = "";
	char email[VARCHAR] = "";
	char address[VARCHAR] = "";

	// Collect updated User information
	getValidInput("Enter First Name: ", "%s", firstname);
	getValidInput("Enter Last Name: ", "%s", lastname);
	getValidInput("Enter New Contact Number: ", "%s", contactnumber);
	getValidInput("Enter New Email: ", "%s", email);
	getValidInput("Enter New Address: ", "%s", &address);

		//char selectQuery[MAX_CHARS] = "";
	//snprintf(selectQuery, MAX_CHARS,
	//	"SELECT userid FROM `User` WHERE firstname=%s AND lastname=%s",
	//	firstname, lastname);
	//if (mysql_query(object, selectQuery) != 0) {
	//	fprintf(stderr, "Failed to check film availability: %s\n", mysql_error(object));
	//	return;
	//}

	char updateQuery[MAX_CHARS];
	snprintf(updateQuery, MAX_CHARS,
		"UPDATE `User` SET contactnumber='%s', email='%s', address='%s' WHERE firstname='%s' AND lastname='%s' AND userid > 0",
		contactnumber, email, address, firstname, lastname);

	if (mysql_query(object, updateQuery) != 0) {
		fprintf(stderr, "Failed to update customer information: %s\n", mysql_error(object));
	}
	else {
		printf("Customer information updated successfully.\n");
	}
}


void addNewRentalTransaction(MYSQL* object);
void updateCustomerInfo(MYSQL* object);
void viewRentalHistory(MYSQL* object);
void deleteCustomerRecord(MYSQL* object);

/*prints the menu*/
void menu();

/*
* FUNCTION : getValidInput
* DESCRIPTION : Prompts the user for input and validates the format.
* PARAMETERS : const char* prompt - the prompt to display to the user
*              const char* format - the format string for input
*              void* variable - pointer to the variable to store the input
* RETURNS : bool - whether the input was valid
*/
bool getValidInput(const char* prompt, const char* format, void* variable) {
	int validInput;
	do {
		printf("%s", prompt);
		validInput = scanf_s(format, variable, VARCHAR);
		while (getchar() != '\n');  // Clear the input buffer

		if (validInput != 1) {
			printf("Invalid input. Please try again.\n");
			validInput = 0;
		}
	} while (validInput != 1);

	return true;
};