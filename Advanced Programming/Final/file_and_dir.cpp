#include <algorithm>
#include <cstdio>
#include "user.h"


file::file(string Name, string owner, long int Length, string username, string cmd) {
	string temp = "./storage/";
	string num = "";
	long int number = (long int)&change_log;
	int i = 0;
	while(number != 0) {
		num += number % 10 + 48;
		number /= 10;
	}
	reverse(num.begin(), num.end());
	temp += num;
	temp += Name;
	change_log.push_back(history(Name, temp, Length, username, cmd));
	name = Name;
	fd.set_owner(owner);
}

file::file(string Name, file_details FD, long int Length, string username, string cmd) : fd(FD) {
	string temp = "./storage/";
	string num = "";
	long int number = (long int)&change_log;
	int i = 0;
	while(number != 0) {
		num += number % 10 + 48;
		number /= 10;
	}
	reverse(num.begin(), num.end());
	temp += num;
	temp += Name;
	change_log.push_back(history(Name, temp, Length, username, cmd));
	name = Name;
}

void file::set_length(long int Length, string un, string cmd) {
	string temp = "./storage/";
	string num = "";
	long int number = (long int)&change_log + change_log.size();
	int i = 0;
	while(number != 0) {
		num += number % 10 + 48;
		number /= 10;
	}
	reverse(num.begin(), num.end());
	temp += num;
	temp += change_log[change_log.size() - 1].name;
	change_log.push_back(history(get_name(), temp, Length, un, cmd));
}

void file::add_history(string Name, long int l ,string un, string cmd) { 
	string temp = "./storage/";
	string num = "";
	long int number = (long int)&change_log + change_log.size();
	int i = 0;
	while(number != 0) {
		num += number % 10 + 48;
		number /= 10;
	}
	reverse(num.begin(), num.end());
	temp += num;
	temp += Name;
	change_log.push_back(history(Name, temp, l, un, cmd));
	name = Name;
}

void file::revert(int idx, user* owner) { 
	for(int i = idx + 1; i < change_log.size(); ++i) {
		string temp = change_log[i].storage_path;
		const char* c = temp.c_str();
		remove(c);
		if(owner != NULL)
			owner->decrease_used_space(change_log[i].length);
	}
	change_log.erase(change_log.begin() + (idx + 1), change_log.end());
	if (change_log.size())
		name = change_log[change_log.size() - 1].name;
}


bool file_details::is_shared_with(string username) {
	for(int i = 0; i < shared_with.size(); i++)
		if(shared_with[i].us->get_username() == username)
			return true;
	return false;
}

int file_details::get_shfd_idx(string username) {
	for(int i = 0; i < shared_with.size(); i++)
		if(shared_with[i].us->get_username() == username)
			return i;
}

bool file_details::get_write_perm_of(string username) {
	for(int i = 0; i < shared_with.size(); i++)
		if(shared_with[i].us->get_username() == username)
			return shared_with[i].write_perm;
}

string file_details::get_related_name_to(string username) {
	for(int i = 0; i < shared_with.size(); i++)
		if(shared_with[i].us->get_username() == username)
			return shared_with[i].name;
}

vector<string> parse_path(string path) {
	vector<string> result;
	string temp = "";
	for(int i = 0; i < path.length(); i++) {
		if(path[i] != '/')
			temp += path[i];
		else {
			if(temp != "")
				result.push_back(temp);
			temp = "";
		}
	}
	result.push_back(temp);
	return result;
}

file_or_dir check_type(string x, dir* Dir, int& idx, user* u) {
	if(!x.length())
		return Not_found;
	bool shared_dir = false;
	if(!Dir->get_perm())
		shared_dir = true;
	if(u == NULL)
		shared_dir = false;
	for(int i = 0; i < Dir->in_files.size(); i++)
		if(!shared_dir) {
			if(x == Dir->in_files[i]->get_name()) {
				idx = i;
				return Filee;
			}
		}
		else {
			if(x == Dir->in_files[i]->fd.get_related_name_to(u->get_username())) {
				idx = i;
				return Filee;
			}
		}
	for(int i = 0; i < Dir->in_dir.size(); i++)
		if(x == Dir->in_dir[i]->get_name()) {
			idx = i;
			return Dirr;
		}
	return Not_found;
}

string dir::pwd() {
	if(get_name() == "/" || prev == NULL)
		return "/";
	return prev->pwd() + get_name() + "/";
}

