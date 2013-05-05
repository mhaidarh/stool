/*
	SMTI06, 54411850, M Haidar Hanif
	Stubase | Simple student database
*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define NPM_LEN 9
#define MAX_LEN 32
#define MAX_NUM 7
#define MAX_ROW 10

struct Students {
	int id;
	int set;
	char npm[NPM_LEN];
	char name[MAX_LEN];
	float score;
};

struct Database {
	struct Students rows[MAX_ROW];
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void err(const char *message)
{
	if (errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}

	exit(1);
}

void Students_display(struct Students *addr)
{
	printf("%d\t%s\t%s\t%g\n",
		addr->id, addr->npm, addr->name, addr->score);
}

void load(struct Connection *conn)
{
	int rc = fread(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1) err("Failed to load database.");
}

struct Connection *open(const char *filename, char mode)
{
	struct Connection *conn = malloc(sizeof(struct Connection));

	if (!conn) err("Memory error");

	conn->db = malloc(sizeof(struct Database));

	if (!conn->db) err("Memory error");

	if (mode == 'c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");
		if (conn->file) {
		load(conn);
		}
	}

	if (!conn->file) err("Failed to open the file");

	return conn;
}

void close(struct Connection *conn)
{
	if (conn) {
		if (conn->file) fclose(conn->file);
		if (conn->db) free(conn->db);
		free(conn);
	}
}

void write(struct Connection *conn)
{
	rewind(conn->file);

	int rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if (rc != 1) err("Failed to write database.");

	rc = fflush(conn->file);
	if (rc == -1) err("Cannot flush database.");;
}

void create(struct Connection *conn)
{
	int i = 0;

	for (i = 0; i < MAX_ROW; i++) {
		struct Students addr = {.id = i, .set = 0};
		conn->db->rows[i] = addr;
	}
}

void insert(struct Connection *conn, int id, const char *npm, const char *name, float score)
{
	struct Students *addr = &conn->db->rows[id];
	if (addr->set) err("Already set, delete it first");

	addr->set = 1;

	char *res = strncpy(addr->npm, npm, NPM_LEN);
	if (!res) err("NPM set failed");

	res = strncpy(addr->name, name, MAX_LEN);
	if (!res) err("Name set failed");

	addr->score = score;
	if (!score) err("Score set failed");
}

void get(struct Connection *conn, int id)
{
	struct Students *addr = &conn->db->rows[id];

	if (addr->set) {
		Students_display(addr);
	} else {
		err("ID is not set or available");
	}
}

void delete(struct Connection *conn, int id)
{
	struct Students addr = {.id = id, .set = 0};
	conn->db->rows[id] = addr;
}

void print(struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;

	printf("%s\t%s\t\t%s\t\t%s\n",
		"ID", "NPM", "Name", "Score");

	for(i = 0; i < MAX_ROW; i++) {
		struct Students *cur = &db->rows[i];
		Students_display(cur);
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3) err("Usage: ./stubase <databasefile> <action> [parameters]");

	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = open(filename, action);
	int id = 0;

	if (argc > 3) id = atoi(argv[3]);
	if (id >= MAX_ROW) err("There's not that many records.");

	switch(action) {
		case 'h':
		if (argc == 3) printf("Stubase Help\n\n"
		"Usage: ./stubase <databasefile> <action> [parameters]\n\n"
		"Available action:\n"
		"c = Create database\n"
		"i = Insert data into database\n"
		"d = Delete data from database\n"
		"g = Get data from database\n"
		"p = Print data from database\n\n"
		"See README for more details\n");
		break;

		case 'c':
		create(conn);
		write(conn);
		break;

		case 'i':
		if (argc != 7) err("Need id, npm, name, and score to insert");
		insert(conn, id, argv[4], argv[5], atof(argv[6]));
		write(conn);
		break;

		case 'd':
		if (argc != 4) err("Need an id to delete");
		delete(conn, id);
		write(conn);
		break;

		case 'g':
		if (argc != 4) err("Need an id to get / row is available");
		get(conn, id);
		break;

		case 'p':
		print(conn);
		break;

		default:
		err("Invalid action, only: c=create, i=insert, d=delete, g=get, p=print");
	}

	close(conn);

	return 0;
}
