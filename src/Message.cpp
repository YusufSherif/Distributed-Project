//
// Created by Yusuf Sherif on 21/11/2021.
//

#include "../include/Message.h"

Message::Message(int operation, void *p_message, size_t p_message_size, int p_rpc_id) {

}
Message::Message(char *marshalled_base64) {

}
char *Message::marshal() {
	return nullptr;
}
int Message::getOperation() {
	return 0;
}
int Message::getRPCId() {
	return 0;
}
void *Message::getMessage() {
	return nullptr;
}
size_t Message::getMessageSize() {
	return 0;
}
MessageType Message::getMessageType() {
	return Reply;
}
void Message::setOperation(int _operation) {

}
void Message::setMessage(void *message, size_t message_size) {

}
void Message::setMessageType(MessageType message_type) {

}
Message::~Message() {

}