void dir::mkdir(string new_dir_path, bool WE) {
	vector<string> words = parse_path(new_dir_path);
	string path = "";
	int idx = words.size() - 2;
	if(words[words.size() - 1].length())
		idx = words.size() - 1;
	for(int i = 0; i < idx; i++) {
		path += words[i];
		path += "/";
	}
	dir* Dir = this;
	if(words.size() != 1) {
		try {
			Dir = cd(path);
		} catch(cd_exception ex) {
			cout << "mkdir: cannot create directory ‘" + new_dir_path + "’: No such file or directory" << endl;
			return;
		}
	}
	for(int i = 0; i < Dir->in_dir.size(); i++)
		if(Dir->in_dir[i]->get_name() == words[idx]) {
			cout << "mkdir: cannot create directory ‘" + words[idx] + "’: File exists" << endl;
			return;
		}
	if(Dir->get_perm())
		Dir->in_dir.push_back(new dir(words[idx], WE, Dir));
	else {
		cout << "mkdir: cannot create directory ‘" + words[idx] + "’: Permission denied" << endl;
		return;
	}
}

vector<string> dir::ls(user* u, string path) {
	dir* Dir = this;
	vector<string> result;
	if(path != "") {
		try {
			Dir = cd(path);
		} catch(cd_exception ex) {
			cout << "ls: cannot access " + path + ": No such file or directory" << endl;
			return result;
		}
	}
	for(int i = 0; i < Dir->in_dir.size(); i++)
		result.push_back(Dir->in_dir[i]->get_name());
	for(int i = 0; i < Dir->in_files.size(); i++) {
		if(Dir->in_files[i]->fd.get_owner() == u->get_username())
			result.push_back(Dir->in_files[i]->get_name());
		else
			result.push_back(Dir->in_files[i]->fd.get_related_name_to(u->get_username()));
	}
	sort(result.begin(), result.end());
	return result;
}

void dir::rm(string file_path, user* u) {
	if(!file_path.length()) {
		cout << "rm must have at least one argument" << endl;
		return;
	}
	vector<string> words = parse_path(file_path);
	string path = "";
	int idx = words.size() - 2;
	if(words[words.size() - 1].length())
		idx = words.size() - 1;
	for(int i = 0; i < idx; i++) {
		path += words[i];
		path += "/";
	}
	dir* Dir = this;
	if(words.size() != 1) {
		try {
			Dir = cd(path);
		} catch(cd_exception ex) {
			cout << "rm: cannot remove ‘" + file_path + "’: No such file or directory" << endl;
			return;
		}
	}
	for(int i = 0; i < Dir->in_files.size(); i++)
		if(Dir->in_files[i]->get_name() == words[idx]) {
			if(!Dir->get_perm()) {
				cout << "rm: cannot remove ‘" + file_path + "’: Permission denied" << endl;
				return;
			}
			u->decrease_used_space(Dir->in_files[i]->get_t_length());
			vector<shared_file_detail> sh_fd = Dir->in_files[i]->fd.get_shared_with();
			for(int j = 0; j < sh_fd.size(); j++) {
				dir* Dire = sh_fd[j].us->root->in_dir[0]->cd(Dir->in_files[i]->fd.get_owner());
				Dire->in_files.erase(Dire->in_files.begin() + sh_fd[j].shared_idx);
			}
			Dir->in_files[i]->revert(-1);
			delete Dir->in_files[i];
			Dir->in_files.erase(Dir->in_files.begin() + i);
			return;
		}
	cout << "rm: cannot remove ‘" + file_path + "’: No such file or directory" << endl;
}

void dir::rm_r(string dir_path, user* u) {
	if(!dir_path.length()) {
		cout << "rm must have at least one argument" << endl;
		return;
	}
	vector<string> words = parse_path(dir_path);
	string path = "";
	int idx = words.size() - 2;
	if(words[words.size() - 1].length())
		idx = words.size() - 1;
	for(int i = 0; i < idx; i++) {
		path += words[i];
		path += "/";
	}
	dir* Dir = this;
	if(words.size() != 1) {
		try {
			Dir = cd(path);
		} catch(cd_exception ex) {
			cout << "rm: cannot remove ‘" + dir_path + "’: No such file or directory" << endl;
			return;
		}
	}
	else if(words[0] == "..") {
		cout << "rm: cannot remove directory: ‘..’" << endl;
		return;
	}
	for(int i = 0; i < Dir->in_files.size(); i++) {
		if(Dir->in_files[i]->get_name() == words[idx]) {
			if(!Dir->get_perm()) {
				cout << "rm: cannot remove ‘" + dir_path + "’: Permission denied" << endl;
				return;
			}
			u->decrease_used_space(Dir->in_files[i]->get_t_length());
			vector<shared_file_detail> sh_fd = Dir->in_files[i]->fd.get_shared_with();
			for(int j = 0; j < sh_fd.size(); j++) {
				dir* Dire = sh_fd[j].us->root->in_dir[0]->cd(Dir->in_files[i]->fd.get_owner());
				Dire->in_files.erase(Dire->in_files.begin() + sh_fd[j].shared_idx);
			}
			Dir->in_files[i]->revert(-1);
			delete Dir->in_files[i];
			Dir->in_files.erase(Dir->in_files.begin() + i);
			return;
		}
	}
	for(int i = 0; i < Dir->in_dir.size(); i++) {
		if(Dir->in_dir[i]->get_name() == words[idx]) {
			for(int j = 0; j < Dir->in_dir[i]->in_files.size(); j++) {
				Dir->in_dir[i]->rm_r(Dir->in_dir[i]->in_files[j]->get_name(), u);
				j = -1;
			}
			for(int j = 0; j < Dir->in_dir[i]->in_dir.size(); j++) {
				Dir->in_dir[i]->rm_r("./" + Dir->in_dir[i]->in_dir[j]->get_name(), u);
				j = -1;
			}
			if(!Dir->in_dir[i]->get_perm()) {
				cout << "rm: cannot remove ‘" + dir_path + "’: Permission denied" << endl;
				return;
			}
			delete Dir->in_dir[i];
			Dir->in_dir.erase(Dir->in_dir.begin() + i);
			return;
		}
	}
	cout << "rm: cannot remove ‘" + dir_path + "’: No such file or directory" << endl;
}

