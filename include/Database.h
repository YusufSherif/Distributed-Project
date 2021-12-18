//
// Created by Yusuf Sherif on 16/12/2021.
//

#ifndef PROJECT_2_INCLUDE_DATABASE_H_
#define PROJECT_2_INCLUDE_DATABASE_H_
#include <sqlite3.h>
#include <iostream>
#include <utility>
#include <vector>
#include "serialization.hpp"

typedef std::vector<std::vector<std::string>> table_type;

class Database {
private:
	sqlite3 *db;
	int rc;
	char *zErrMsg;
	static int callback(void *ptr, int argc, char* argv[], char* cols[]);
	static int count_callback(void *count, int argc, char **argv, char **azColName);
	std::string getImage(const std::string username, const std::string& password, const std::string image_id);
public:
	Database(const std::string& filename);
	std::string signup(const std::string& username, const std::string& password);
	std::string signin(const std::string& username, const std::string& password);
	std::string insert_image(const std::string& username, const std::string& password, const std::string& image, const std::vector<std::vector<std::string>>& accessList);
	std::string get_my_images(const std::string& username, const std::string& password);
	std::string get_accessible_images(const std::string& username, const std::string& password);
	std::string getOwnedImage(const std::string &username, const std::string& password, const std::string &image_id);
	std::string getAccessibleImage(const std::string &username, const std::string& password, const std::string &image_id);
	std::string getAccessibleImageDetails(const std::string &username, const std::string& password, const std::string &image_id);
	std::string getOwnedImageDetails(const std::string &username, const std::string& password, const std::string &image_id);
	~Database();
};

#endif //PROJECT_2_INCLUDE_DATABASE_H_
