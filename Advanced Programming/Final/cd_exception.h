#include <iostream>
using namespace std;

class cd_exception {
	private:
		string msg;
	public:
		cd_exception(string MSG) : msg(MSG) {}
		string get_msg() { return msg; }
};