void cut_paste(file* f1, file* f2, dir* Dir1, string un, string cmd) {
	f2->add_history(f2->get_name(), f1->get_length(), un, cmd);
	const char* c = (f2->get_storage_path()).c_str();
	ofstream sec(c);
	const char* C = (f1->get_storage_path()).c_str();
	ifstream fir(C);
	sec << fir.rdbuf();
	fir.close();
	sec.close();
	f1->revert(-1);
	vector<shared_file_detail> sh_w = f1->fd.get_shared_with();
	for(int i = 0; i < sh_w.size(); i++) {
		dir* Dir = sh_w[i].us->root->in_dir[0]->cd(f1->fd.get_owner());
		Dir->in_files.erase(Dir->in_files.begin() + sh_w[i].shared_idx);
	}
	Dir1->in_files.erase(find(Dir1->in_files.begin(), Dir1->in_files.end(), f1));
	delete f1;
	return;
}

void dir::mv(string first_path, string sec_path, user* owner) {
	vector<string> words1 = parse_path(first_path);
	string path1 = "";
	int idx = 0;
	for(int i = 0; i < words1.size() - 1; i++) {
		path1 += words1[i];
		path1 += "/";
	}
	dir* Dir1 = this;
	if(words1.size() != 1) {
		try {
			Dir1 = cd(path1);
		} catch(cd_exception ex) {
			cout << "mv: cannot stat ‘" + first_path + "’: No such file or directory" << endl;
			return;
		}
	}
	else if(words1[0] == "..") {
		cout << "mv: cannot move ‘..’ to ‘" + sec_path + "’: Device or resource busy" << endl;
		return;
	}
	vector<string> words2 = parse_path(sec_path);
	string path2 = "";
	for(int i = 0; i < words2.size() - 1; i++) {
		path2 += words2[i];
		path2 += "/";
	}
	dir* Dir2 = this;
	if(words2.size() != 1) {
		try {
			Dir2 = cd(path2);
		} catch(cd_exception ex) {
			cout << "mv: cannot move ‘" + first_path + "’ to ‘" + sec_path + "’: No such file or directory" << endl;
			return;
		}
	}
	else if(words2[0] == "..")
		Dir2 = cd("..");

	if(!Dir1->get_perm()) {
		cout << "mv: cannot move ‘" + first_path + "’: Permission denied" << endl;
		return;
	}
	if(!Dir2->get_perm()) {
		cout << "mv: cannot move ‘" + sec_path + "’: Permission denied" << endl;
		return;
	}
	file_or_dir first = Not_found, second = Not_found;
	file* First_File, *Second_File;
	dir* First_Dir;
	if(words1[words1.size() - 1].length())
		first = check_type(words1[words1.size() - 1], Dir1 , idx, owner);
	else
		first = check_type(words1[words1.size() - 2], Dir1 , idx, owner);
	if(first == Filee)
		First_File = Dir1->in_files[idx];
	else if(first == Dirr)
		First_Dir = Dir1->in_dir[idx];
	else {
		cout << "mv: cannot stat ‘" + first_path + "’: No such file or directory" << endl;
		return;
	}

	if(words2.size() == 1 && words2[0] == "..")
		second = Dirr;
	else {
		second = check_type(words2[words2.size() - 1], Dir2 , idx, owner);
		if(second == Filee)
			Second_File = Dir2->in_files[idx];
		else if(second == Dirr)
			Dir2 = Dir2->cd(Dir2->in_dir[idx]->get_name());
	}
	if(first == Filee) {
		if(second == Dirr) {
			Dir2->in_files.push_back(First_File);
			Dir1->in_files.erase(find(Dir1->in_files.begin(), Dir1->in_files.end(), First_File));
			return;
		}
		if(second == Filee) {
			if(Dir1 == Dir2 && First_File == Second_File) {
				cout << "mv: ‘" + first_path + "’ and ‘" + sec_path + "’ are the same file" << endl;
				return;
			}
			if(sec_path[sec_path.length() - 1] == '/') {
				cout << "mv: failed to access ‘" + sec_path + "’: Not a directory" << endl;
				return;
			}
			string cmd = "mv:" + First_File->get_name() + " -> " + Second_File->get_name();
			owner->decrease_used_space(First_File->get_t_length() - First_File->get_length());
			cut_paste(First_File, Second_File, Dir1, owner->get_username(), cmd);
			return;
		}
		if(second == Not_found) {
			string Name = First_File->get_name();
			if(words2[words2.size() - 1].length())
				Name = words2[words2.size() - 1];
			Dir2->in_files.push_back(new file(Name));
			string cmd = "mv:" + First_File->get_name() + " -> " + Dir2->in_files[Dir2->in_files.size() - 1]->get_name();
			owner->decrease_used_space(First_File->get_t_length() - First_File->get_length());
			cut_paste(First_File, Dir2->in_files[Dir2->in_files.size() - 1], Dir1, owner->get_username(), cmd);
			return;
		}
	}
	else if(first == Dirr) {
		if(second == Filee) {
			cout << "mv: cannot overwrite non-directory ‘" + Second_File->get_name() + "’ with directory ‘" + first_path + "’" << endl;
			return;
		}
		if(second == Dirr) {
			int i;
			i = Dir2->pwd().find(First_Dir->pwd());
			if(i != -1) {
				cout << "mv: cannot move ‘" + first_path + "’ to a subdirectory of itself, ‘" + sec_path + "’" << endl;
				return;
			}
			Dir2->in_dir.push_back(First_Dir);
			First_Dir->prev->in_dir.erase(find(First_Dir->prev->in_dir.begin(), First_Dir->prev->in_dir.end(), First_Dir));
			First_Dir->prev = Dir2;
			return;
		}
		if(second == Not_found) {
			First_Dir->prev->in_dir.erase(find(First_Dir->prev->in_dir.begin(), First_Dir->prev->in_dir.end(), First_Dir));
			First_Dir->name = words2[words2.size() - 1];
			Dir2->in_dir.push_back(First_Dir);
			First_Dir->prev = Dir2;
			return;
		}
	}
}

