#ifndef _USER_H_
#define _USER_H_

#include <iostream>
#include <vector>
#include "file_and_dir.h"

using namespace std;

class user;
class users;
class request;
class users_medium;

struct user_inf {
	user_inf(string un, int Allocated, double Used) : username(un), allocated(Allocated), used(Used) {}
	string username;
	int allocated;
	double used;
};

class request {
	private:
		string username;
		int amount;
		string status;
	public:
		request(int a, string un) : amount(a), username(un), status("pending") {}
		string get_status() { return status; }
		int get_amount() { return amount; }
		string get_user() { return username; }
		void accept();
		void reject();
};

struct user_perm {
	user_perm(string name, bool perm) : username(name), write_perm(perm) {}
	string username;
	bool write_perm;
};

class user {
	protected:
		string username;
		string password;
		int allocated_storage;
		double used_space;
		bool signed_in;
		vector<request*> requests;
		void add_request(request*);
	public:
		dir* root;
		dir* curr;
		user() {}
		user(string un, string pw) : username(un), password(pw), signed_in(false), allocated_storage(5), used_space(0) {
			root = new dir("/");
			curr = root;
			root->mkdir("shared", false);
		}
		string get_username() { return username; }
		bool is_password_valid(string pw) { return password == pw; }
		bool is_signed_in() { return signed_in; }
		bool sign_in() { if(signed_in == true) return false; signed_in = true; return true; }
		void sign_out() { signed_in = false; }
		string get_password() { return password; }
		void make_request(int amount, user* Admin) { requests.push_back(new request(amount, username)); if(this != Admin) Admin->add_request(requests[requests_size() - 1]); }
		int requests_size() { return requests.size(); }
		request* get_request(int idx) { return requests[idx]; }
		vector<request*> get_requests() { return requests; }
		int get_allocated_storage_amount() { return allocated_storage; }
		double get_used_space() { return used_space; }
		void share(string path, vector<user_perm> user_perm, users_medium* um, dir* Dir);
		void sharelist(string path, dir* Dir);
		void unshare(string path, vector<string> US, users_medium* um, dir* Dir);
		void rm_r(string path);
		void show_history(string path);
		void revert(string path, string index);
		int how_many_shared(string, dir*);
		void increase_used_space(long int l) { used_space += (double)l / 1000000; }
		void decrease_used_space(long int l) { used_space -= (double)l / 1000000; }
		void set_used_space(double Used_S) { used_space = Used_S; }
		friend class admin;
};

class admin : public user {
	public:
		void destroy_user(string username);
		void accept_request(int idx);
		void reject_request(int idx);
		vector<user_inf> userlist();
		static admin& access_admin_acc(users* US) {
			static admin Admin(US);
			return Admin;
		}
	private:
		users* US;
		admin(users* Us) : user("admin" , "s3cret"), US(Us) {}
};

class users {
	private:
		vector<user*> Users;
		int count;
	public:
		users() : count(0) {}
		users(const users&);
		~users();
		users& operator=(const users&);
		user operator[](int);
		user* operator()(int);
		user* get_user(string username);
		bool is_signed_in(string un);
		void add(string Username, string Password);
		void add(user);
		void fremove(string username);
		void remove(string username);
		bool is_any_signed_in() {
			for(int i = 0; i < count; i++)
				if(Users[i]->is_signed_in())
					return true;
			return false;
		}
		bool does_exist(string un);
		int size() { return count; }
		void sign_in_user(string un, string pw);
		void sign_out_everyone();
		string signed_in_users_name();
		user* signed_in_user();
};


class users_medium {
	public:
		bool is_user_perm_valid(user_perm);
		void share(file*, string, bool);
		void unshare(file*, string);
		bool does_exist(string username) {
			return US->does_exist(username);
		}
		static users_medium& access_medium(users* US) {
			static users_medium medium(US);
			return medium;
		}
	private:
		users* US;
		users_medium(users* Us) : US(Us) {}
};

#endif