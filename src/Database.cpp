//
// Created by Yusuf Sherif on 16/12/2021.
//

#include "../include/Database.h"

Database::Database(const std::string &filename) {
	rc = sqlite3_open(filename.c_str(), &db);

	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	} else {
		fprintf(stdout, "Opened database successfully\n");
	}
}

Database::~Database() {
	sqlite3_close(db);
}

std::string Database::signup(const std::string &username, const std::string &password) {
	std::string sql = "INSERT INTO users "  \
         "VALUES ('" + username + "', '" + password + "');";

	rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &zErrMsg);
	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return serialize<bool>(false);
	} else {
		fprintf(stdout, "Records created successfully\n");
	}
	return serialize<bool>(true);
}
std::string Database::signin(const std::string &username, const std::string &password) {
	std::string sql = "SELECT count(*) from users WHERE username='" + username + "' AND password='" + password + "';";

	int count;

	rc = sqlite3_exec(db, sql.c_str(), count_callback, &count, &zErrMsg);
	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}

	return serialize<bool>(count == 1);
}

int Database::count_callback(void *count, int argc, char **argv, char **azColName) {
	int *c = static_cast<int *>(count);
	*c = atoi(argv[0]);
	return 0;
}

std::string Database::insert_image(const std::string &username, const std::string &password, const std::string &image,
								   const std::vector<std::vector<std::string>> &accessList) {

	std::string sql = "INSERT INTO image(original_image, owner) "  \
         "VALUES ('" + image + "', '" + username + "');";

	rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &zErrMsg);
	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return serialize<bool>(false);
	} else {
		fprintf(stdout, "Inserted image successfully\n");
	}

	sql = "SELECT count(*) from image;";

	int count;

	rc = sqlite3_exec(db, sql.c_str(), count_callback, &count, &zErrMsg);
	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return serialize<bool>(false);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}

	sql = "INSERT INTO users_has_access VALUES";
	for (auto &i:accessList) {
		sql += "('" + i[0] + "', " + std::to_string(count) + ", 0, " + i[1] + "),";
	}
	sql[sql.length() - 1] = ';';

	rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &zErrMsg);
	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return serialize<bool>(false);
	} else {
		fprintf(stdout, "Inserted access list successfully\n");
	}

	return serialize<bool>(true);
}

std::string Database::get_my_images(const std::string &username, const std::string &password) {
	bool authenticated = std::stoi(signin(username, password));
	table_type table = table_type();
	if (!authenticated)
		return serialize<table_type>(table);

	std::string sql = "SELECT id from image WHERE owner='" + username + "';";

	rc = sqlite3_exec(db, sql.c_str(), callback, &table, &zErrMsg);
	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}
	return serialize<table_type>(table);
}

int Database::callback(void *ptr, int argc, char **argv, char **cols) {
	auto *table = static_cast<table_type *>(ptr);
	std::vector<std::string> row;
	row.reserve(argc);
	for (int i = 0; i < argc; i++)
		row.emplace_back(argv[i] ? argv[i] : "NULL");
	table->push_back(row);
	return 0;
}

std::string Database::get_accessible_images(const std::string &username, const std::string &password) {
	bool authenticated = std::stoi(signin(username, password));
	table_type table = table_type();
	if (!authenticated)
		return serialize<table_type>(table);

	std::string sql =
	"SELECT image_id from users_has_access JOIN image ON image.id = users_has_access.image_id WHERE users_username='"
	+ username
	+ "' AND views < max_views;";

	rc = sqlite3_exec(db, sql.c_str(), callback, &table, &zErrMsg);
	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}
	return serialize<table_type>(table);
}

std::string Database::getImage(const std::string username, const std::string &password, const std::string image_id) {
	std::string res;
	table_type table;

	bool authenticated = std::stoi(signin(username, password));
	if (!authenticated)
		return res;

	std::string sql =
	"SELECT original_image from users_has_access JOIN image ON image.id = users_has_access.image_id WHERE ((users_username='"
	+ username
	+ "' AND views < max_views) OR (owner='" + username + "')) AND image.id = "
	+ image_id + ";";

	rc = sqlite3_exec(db, sql.c_str(), callback, &table, &zErrMsg);
	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return res;
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}

	if (!table.empty()) {
		res = table[0][0];
	}

	return res;
}
std::string Database::getOwnedImage(const std::string &username,
									const std::string &password,
									const std::string &image_id) {
	return getImage(username, password, image_id);
}

std::string Database::getAccessibleImage(const std::string &username,
										 const std::string &password,
										 const std::string &image_id) {

	std::string image;
	image = getImage(username, password, image_id);
	if (!image.empty()) {
		std::string sql = "UPDATE users_has_access SET views = views + 1 WHERE image_id = " + image_id + ";";
		rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &zErrMsg);
		sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

		if (rc != SQLITE_OK) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		} else {
			fprintf(stdout, "Record updated successfully\n");
		}
	}

	return image;
}
std::string Database::getAccessibleImageDetails(const std::string &username,
												const std::string &password,
												const std::string &image_id) {
	bool authenticated = std::stoi(signin(username, password));
	table_type table = table_type();
	if (!authenticated)
		return serialize<table_type>(table);

	std::string sql =
	"SELECT id, owner, max_views, views from users_has_access JOIN image ON image.id = users_has_access.image_id WHERE image_id="
	+ image_id + " AND (users_username = '" + username + "' OR owner = '" + username + "');";

	rc = sqlite3_exec(db, sql.c_str(), callback, &table, &zErrMsg);
	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}
	return serialize<table_type>(table);
}
std::string Database::getOwnedImageDetails(const std::string &username,
										   const std::string &password,
										   const std::string &image_id) {
	bool authenticated = std::stoi(signin(username, password));
	table_type table = table_type();
	if (!authenticated)
		return serialize<table_type>(table);

	std::string sql =
	"SELECT users_username, views, max_views from users_has_access JOIN image ON image.id = users_has_access.image_id WHERE image_id="
	+ image_id + " AND owner = '" + username + "';";

	rc = sqlite3_exec(db, sql.c_str(), callback, &table, &zErrMsg);
	sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	} else {
		fprintf(stdout, "Operation done successfully\n");
	}
	return serialize<table_type>(table);
}

