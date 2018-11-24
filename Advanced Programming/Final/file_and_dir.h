#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sys/stat.h>
#include "cd_exception.h"

using namespace std;

class user;
class users;

struct shared_file_detail {
	shared_file_detail(user* u, int Shared_idx, bool Write_perm, string N) : us(u), shared_idx(Shared_idx), write_perm(Write_perm), name(N) {}
	user* us;
	int shared_idx;
	bool write_perm;
	string name;
};

enum file_or_dir {
	Filee,
	Dirr,
	Not_found
};

class file_details {
	private:
		string owner;
		vector<shared_file_detail> shared_with;
	public:
		string get_owner() { return owner; }
		bool is_shared_with(string);
		bool get_write_perm_of(string);
		string get_related_name_to(string);
		void set_owner(string Owner) { owner = Owner; }
		vector<shared_file_detail> get_shared_with() { return shared_with; }
		int get_shfd_idx(string);
		void set_perm(int idx, bool perm) { shared_with[idx].write_perm = perm; }
		void add_shfd(shared_file_detail x) { shared_with.push_back(x); }
		void delete_shfd(int idx) { shared_with.erase(shared_with.begin() + idx); }
};

struct history {
	history(string Name, string SP, long int l, string un, string Cmd) : name(Name), storage_path(SP), length(l), username(un), cmd(Cmd) {}
	string name;
	string storage_path;
	long int length;
	string username;
	string cmd;
};

class file {
	private:
		string name;
		vector<history> change_log;
	public:
		file_details fd;
		file(string Name) : name(Name) {}
		file(string Name, string owner, long int Length, string username, string cmd);
		file(string Name, file_details FD, long int Length, string username, string cmd);
		file(string Name, string st, string username, string cmd) { change_log.push_back(history(Name, st, 0, username, cmd)); }
		string get_name() { return name; }
		string get_storage_path() { return change_log[change_log.size() - 1].storage_path; }
		long int get_length() { return change_log[change_log.size() - 1].length; }
		long int get_t_length() { long int l = 0; for(int i = 0; i < change_log.size(); ++i) l += change_log[i].length; return l; }
		void set_length(long int Length, string un, string cmd);
		void add_history(string Name, long int l ,string un, string cmd);
		void revert(int idx, user* owner = NULL);
		vector<history> get_change_log() { return change_log; }
};

class dir {
	private:
		string name;
		bool write_enabled;
		dir* prev;
		vector<dir*> in_dir;
		vector<file*> in_files;
	public:
		dir(string Name, dir* Prev = NULL) : name(Name), prev(Prev), write_enabled(true) {}
		dir(string Name,bool WE , dir* Prev = NULL) : name(Name), prev(Prev), write_enabled(WE) {}
		string get_name() const { return name; }
		bool get_perm() { return write_enabled; }
		void mkdir(string Name, bool WE = true);
		string pwd();
		dir* cd(string path = "");
		vector<string> ls(user* u, string path = "");
		void rm(string file_path, user* u);
		void rm_r(string dir_path, user* u);
		void mv(string first_path, string sec_path, user* owner);
		void cp(string first_path, string sec_path, user* owner, users* US);
		void cat(string in_path, user* u);
		void put(string src, string des, user* owner, users* US);
		void get(string src, string des, user* u);
		friend class user;
		friend class users_medium;
		friend class users;
		friend file_or_dir check_type(string, dir*, int&, user*);
		friend void cut_paste(file*, file*, dir*, string, string);
};