void dir::cp(string first_path, string sec_path, user* owner, users* US) {
	vector<string> words1 = parse_path(first_path);
	vector<string> words2 = parse_path(sec_path);
	string path1 = "";
	int count1 = words1.size() - 2;
	if(words1[words1.size() - 1].length())
		count1 = words1.size() - 1;
	for(int i = 0; i < count1; i++) {
		path1 += words1[i];
		path1 += "/";
	}
	int count2 = words2.size() - 2;
	if(words2[words2.size() - 1].length())
		count2 = words2.size() - 1;
	string path2 = "";
	for(int i = 0; i < count2; i++) {
		path2 += words2[i];
		path2 += "/";
	}
	dir* Dir1 = this;
	if(words1.size() != 1) {
		try {
			Dir1 = cd(path1);
		} catch(cd_exception ex) {
			cout << "cp: cannot stat ‘" + first_path + "’: No such file or directory" << endl;
			return;
		}
	}
	dir* Dir2 = this;
	if(words2.size() != 1) {
		try {
			Dir2 = cd(path2);
		} catch(cd_exception ex) {
			cout << "cp: cannot create regular file ‘" + sec_path + "’: No such file or directory" << endl;
			return;
		}
	}
	if(!words1[words1.size() - 1].length()) {
		cout << "cp: cannot stat ‘" + first_path + "’: No such file or directory" << endl;
		return;
	}
	int idx1 = 0;
	file_or_dir first = check_type(words1[words1.size() - 1], Dir1, idx1, owner);
	if(first == Dirr) {
		cout << "cp: omitting directory ‘" + first_path + "’" << endl;
		return;
	}
	if(first == Not_found) {
		cout << "cp: cannot stat ‘" + first_path + "’: No such file or directory" << endl;
		return;
	}
	int idx2 = 0;
	file_or_dir second;
	if(words2[words2.size() - 1].length())
		second = check_type(words2[words2.size() - 1], Dir2, idx2, owner);
	else
		second = check_type(words2[words2.size() - 2], Dir2, idx2, owner);
	if(second == Not_found && words2[words2.size() - 1].length()) {
		if(!Dir2->get_perm()) {
			cout << "cp: cannot copy ‘" + sec_path + "’: Permission denied" << endl;
			return;
		}
		int alloced = owner->get_allocated_storage_amount();
		double used = owner->get_used_space();
		if((used + (double)Dir1->in_files[idx1]->get_t_length() / 1000000) > alloced) {
			cout << "cp: There is not enough storage space" << endl;
			return;
		}
		string cmd = "cp: " + Dir1->in_files[idx1]->get_name() + " -> " + Dir2->pwd() + words2[words2.size() - 1];
		Dir2->in_files.push_back(new file(words2[words2.size() - 1], owner->get_username(), Dir1->in_files[idx1]->get_length(), owner->get_username(), cmd));
		owner->increase_used_space(Dir1->in_files[idx1]->get_length());
		const char* c = (Dir2->in_files[Dir2->in_files.size() -1]->get_storage_path()).c_str();
		ofstream sec(c);
		const char* C = (Dir1->in_files[idx1]->get_storage_path()).c_str();
		ifstream fir(C);
		sec << fir.rdbuf();
		fir.close();
		sec.close();
		return;
	}
	else if(second == Not_found && !words2[words2.size() - 1].length()) {
		cout << "cp: cannot create regular file ‘" + sec_path + "’: Not a directory" << endl;
		return;
	}
	else if(second == Dirr) {
		string Name = words2[words2.size() - 2];
		if(words2[words2.size() - 1].length())
			Name = words2[words2.size() - 1];
		Dir2 = Dir2->in_dir[idx2];
		if(!Dir2->get_perm()) {
			cout << "cp: cannot copy ‘" + sec_path + "’: Permission denied" << endl;
			return;
		}
		if(Dir1 == Dir2) {
			cout << "cp: ‘" + first_path + "’ and ‘" + sec_path + "’ are the same file" << endl;
			return;
		}
		int alloced = owner->get_allocated_storage_amount();
		double used = owner->get_used_space();
		if((used + (double)Dir1->in_files[idx1]->get_length() / 1000000) > alloced) {
			cout << "cp: There is not enough storage space" << endl;
			return;
		}
		string cmd = "cp: " + Dir1->in_files[idx1]->get_name() + " -> " + Dir2->pwd();
		Dir2->in_files.push_back(new file(Dir1->in_files[idx1]->get_name(), owner->get_username(), Dir1->in_files[idx1]->get_length(), owner->get_username(), cmd));
		owner->increase_used_space(Dir1->in_files[idx1]->get_length());
		const char* c = (Dir2->in_files[Dir2->in_files.size() -1]->get_storage_path()).c_str();
		ofstream sec(c);
		const char* C = (Dir1->in_files[idx1]->get_storage_path()).c_str();
		ifstream fir(C);
		sec << fir.rdbuf();
		fir.close();
		sec.close();
		return;
	}
	else if(second == Filee) {
		if(Dir2->in_files[idx2]->fd.get_owner() != owner->get_username()) {
			if(!Dir2->in_files[idx2]->fd.get_write_perm_of(owner->get_username())) {
				cout << "cp: Permission denied" << endl;
				return;
			}
		}
		if(Dir1 == Dir2 && Dir2->in_files[idx2]->get_name() == Dir1->in_files[idx1]->get_name()) {
			cout << "cp: ‘" + first_path + "’ and ‘" + sec_path + "’ are the same file" << endl;
			return;
		}
		if(Dir2->in_files[idx2]->fd.get_owner() == owner->get_username()) {
			int alloced = owner->get_allocated_storage_amount();
			double used = owner->get_used_space();
			if((used + (double)Dir1->in_files[idx1]->get_length() / 1000000) > alloced) {
				cout << "cp: There is not enough storage space" << endl;
				return;
			}
			owner->increase_used_space(Dir1->in_files[idx1]->get_length());
		}
		else {
			int alloced = US->get_user(Dir2->in_files[idx2]->fd.get_owner())->get_allocated_storage_amount();
			int used = US->get_user(Dir2->in_files[idx2]->fd.get_owner())->get_used_space();
			if((used + (double)Dir1->in_files[idx1]->get_length() / 1000000) > alloced) {
				cout << "cp: There is not enough storage space" << endl;
				return;
			}
			US->get_user(Dir2->in_files[idx2]->fd.get_owner())->increase_used_space(Dir1->in_files[idx1]->get_length());
		}
		string cmd = "cp: " + Dir1->in_files[idx1]->get_name() + " -> " + Dir2->in_files[idx2]->get_name();
		Dir2->in_files[idx2]->add_history(Dir2->in_files[idx2]->get_name(), Dir1->in_files[idx1]->get_length(), owner->get_username(), cmd);
		const char* c = (Dir2->in_files[idx2]->get_storage_path()).c_str();
		const char* C = (Dir1->in_files[idx1]->get_storage_path()).c_str();
		ofstream sec(c);
		ifstream fir(C);
		sec << fir.rdbuf();
		fir.close();
		sec.close();
		return;
	}
}

