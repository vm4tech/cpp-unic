#pragma once
class UserChat
{
	
public:
	char friendSocket[6];
	CListBox currentChat;
	// Construction
	UserChat(CListBox clb, char* friendSocket);
	void setFrendSocket(char* friendSocket);
};

