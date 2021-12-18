#include "../include/Server.h"

Message Server::getRequest() {
	std::vector<char> buffer;
	static const size_t MaxBytesPerRecv = 1024;
	int bytesRead = 0;
	int i = 0;
	int totalSize = 0;
	do {
		const size_t oldSize = buffer.size();

		buffer.resize(oldSize + MaxBytesPerRecv);
		bytesRead =
		udpServerSocket->readFromSocketWithTimeout(&(buffer.data())[(i) * (MaxBytesPerRecv)], MaxBytesPerRecv, 1, 0);
		if (bytesRead == -1) {
			std::cout << "No bytes read from socket" << std::endl;
			break;
		}
		i++;
		buffer.resize(oldSize + bytesRead);
		totalSize = oldSize + bytesRead;
	} while (bytesRead > 0);

	std::cout << "Server: read the following from socket " + std::string(buffer.data()) << std::endl;

	buffer.resize(totalSize + 1);
	buffer[totalSize + 1] = '\0';
	if (totalSize <= 0) {
		return Message();
	}
	Message resultM(buffer.data());
	std::cout << "Server: recieved payload " + resultM.toStirng() << std::endl;
	return resultM;
}

Message Server::doOperation(Message _message) {
	std::string op = _message.getOperation();
	std::string payload = _message.getMessage();
	nlohmann::json req = nlohmann::json::parse(payload);
	Message reply(Reply, "", "", 0);
	if (op == "signin") {
		reply.setPayload(db.signin(req["username"], req["password"]));
	}
	if (op == "signup") {
		reply.setPayload(db.signup(req["username"], req["password"]));
	}
	if (op == "insert_image") {
		auto access_list = req["access_list"].get<std::vector<std::vector<std::string>>>();
		auto res = db.insert_image(req["username"], req["password"], req["image"], access_list);
		reply.setPayload(res);
	}
	if(op=="get_my_images") {
		reply.setPayload(db.get_my_images(req["username"], req["password"]));
	}
	if(op=="get_accessible_images"){
		reply.setPayload(db.get_accessible_images(req["username"], req["password"]));
	}
	if(op=="getOwnedImage"){
		auto img = decode_image(db.getOwnedImage(req["username"], req["password"], req["image_id"]));
		auto img_data = db.getOwnedImageDetails(req["username"], req["password"], req["image_id"]);
		auto info = deserialize<table_type>(img_data);
		if(!info.empty() && !img.empty()) {
			nlohmann::json j;
			j["id"] = req["image_id"];
			j["owner"] = req["username"];
			j["access_list"] = info;
			cv::Mat img_mat = cv::imdecode(img, -1);
			cv::Mat stego = cppsteg::leastBit::encodeText(img_mat, j.dump());
			std::vector<uchar> buf;
			cv::imencode(".png", stego, buf);
			std::string stego_encoded_img = encode_image(buf);
			reply.setPayload(stego_encoded_img);
		}
	}
	if(op=="getAccessibleImage"){
		auto img = decode_image(db.getAccessibleImage(req["username"], req["password"], req["image_id"]));
		auto img_data = db.getAccessibleImageDetails(req["username"], req["password"], req["image_id"]);
		auto info = deserialize<table_type>(img_data);
		if(!info.empty() && !img.empty()) {
			nlohmann::json j;
			j["id"] = info[0][0];
			j["owner"] = info[0][1];
			j["maxViews"] = info[0][2];
			j["views"] = info[0][3];
			cv::Mat img_mat = cv::imdecode(img, -1);
			cv::Mat stego = cppsteg::leastBit::encodeText(img_mat, j.dump());
			std::vector<uchar> buf;
			cv::imencode(".png", stego, buf);
			std::string stego_encoded_img = encode_image(buf);
			reply.setPayload(stego_encoded_img);
		}
	}
	return reply;
}

void Server::sendReply(Message _message) {
	std::string msg = _message.marshal();
	udpServerSocket->writeToSocket(msg.data(), 1024);
	std::cout << "Sent Message: " + msg << std::endl;
}

Server::Server(const char *_listen_hostAddr, int _listen_port, const std::string &db_path) : db(db_path) {
	udpServerSocket = new UDPServerSocket();
	udpServerSocket->initializeServer(_listen_hostAddr, _listen_port);
}
void Server::serveRequest() {
	Message m = getRequest();
	if (m.getMessageSize() == 0)
		return;
	sendReply(doOperation(m));
}

Server::~Server() {
	delete udpServerSocket;
}
