/*
* FILE : crud.h
* PROJECT : PROG2111 - Final Project
* PROGRAMMERS : Group 7; Justin, Valentyn, Ahmed, Tian
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
	getValidInput("Enter First Name: ", "%[^\n]", firstname);
	getValidInput("Enter Last Name: ", "%[^\n]", lastname);

	time_t t = time(NULL); // Get current time
	struct tm* tm_info = localtime(&t); // Convert to local time structure
	strftime(membershidate, DATE_CHARS, "%Y-%m-%d", tm_info); // Format the date

	// Insert User into the database
	char query[MAX_CHARS] = "";
	snprintf(query, MAX_CHARS,
		"INSERT INTO `User` (firstname, lastname, membershipdate) VALUES ('%s', '%s', '%s')",
		firstname, lastname, membershidate);

	if (mysql_query(object, query) != 0) {
		fprintf(stderr, "Failed to add User: %s\n", mysql_error(object));
	}
	else {
		printf("User added successfully.\n");
	}
}

void insertLoan(MYSQL* object)
{
	char firstname[VARCHAR] = "";
	char lastname[VARCHAR] = "";
	char booktitle [VARCHAR] = "";

	// Collect essential user information.
	getValidInput("Enter First Name: ", "%[^\n]", firstname);
	getValidInput("Enter Last Name: ", "%[^\n]", lastname);
	getValidInput("Enter Book Title: ", "%[^\n]", booktitle);

	// Insert User into the database
	char query[MAX_CHARS] = "";
	snprintf(query, MAX_CHARS,
		"INSERT INTO Loan (BookId, UserId) \
		VALUES( \
			(SELECT BookId FROM Book WHERE title = '%s' LIMIT 1), \
			(SELECT UserId FROM `User` WHERE firstname = '%s' AND lastname = '%s' LIMIT 1) \
		)",
		booktitle, firstname, lastname);

	if (mysql_query(object, query) != 0) {
		fprintf(stderr, "Failed to add Loan: %s\n", mysql_error(object));
	}
	else {
		printf("Loan added successfully.\n");
	}
}

void create(MYSQL* object, const char* table)
{
	if (strcmp(table, "User") == 0)
	{
		insertUser(object);
	}
	else if (strcmp(table, "Loan") == 0)
	{
		insertLoan(object);
	}
}

MYSQL_RES* selectAll(MYSQL* object, const char* table)
{
	char query[MAX_CHARS] = "";
	snprintf(query, MAX_CHARS, "SELECT * FROM %s", table);

	if (mysql_query(object, query) != 0) {
		fprintf(stderr, "Failed to select %s: %s\n", table, mysql_error(object));
		return NULL;
	}

	MYSQL_RES* result = mysql_store_result(object);
	if (result == NULL)
	{
		fprintf(stderr, "Failed to get the result set: Error: %s\n", mysql_error(object));
	}
	return result;
}

void insert(MYSQL_RES* result, MYSQL* object)
{
	char query[MAX_CHARS] = "INSERT INTO ";

	MYSQL_FIELD* fields = mysql_fetch_field(result);
	unsigned int numFields = mysql_num_fields(result);
	if (numFields > 0)
	{
		strcat_s(query, MAX_CHARS, fields[1].table);
	}
	else
	{
		return;
	}

	// Allocate memory for array of strings;
	char** rows = malloc(numFields * sizeof(char*));
	for(unsigned int i =0; i < numFields; i++)
	{
		if (i == 0)
		{
			strcat_s(query, MAX_CHARS, " (");
		}

		if ((fields[i].type & NOT_NULL_FLAG) && ((fields[i].type & PRI_KEY_FLAG) == 0))
		{
			rows[i] = malloc(VARCHAR * sizeof(char));

			printf("Enter %s", fields[i].name);
			getValidInput(": ", "%[^\n]", rows[i]);

			strcat_s(query, MAX_CHARS, fields[i].name);
			if (i < (numFields - 1))
			{
				strcat_s(query, MAX_CHARS, ", ");
			}
			else
			{
				strcat_s(query, MAX_CHARS, ") ");
			}
		}
		else
		{
			rows[i] = NULL;
		}
	}

	// Concate then free array of strings;
	for (unsigned int i = 0; i < numFields; i++)
	{
		if (i == 0)
		{
			strcat_s(query, MAX_CHARS, "VALUES (");
		}
		if (rows[i] != NULL)
		{
			strcat_s(query, MAX_CHARS, "'");
			strcat_s(query, MAX_CHARS, rows[i]);
			strcat_s(query, MAX_CHARS, "'");
			if (i < (numFields - 1))
			{
				strcat_s(query, MAX_CHARS, ", ");
			}
			else
			{
				strcat_s(query, MAX_CHARS, ") ");
			}
			free(rows[i]);
		}
	}
	free(rows);

	if (mysql_query(object, query) != 0) {
		fprintf(stderr, "Failed to add Loan: %s\n", mysql_error(object));
	}
	else {
		printf("Loan added successfully.\n");
	}
}

void printResult(MYSQL_RES* result)
{
	MYSQL_FIELD* field;

	while ((field = mysql_fetch_field(result)))
	{
		printf("%16s ", field->name);
	}
	putchar('\n');

	size_t numFields = mysql_num_fields(result);
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(result)) != NULL)
	{
		for (int i = 0; i < numFields; i++)
		{
			printf("%16s ", row[i]);
		}
		putchar('\n');
	}
}

void read(MYSQL* object, const char* table)
{
	char firstname[VARCHAR] = "";
	char lastname[VARCHAR] = "";

	// Collect essential user information.
	getValidInput("Enter First Name: ", "%[^\n]", firstname);
	getValidInput("Enter Last Name: ", "%[^\n]", lastname);

	// Insert User into the database
	char query[MAX_CHARS] = "";
	snprintf(query, MAX_CHARS,
		"SELECT L.loanid, firstname, lastname, title FROM Loan L \
		INNER JOIN `User` U ON L.userid = U.userid              \
		INNER JOIN Book B ON B.bookid = L.bookid                \
		WHERE firstname='%s' AND lastname='%s'",
		firstname, lastname);

	if (mysql_query(object, query) != 0) {
		fprintf(stderr, "Failed to add User: %s\n", mysql_error(object));
		return;
	}

	MYSQL_RES* resultSet = mysql_store_result(object);
	if (resultSet == NULL)
	{
		printf("Failed to get the result set: Error: %s\n", mysql_error(object));
		return;
	}

	// Display the rental history
	MYSQL_ROW row;
	printf("\nLoan History:\n");
	while ((row = mysql_fetch_row(resultSet)) != NULL) {
		printf("Loan ID: %s, First Name: %s, Last Name: %s, Book Title: %s\n", row[0], row[1], row[2], row[3]);
	}

	mysql_free_result(resultSet);
}

void update(MYSQL* object, const char* table)
{
	char firstname[VARCHAR] = "";
	char lastname[VARCHAR] = "";
	char contactnumber[VARCHAR] = "";
	char email[VARCHAR] = "";
	char address[VARCHAR] = "";

	// Collect updated User information
	getValidInput("Enter First Name: ", "%[^\n]", firstname);
	getValidInput("Enter Last Name: ", "%[^\n]", lastname);
	getValidInput("Enter New Contact Number: ", "%[^\n]", contactnumber);
	getValidInput("Enter New Email: ", "%[^\n]", email);
	getValidInput("Enter New Address: ", "%[^\n]", address);

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
		fprintf(stderr, "Failed to update user information: %s\n", mysql_error(object));
	}
	else {
		printf("User information updated successfully.\n");
	}
}

void delete(MYSQL* object, const char* table)
{
	char firstname[VARCHAR] = "";
	char lastname[VARCHAR] = "";

	// Collect user name to be deleted
	getValidInput("Enter First Name: ", "%[^\n]", firstname);
	getValidInput("Enter Last Name: ", "%[^\n]", lastname);

	// Delete customer record from the database
	char query[MAX_CHARS];
	snprintf(query, sizeof(query), "DELETE FROM `User` WHERE firstname='%s' AND lastname='%s'", firstname, lastname);

	if (mysql_query(object, query) != 0) {
		fprintf(stderr, "Failed to delete customer record: %s\n", mysql_error(object));
	}
	else {
		printf("Customer record deleted successfully.\n");
	}
}

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