//
// Created by Yusuf Sherif on 21/11/2021.
//

#include <cstring>
#include "../include/Message.h"

Message::Message(int p_operation, void *p_message, size_t p_message_size, int p_rpc_id) {
	message = new char[message_size];
	memcpy(message, p_message, message_size);
	operation = p_operation;
	message_size = p_message_size;
	rpc_id = p_rpc_id;
}
Message::Message(std::string marshalled_base64) {
	std::string str = base64_decode(marshalled_base64);
	size_t old_index = 0;
	size_t index = str.find('|');
	message_type = (MessageType)(atoi(str.substr(old_index, index).c_str()));
	old_index = index;
	index = str.find('|', index);
	operation = atoi(str.substr(old_index, index).c_str());
	old_index = index;
	index = str.find('|', index);
	message_size = atoi(str.substr(old_index, index).c_str());
	old_index = index;
	index = str.find('|', index);
	rpc_id = atoi(str.substr(old_index, index).c_str());

	old_index = index;
	index = str.length() - 1;
	std::string msg = str.substr(old_index, index);
	memcpy(message, msg.c_str(), msg.length());
}
std::string Message::marshal() {
	std::string str = toStirng();
	std::string encoded_str = base64_encode(str);
	return encoded_str;
}
int Message::getOperation() {
	return operation;
}
int Message::getRPCId() {
	return rpc_id;
}
void *Message::getMessage() {
	return message;
}
size_t Message::getMessageSize() {
	return message_size;
}
MessageType Message::getMessageType() {
	return message_type;
}
void Message::setOperation(int _operation) {
	operation = _operation;
}
void Message::setMessage(void *p_message, size_t message_size) {
	memcpy(message, p_message, message_size);
}
void Message::setMessageType(MessageType p_message_type) {
	message_type = p_message_type;
}
Message::~Message() {
	delete[] message;
}
std::string Message::toStirng() {
	std::stringstream s;
	s << message_type << "|" << operation << "|" << message_size << "|" << rpc_id << "|" << message;
	return s.str();
}
