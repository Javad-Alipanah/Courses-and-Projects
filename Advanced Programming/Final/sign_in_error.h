#ifndef _SIGN_IN_ERROR_H_
#define _SIGN_IN_ERROR_H_

#include <iostream>
using namespace std;

class sign_in_error {
private:
	string error_msg;
	string step;
public:
	sign_in_error(string Error_msg, string Step) : error_msg(Error_msg), step(Step) {}
	string get_error_msg() { return error_msg; }
	string get_step() { return step; }
};

#endif