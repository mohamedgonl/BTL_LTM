
#include "stdafx.h"
#include "StructDefination.h"
#ifndef Controllers_h
#define Controllers_h




// 2. Login
string loginAccount(string username, string password, UserInfo* userInfo) {

	// check if account is logged in
	for (int i = 0; i < sizeof (loginSessions); i++) {
		if (!strcmp((loginSessions[i]->userInfo.username).c_str, username.c_str)) return "113";
	}
	// if this account has not logged in
	for (int i = 0; i < sizeof(accounts); i++) {
		if (!strcmp(accounts[i].username.c_str, username.c_str)) {
			if ( !strcmp(accounts[i].password.c_str, password.c_str) ) { // if username and password matched
				// update user info
				userInfo->username = username;
				userInfo->status = 1;
				return "110";
			}
			else return "112";
		}
	}
	return "111";
}

// 3. Register

string registerAccount (string username, string password) {
	for (int i = 0; i < sizeof(accounts); i++) {
		if (!strcmp(accounts[i].username.c_str, username.c_str)) // if username is existed
			return "121";
	}

	// save account to data file
	
	try
	{
		ofstream file(fileDirectory);

	}
	catch (const std::exception&)
	{
		cout << "Error at function 3.Register: Save data error\n";
	}
	
	

	return "120";
}

// 4. Get list all teams


#endif 
