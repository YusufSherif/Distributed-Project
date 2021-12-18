//
// Created by Yusuf Sherif on 21/11/2021.
//

#include <cstring>
#include "../include/Message.h"

Message::Message(MessageType p_message_type, const std::string &p_operation, const std::string &p_message, int p_rpc_id) {
	operation = p_operation;
	rpc_id = p_rpc_id;
	payload = p_message;
	message_type = p_message_type;
}
Message::Message(const std::string& marshalled_base64) {
	std::string str = base64_decode(marshalled_base64);
	int old_index = 0;
	int index = str.find('|');
	message_type = (MessageType)(atoi(str.substr(old_index, index - old_index).c_str()));
	old_index = ++index;
	index = str.find('|', index);
	operation = str.substr(old_index, index - old_index).c_str();
	old_index = ++index;
	index = str.find('|', index);
	rpc_id = atoi(str.substr(old_index, index - old_index).c_str());
	old_index = ++index;
	index = str.length();
	payload = str.substr(old_index, index - old_index);
}
std::string Message::marshal() {
	std::string str = toStirng();
	std::string encoded_str = base64_encode(str);
	return encoded_str;
}
std::string Message::getOperation() {
	return operation;
}
int Message::getRPCId() {
	return rpc_id;
}
std::string Message::getMessage() {
	return payload;
}
MessageType Message::getMessageType() {
	return message_type;
}
void Message::setOperation(const std::string &_operation) {
	operation = _operation;
}
void Message::setPayload(const std::string &message) {
	payload = message;
}
void Message::setMessageType(MessageType p_message_type) {
	message_type = p_message_type;
}
Message::~Message() {
}

std::string Message::toStirng() {
	std::stringstream s;
	s << message_type << "|" << operation << "|" << rpc_id << "|" << payload;
	return s.str();
}
Message::Message() {
	rpc_id = 0;
	payload = "";
}
size_t Message::getMessageSize() {
	return payload.size();
}
