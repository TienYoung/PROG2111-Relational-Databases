#define _CRT_SECURE_NO_WARNINGS

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	const char* server = "localhost";
	const char* username = "root";
	const char* password = "constantine";
	const char* defaultDatabase = "sakila";

	// 1.Initialize a database connection object
	MYSQL* databaseObject = mysql_init(NULL);

	if (databaseObject == NULL) 
	{
		printf("Failed to initialize the database object");
		return EXIT_FAILURE;
	}

	// 2.Connect to the database
	if (!mysql_real_connect(databaseObject, server, username, password, defaultDatabase, 0, NULL, 0))
	{
		printf("Failed to connect to the database: Error: %s\n", mysql_error(databaseObject));
		return EXIT_FAILURE;
	}


	// 3.Execute a query
	const char* query = "SELECT actor_id, first_name, last_name FROM actor WHERE first_name LIKE \'M%\'";
	if (mysql_query(databaseObject, query) != 0)
	{
		printf("Failed to execute the query: Error: %s\n", mysql_error(databaseObject));
		mysql_close(databaseObject);
		return EXIT_FAILURE;
	}

	// 4.Get the result set
	MYSQL_RES* resultSet = mysql_store_result(databaseObject);

	if (resultSet == NULL)
	{
		printf("Failed to get the result set: Error: %s\n", mysql_error(databaseObject));
		mysql_close(databaseObject);
		return EXIT_FAILURE;
	}

	//5. Print the result set
	MYSQL_ROW row;
	while((row = mysql_fetch_row(resultSet)) != NULL)
	{
		printf("Actor ID: %s, First Name: %s, Last Name: %s\n", row[0], row[1], row[2]);
	}

	// 6. Free the result set and close the database connection
	mysql_free_result(resultSet);


	// Update an Existing Record
	const char* getActorWithActorId1 = "SELECT * FROM actor WHERE actor_id = 1";

	if (mysql_query(databaseObject, getActorWithActorId1) != 0)
	{
		printf("Failed to execute the query: Error: %s\n", mysql_error(databaseObject));
		mysql_close(databaseObject);
		return EXIT_FAILURE;
	}

	MYSQL_RES* resultSetForActorWithActorId1 = mysql_store_result(databaseObject);
	int numberOfRows = mysql_num_rows(resultSetForActorWithActorId1);
	if (numberOfRows == 0)
	{
		printf("No actor with actor_id = 1 found\n");
		mysql_close(databaseObject);
		return EXIT_FAILURE;
	}

	if (numberOfRows == 1)
	{
		MYSQL_ROW rowForActorWithActorId1 = mysql_fetch_row(resultSetForActorWithActorId1);
		printf("Actor ID: %s, First Name: %s, Last Name: %s\n", rowForActorWithActorId1[0], rowForActorWithActorId1[1], rowForActorWithActorId1[2]);
	}

	const char* updateActor = "UPDATE actor SET first_name = 'John' WHERE actor_id = 1;";

	if (mysql_query(databaseObject, updateActor) != 0)
	{
		printf("Failed to execute the query: Error: %s\n", mysql_error(databaseObject));
		mysql_close(databaseObject);
		return EXIT_FAILURE;
	}

	mysql_close(databaseObject);

	return EXIT_SUCCESS;
}


