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
	std::string operation;
	std::string payload;
	int rpc_id;
public:
	Message();
	Message(MessageType p_message_type, const std::string& operation, const std::string& p_message, int p_rpc_id);
	Message(const std::string& marshalled_base64);
	std::string marshal();
	std::string getOperation();
	int getRPCId();
	std::string getMessage();
	size_t getMessageSize();
	MessageType getMessageType();
	void setOperation(const std::string& _operation);
	void setPayload(const std::string &message);
	void setMessageType(MessageType message_type);
	std::string toStirng();
	~Message();
};
#endif // MESSAGE_H