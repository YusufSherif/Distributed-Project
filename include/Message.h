#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdio>
#include <sstream>
#include <string>
#include "../libs/base64.h"

enum MessageType { Request, Reply };
class Message {
private:
	MessageType message_type;
	int operation;
	void *message;
	size_t message_size;
	int rpc_id;
public:
	Message(int operation, const void *p_message, size_t p_message_size, int p_rpc_id);
	Message(std::string marshalled_base64);
	std::string marshal();
	int getOperation();
	int getRPCId();
	void *getMessage();
	size_t getMessageSize();
	MessageType getMessageType();
	void setOperation(int _operation);
	void setMessage(void *message, size_t message_size);
	void setMessageType(MessageType message_type);
	std::string toStirng();
	~Message();
};
#endif // MESSAGE_H