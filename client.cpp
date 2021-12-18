#include "include/Client.h"
#include "libs/nlohmann_json/json.hpp"
#include "include/imageOperations.hpp"
#include "include/serialization.hpp"

typedef std::vector<std::vector<std::string>> table_type;

bool signup(Client &c, const std::string &username, const std::string &password) {
	nlohmann::json j;
	j["username"] = username;
	j["password"] = password;
	Message m(Request, "signup", j.dump(), 0);
	Message recvd = c.execute(m);
	bool res = deserialize<bool>(m.getMessage());
	return res;
}

bool signin(Client &c, const std::string &username, const std::string &password) {
	nlohmann::json j;
	j["username"] = username;
	j["password"] = password;
	Message m(Request, "signin", j.dump(), 0);
	Message recvd = c.execute(m);
	bool res = deserialize<bool>(m.getMessage());
	return res;
}

bool insert_image(Client &c,
				  const std::string &username,
				  const std::string &password,
				  const std::string &img_path,
				  const std::vector<std::vector<std::string>> &accessList) {
	nlohmann::json j;
	j["username"] = username;
	j["password"] = password;

	auto img = open_image(img_path);
	std::string encoded_img = encode_image(img);
	j["image"] = encoded_img;
	j["access_list"] = accessList;

	Message m(Request, "insert_image", j.dump(), 0);
	Message recvd = c.execute(m);
	bool res = deserialize<bool>(m.getMessage());
	return res;
}

table_type get_my_images(Client &c, const std::string &username, const std::string &password) {
	nlohmann::json j;
	j["username"] = username;
	j["password"] = password;

	Message m(Request, "get_my_images", j.dump(), 0);
	Message recvd = c.execute(m);
	std::string pl = recvd.getMessage();
	auto table = deserialize<table_type>(pl);
	return table;
}

table_type get_accessible_images(Client &c, const std::string &username, const std::string &password) {
	nlohmann::json j;
	j["username"] = username;
	j["password"] = password;

	Message m(Request, "get_accessible_images", j.dump(), 0);
	Message recvd = c.execute(m);
	std::string pl = recvd.getMessage();
	auto table = deserialize<table_type>(pl);
	return table;
}

std::string getOwnedImage(Client &c, const std::string &username, const std::string &password, const std::string &img_id) {
	nlohmann::json j;
	j["username"] = username;
	j["password"] = password;
	j["image_id"] = img_id;

	Message m(Request, "getOwnedImage", j.dump(), 0);
	Message recvd = c.execute(m);
	std::string pl = recvd.getMessage();
	if (!pl.empty()) {
		auto img = decode_image(pl);
		save_image(img, "images/getOwnedImageRes.png");
		std::cout << "getOwnedImage Secret Text: " << cppsteg::leastBit::decodeText("images/getOwnedImageRes.png")
				  << std::endl;
	} else {
		std::cout << "getOwnedImage payload empty! " << std::endl;
	}
	return pl;
}

std::string getAccessibleImage(Client &c,
						const std::string &username,
						const std::string &password,
						const std::string &img_id) {
	nlohmann::json j;
	j["username"] = username;
	j["password"] = password;
	j["image_id"] = img_id;

	Message m(Request, "getAccessibleImage", j.dump(), 0);
	Message recvd = c.execute(m);
	std::string pl = recvd.getMessage();
	if (!pl.empty()) {
		auto img = decode_image(pl);
		save_image(img, "images/getAccessibleImageRes.png");
		std::cout << "getAccessibleImageRes Secret Text: "
				  << cppsteg::leastBit::decodeText("images/getAccessibleImageRes.png")
				  << std::endl;
	} else {
		std::cout << "getAccessibleImage payload empty! "
				  << std::endl;
	}
	return pl;
}

int main() {
	Client c("127.0.0.1", 8000);
	signup(c, "youssef", "sherif");
	signin(c, "youssef", "sherif");
	std::vector<std::vector<std::string>> accessList;
	accessList.push_back({"hello", "5"});
	insert_image(c, "youssef", "sherif", "images/input.png", accessList);
	table_type my_imgs = get_my_images(c, "hello", "world");
	table_type accessible_imgs = get_accessible_images(c, "hello", "world");
	getOwnedImage(c, "hello", "world", my_imgs[0][0]);
	getAccessibleImage(c, "hello", "world", accessible_imgs[0][0]);
}