void dir::cat(string in_path, user* u) {
	vector<string> words = parse_path(in_path);
	if(!words[words.size() - 1].length())
		words.erase(words.end() - 1);
	if(in_path.length() && !words.size()) {
		cout << "cat: " + in_path + ": Is a directory" << endl;
		return;
	}
	if(!words.size()) {
		cout << "cat must have at least one argument" << endl;
		return;
	}
	string path = "";
	int idx = 0;
	for(int i = 0; i < words.size() - 1; i++) {
		path += words[i];
		path += "/";
	}
	dir* Dir = this;
	if(words.size() != 1) {
		try {
			Dir = cd(path);
		} catch(cd_exception ex) {
			cout << "cat: " + in_path + ": No such file or directory" << endl;
			return;
		}
	}
	else if(words[0] == "..") {
		cout << "cat: " + in_path + ": Is a directory" << endl;
		return;
	}
	file_or_dir type;
	type = check_type(words[words.size() - 1] , Dir, idx, u);
	if(type == Dirr) {
		cout << "cat: " + in_path + ": Is a directory" << endl;
		return;
	}
	else if(type == Not_found) {
		cout << "cat: " + in_path + ": No such file or directory" << endl;
		return;
	}
	else {
		const char* c = (Dir->in_files[idx]->get_storage_path()).c_str();
		ifstream F(c);
		if(F.is_open()) {
			cout << F.rdbuf();
			F.close();
		}
	}
	cout << endl;
}

void dir::put(string src, string des, user* owner, users* US) {
	vector<string> words1 = parse_path(src);
	vector<string> words2 = parse_path(des);
	if(src.length() && !words1[words1.size() - 1].length()) {
		cout << "put: ‘" + src + "’: Is not a file" << endl;
		return;
	}
	string path = "";
	if(des.length() && !words2[words2.size() - 1].length())
		if(words2.size() == 1)
			path = des;
	int idx = 0;
	for(int i = 0; i < words2.size() - 1; i++) {
		path += words2[i];
		path += "/";
	}
	dir* Dir = this;
	if(words2.size() != 1) {
		try {
			Dir = cd(path);
		} catch(cd_exception ex) {
			cout << "put: ‘" + des + "’: No such file or directory" << endl;
			return;
		}
	}
	else if(words2[0] == ".." || words2[0] == ".") {
		const char* c = src.c_str();
		ifstream in_f(c);
		if(!in_f.is_open()) {
			cout << "put :‘" + src + "’: No such file or directory" << endl;
			return;
		}
		struct stat s;
		if(!stat(c, &s))
    		if( s.st_mode & S_IFDIR ) {
    		    cout << "put: ‘" + src + "’: Is a directory" << endl;
    		    in_f.close();
    		    return;
    		}
		Dir = Dir->cd(words2[0]);
		file_or_dir type = check_type(words1[words1.size() - 1], Dir, idx, owner);
		if(type == Filee) {
			if(Dir->in_files[idx]->fd.get_owner() != owner->get_username()) {
				if(!Dir->in_files[idx]->fd.get_write_perm_of(owner->get_username())) {
					cout << "put: Permission denied" << endl;
					in_f.close();
					return;
				}
			}
			in_f.seekg (0, in_f.end);
    		long int l = in_f.tellg();
    		in_f.seekg (0, in_f.beg);
			if(Dir->in_files[idx]->fd.get_owner() == owner->get_username()) {
				int alloced = owner->get_allocated_storage_amount();
				double used = owner->get_used_space();
				if((used + (double)l / 1000000) > alloced) {
					cout << "put: There is not enough storage space" << endl;
					return;
				}
				owner->increase_used_space(l);
			}
			else {
				int alloced = US->get_user(Dir->in_files[idx]->fd.get_owner())->get_allocated_storage_amount();
				int used = US->get_user(Dir->in_files[idx]->fd.get_owner())->get_used_space();
				if((used + (double)l / 1000000) > alloced) {
					cout << "put: There is not enough storage space" << endl;
					return;
				}
				US->get_user(Dir->in_files[idx]->fd.get_owner())->increase_used_space(l);
			}
			string cmd = "put: " + src + " -> " + Dir->in_files[idx]->get_name();
			Dir->in_files[idx]->add_history(Dir->in_files[idx]->get_name(), l, owner->get_username(), cmd);
			const char* C = (Dir->in_files[idx]->get_storage_path()).c_str();
			ofstream out_f(C);
			out_f << in_f.rdbuf();
			in_f.close();
			out_f.close();
			return;
		}
		else if(type == Dirr) {
			cout << "put: A directory with this name exists" << endl;
			in_f.close();
			return;
		}
		else {
			if(!Dir->get_perm()) {
				cout << "put: cannot upload to ‘" + des + "’: Permission denied" << endl;
				in_f.close();
				return;
			}
			in_f.seekg (0, in_f.end);
    		long int l = in_f.tellg();
    		in_f.seekg (0, in_f.beg);
			int alloced = owner->get_allocated_storage_amount();
			double used = owner->get_used_space();
			if((used + (double)l / 1000000) > alloced) {
				cout << "put: There is not enough storage space" << endl;
				in_f.close();
				return;
			}
			string cmd = "put: " + src + " -> " + des;
			Dir->in_files.push_back(new file(words1[words1.size() - 1], owner->get_username(), l, owner->get_username(), cmd));
			owner->increase_used_space(l);
			const char* C = (Dir->in_files[Dir->in_files.size() - 1]->get_storage_path()).c_str();
			ofstream out_f(C);
			out_f << in_f.rdbuf();
			in_f.close();
			out_f.close();
			return;
		}
	}
	const char* c = src.c_str();
	ifstream in_f(c);
	if(!in_f.is_open()) {
		cout << "put :" + src + ": No such file or directory" << endl;
		return;
	}
	struct stat s;
	if(!stat(c, &s))
    	if( s.st_mode & S_IFDIR ) {
    	    cout << "put: " + src + ": Is a directory" << endl;
    	    in_f.close();
    	    return;
    	}
    string N = words1[words1.size() - 1];
    if(words2[words2.size() - 1].length() && words2[words2.size() - 1] != "." && words2[words2.size() - 1] != "..")
    	N = words2[words2.size() - 1];
    if(words2[words2.size() - 1] == "..")
    	Dir = Dir->cd("..");
	file_or_dir type = check_type(N, Dir, idx, owner);
	if(type == Filee) {
		if(Dir->in_files[idx]->fd.get_owner() != owner->get_username()) {
			if(!Dir->in_files[idx]->fd.get_write_perm_of(owner->get_username())) {
				cout << "put: Permission denied" << endl;
				in_f.close();
				return;
			}
		}
		in_f.seekg (0, in_f.end);
		long int l = in_f.tellg();
		in_f.seekg (0, in_f.beg);
		if(Dir->in_files[idx]->fd.get_owner() != owner->get_username()) {
			if(!Dir->in_files[idx]->fd.get_write_perm_of(owner->get_username())) {
				cout << "put: Permission denied" << endl;
				in_f.close();
				return;
			}
		}
		if(Dir->in_files[idx]->fd.get_owner() == owner->get_username()) {
			int alloced = owner->get_allocated_storage_amount();
			double used = owner->get_used_space();
			if((used + (double)l / 1000000) > alloced) {
				cout << "put: There is not enough storage space" << endl;
				return;
			}
			owner->increase_used_space(l);
		}
		else {
			int alloced = US->get_user(Dir->in_files[idx]->fd.get_owner())->get_allocated_storage_amount();
			int used = US->get_user(Dir->in_files[idx]->fd.get_owner())->get_used_space();
			if((used + (double)l / 1000000) > alloced) {
				cout << "put: There is not enough storage space" << endl;
				return;
			}
			US->get_user(Dir->in_files[idx]->fd.get_owner())->increase_used_space(l);
		}
		string cmd = "put: " + src + " -> " + Dir->in_files[idx]->get_name();
		Dir->in_files[idx]->add_history(Dir->in_files[idx]->get_name(), l, owner->get_username(), cmd);
		const char* C = (Dir->in_files[idx]->get_storage_path()).c_str();
		ofstream out_f(C);
		out_f << in_f.rdbuf();
		in_f.close();
		out_f.close();
		return;
	}
	else if(type == Not_found) {
		if(!Dir->get_perm()) {
			cout << "put: cannot upload to ‘" + des + "’: Permission denied" << endl;
			in_f.close();
			return;
		}
		in_f.seekg (0, in_f.end);
    	long int l = in_f.tellg();
    	in_f.seekg (0, in_f.beg);
		int alloced = owner->get_allocated_storage_amount();
		double used = owner->get_used_space();
		if((used + (double)l / 1000000) > alloced) {
			cout << "put: There is not enough storage space" << endl;
			in_f.close();
			return;
		}
		string cmd = "put: " + src + " -> " + des;
		Dir->in_files.push_back(new file(N, owner->get_username(), l, owner->get_username(), cmd));
		owner->increase_used_space(l);
		const char* C = (Dir->in_files[Dir->in_files.size() - 1]->get_storage_path()).c_str();
		ofstream out_f(C);
		out_f << in_f.rdbuf();
		in_f.close();
		out_f.close();
		return;
	}
	else {
		Dir = Dir->in_dir[idx];
		if(!Dir->get_perm()) {
			cout << "put: cannot upload to ‘" + des + "’: Permission denied" << endl;
			in_f.close();
			return;
		}
		in_f.seekg (0, in_f.end);
    	long int l = in_f.tellg();
    	in_f.seekg (0, in_f.beg);
		int alloced = owner->get_allocated_storage_amount();
		double used = owner->get_used_space();
		if((used + (double)l / 1000000) > alloced) {
			cout << "put: There is not enough storage space" << endl;
			in_f.close();
			return;
		}
		string cmd = "put: " + src + " -> " + des;
		Dir->in_files.push_back(new file(words1[words1.size() - 1], owner->get_username(), l, owner->get_username(), cmd));
		owner->increase_used_space(l);
		const char* C = (Dir->in_files[Dir->in_files.size() - 1]->get_storage_path()).c_str();
		ofstream out_f(C);
		out_f << in_f.rdbuf();
		in_f.close();
		out_f.close();
		return;
	}
}

void dir::get(string src, string des, user* u) {
	vector<string> words1 = parse_path(src);
	vector<string> words2 = parse_path(des);
	if(src.length() && !words1[words1.size() - 1].length()) {
		cout << "get: " + src + ": Is not a file" << endl;
		return;
	}
	string path = "";
	int idx = 0;
	for(int i = 0; i < words1.size() - 1; i++) {
		path += words1[i];
		path += "/";
	}
	dir* Dir = this;
	if(words1.size() != 1) {
		try {
			Dir = cd(path);
		} catch(cd_exception ex) {
			cout << "get: " + src + ": No such file or directory" << endl;
			return;
		}
	}
	else if(words1[0] == ".." || words1[0] == ".") {
		cout << "get: " + src + ": Is a directory" << endl;
		return;
	}
    file_or_dir type = check_type(words1[words1.size() - 1], Dir, idx, u);
    if(type == Filee) {
		const char* c = des.c_str();
		ofstream out_f(c);
		struct stat s;
		if(!stat(c, &s)) {
			if( s.st_mode & S_IFDIR ) {
				file_or_dir type = check_type(words1[words1.size() - 1], Dir, idx, u);
				const char* C = (Dir->in_files[idx]->get_storage_path()).c_str();
				if(des[des.length() - 1] != '/')
					des += "/";
				string out_name = des + Dir->in_files[idx]->get_name();
				c = out_name.c_str();
				out_f.close();
				out_f.open(c);
				if(!out_f.is_open()) {
					cout << "get :" + des + ": No such file or directory" << endl;
					return;
				}
				ifstream in_f(C);
				out_f << in_f.rdbuf();
				in_f.close();
				out_f.close();
				return;
			}
		if(!out_f.is_open()) {
			cout << "get :" + des + ": No such file or directory" << endl;
			return;
		}
    	const char* C = (Dir->in_files[idx]->get_storage_path()).c_str();
    	ifstream in_f(C);
    	out_f << in_f.rdbuf();
    	in_f.close();
    	out_f.close();
    	return;
    	}
    	if(!out_f.is_open()) {
			cout << "get :" + des + ": No such file or directory" << endl;
			return;
		}
    }
    if(type == Dirr) {
    	cout << "get: " + src + ": Is a directory" << endl;
    	return;
    }
    if(type == Not_found) {
    	cout << "get: " + src + ": No such file or directory" << endl;
    	return;
    }
}

dir* dir::cd(string path) {
	dir* Dir = this;
	int idx = 0;
	if(!path.length()) {
		while(Dir->prev != NULL) {
			Dir = Dir->prev;
		}
		return Dir;
	}
	vector<string> words = parse_path(path);
	if(!words[words.size() - 1].length())
		words.erase(words.end() - 1);
	if(path[0] == '/') {
		while(Dir->prev != NULL) {
			Dir = Dir->prev;
		}
		for(int j = 0; j < words.size(); j++) {
			if(words[j] == "..") {
				if(Dir->prev != NULL)
					Dir = Dir->prev;
				continue;
			}
			if(words[j] == ".")
				continue;
			if(check_type(words[j] , Dir, idx, NULL) == Dirr)
					Dir = Dir->in_dir[idx];
			else
				throw cd_exception("cd: " + path + " No such file or directory");
		}
	}

	else {
		for(int j = 0; j < words.size(); j++) {
			if(words[j] == "..") {
				if(Dir->prev != NULL)
					Dir = Dir->prev;
				continue;
			}
			if(words[j] == ".")
				continue;
			if(check_type(words[j] , Dir, idx, NULL) == Dirr)
					Dir = Dir->in_dir[idx];
			else
				throw cd_exception("cd: " + path + " No such file or directory");
		}
	}
	return Dir;
}