#include "user.h"
#include "sign_in_error.h"

vector<string> parse_path(string path);
file_or_dir check_type(string x, dir* Dir, int& idx, user* u);

users::users(const users& u) {
	count = u.count;
	for(int i = 0; i < count; i++)
		Users.push_back(new user(*(u.Users[i])));
}

users::~users() {
	if(count)
		for(int i = 0; i < count; i++) {
			fremove(Users[i]->get_username());
			i--;
		}
}

user* users::get_user(string username) {
	for(int i = 0; i < count; i++)
		if(Users[i]->get_username() == username)
			return Users[i];
}

void admin::destroy_user(string username) {
	if(!US->does_exist(username)) {
		cout << "user: ‘" + username + "’ doesn't exist" << endl;
		return;
	}
	user* u = US->get_user(username);
	vector<request*> Requests = u->get_requests();
	for(int i = 0; i < Requests.size(); i++)
		for(int j = 0; j < requests.size(); j++)
			if(requests[j] == Requests[i]) {
				requests.erase(requests.begin() + j);
				j--;
			}
	US->remove(username);
}

void user::add_request(request* Request) {
	requests.push_back(Request);
}

void request::accept() {
	status = "accepted";
}

void request::reject() {
	status = "rejected";
}

void admin::accept_request(int idx) {
	if(idx > requests.size() || idx < 1) {
		cout << "Invalid request number." << endl;
		return;
	}
	if(requests[idx - 1]->get_status() != "pending") {
		cout << "accept: Managed before" << endl;
		return;
	}
	requests[idx - 1]->accept();
	if(requests[idx - 1]->get_user() != "admin")
		US->get_user(requests[idx - 1]->get_user())->allocated_storage += requests[idx - 1]->get_amount();
	else
		allocated_storage += requests[idx - 1]->get_amount();
	if(requests[idx - 1]->get_user() != "admin")
		requests.erase(requests.begin() + idx - 1);
}

void admin::reject_request(int idx) {
	if(idx > requests.size() || idx < 1) {
		cout << "Invalid request number." << endl;
		return;
	}
	if(requests[idx - 1]->get_status() != "pending") {
		cout << "accept: Managed before" << endl;
		return;
	}
	requests[idx - 1]->reject();
	if(requests[idx - 1]->get_user() != "admin")
		requests.erase(requests.begin() + idx - 1);
}


vector<user_inf> admin::userlist() {
	vector<user_inf> ui;
	for(int i = 0; i < US->size(); i++) {
		ui.push_back(user_inf((*US)[i].get_username(), (*US)[i].get_allocated_storage_amount(), (*US)[i].get_used_space()));
	}
	return ui;
}

void users::remove(string username) {
	if(username == "admin") {
		cout << "Cannot delete admin.\n" << "Permission denied." << endl;
		return;
	}
	fremove(username);
}

void users::fremove(string username) {
	if(!does_exist(username))
		return;
	for(int i = 0; i < count; i++)
		if(Users[i]->get_username() == username) {
			while(Users[i]->root->in_dir.size() != 1)
				if(Users[i]->root->in_dir[1]->get_name() != "shared")
					Users[i]->root->rm_r(Users[i]->root->in_dir[1]->get_name(), Users[i]);
			for(int k = 0; k < Users[i]->root->in_dir[0]->in_dir.size(); k++) {
				for(int s = 0; s < Users[i]->root->in_dir[0]->in_dir[k]->in_files.size(); s++) {
					int p = Users[i]->root->in_dir[0]->in_dir[k]->in_files[s]->fd.get_shfd_idx(Users[i]->get_username());
					Users[i]->root->in_dir[0]->in_dir[k]->in_files[s]->fd.delete_shfd(p);
				}
				delete Users[i]->root->in_dir[0]->in_dir[k];
			}
			delete Users[i]->root->in_dir[0];
			Users[i]->root->in_dir.pop_back();
			for(int t = 0; t < Users[i]->root->in_files.size(); t++) {
				Users[i]->root->rm_r(Users[i]->root->in_files[t]->get_name(), Users[i]);
				t--;
			}
			delete Users[i]->root;
			vector<request*> requests = Users[i]->get_requests();
			for(int m = 0; m < requests.size(); m++)
				delete requests[m];
		if(Users[i]->get_username() != "admin")
			delete Users[i];
		Users.erase(Users.begin() + i);
		}
	count--;
}

users& users::operator=(const users& u) {
	if(this == &u)
		return *this;
	if(count) {
		for(int i = 0; i < Users.size(); i++)
			delete Users[i];
		Users.erase(Users.begin(), Users.end());
	}
	count = u.count;
	for(int i = 0; i < count; i++)
		Users.push_back(new user(*(u.Users[i])));
	return *this;
}

string users::signed_in_users_name() {
	for(int i = 0; i < count; i++)
		if(is_signed_in(Users[i]->get_username()))
			return Users[i]->get_username();
	return "";
}

user* users::signed_in_user() {
	for(int i = 0; i < count; i++)
		if(is_signed_in(Users[i]->get_username()))
			return Users[i];
}

void users::add(string Username, string Password) {
	count++;
	if(count - 1) {
		for(int	i = 0; i < count - 1; i++)
			Users[i]->sign_out();
	}
	Users.push_back(new user(Username, Password));
}

void users::add(user u) {
	count++;
	if(count - 1) {
		for(int	i = 0; i < count - 1; i++)
			Users[i]->sign_out();
	}
	Users.push_back(&u);
}

bool users::does_exist(string un) {
	for(int i = 0; i < count; i++)
		if(Users[i]->get_username() == un)
			return true;
	return false;
}

void users::sign_out_everyone() {
	for(int i = 0; i < count; i++)
		if(Users[i]->is_signed_in())
			Users[i]->sign_out();
}

void users::sign_in_user(string un, string pw) {
	if(!does_exist(un))
		throw sign_in_error("An error occurred while signing in.", "Cannot find any account related to this username.");
	for(int i = 0; i < count; i++)
		if(Users[i]->get_username() == un) {
			if(Users[i]->is_password_valid(pw))
				Users[i]->sign_in();
			else
				throw sign_in_error("Wrong password.", "Please try again.");
		}
}


bool users::is_signed_in(string un) {
	for(int i = 0; i < count; i++)
		if(Users[i]->get_username() == un)
			return Users[i]->is_signed_in();
}

user users::operator[](int i) {
	return *Users[i];
}

user* users::operator()(int i) {
	return Users[i];
}

bool users_medium::is_user_perm_valid(user_perm UP) {
	for(int i = 0; i < US->size(); i++)
		if(UP.username == (*US)[i].get_username())
			return true;
	return false;
}

int user::how_many_shared(string name, dir* Dir) {
	int count = 0;
	for(int i = 0; i < Dir->in_files.size(); i++) {
		if(Dir->in_files[i]->get_name() == name)
			count++;
	}
	return count;
}

void users_medium::share(file* F, string username, bool write_perm) {
	if(F->fd.is_shared_with(username)) {
		int index = F->fd.get_shfd_idx(username);
		F->fd.set_perm(index, write_perm);
		return;
	}
	string name = F->get_name();
	for(int i = 0; i < US->size(); i++) {
		if((*US)(i)->get_username() == username) {
			dir* Dir = (*US)(i)->root->in_dir[0];
			for(int j = 0; j < Dir->in_dir.size(); i++) {
				if(Dir->in_dir[j]->get_name() == F->fd.get_owner()) {
					Dir = Dir->in_dir[j];
					char c = ((*US)(i)->how_many_shared(name, Dir)) + 48;
					F->fd.add_shfd(shared_file_detail((*US)(i), Dir->in_files.size(), write_perm, name + c));
					Dir->in_files.push_back(F);
					return;
				}
			}
			Dir->in_dir.push_back(new dir(F->fd.get_owner(), false, Dir));
			share(F, username, write_perm);
		}
	}
}

void user::rm_r(string path) {
	dir* Dir = curr;
	try {
		Dir = curr->cd(path);
	} catch(cd_exception ex) {
		curr->rm_r(path, this);
		return;
	}
	dir* Dir2 = curr;
	while(Dir2 != Dir && Dir2 != root)
		Dir2 = Dir2->prev;
	if(Dir2 == Dir) {
		Dir = Dir->prev;
		curr->rm_r(path, this);
		if(Dir != NULL)
			curr = Dir;
	}
	else
		curr->rm_r(path, this);
}

void user::show_history(string path) {
	vector<string> words = parse_path(path);
	if(!words[words.size() - 1].length()) {
		cout << "history: ‘" + path + "’: No such file or directory" << endl;
		return;
	}
	string Path= "";
	dir* Dir = curr;
	for(int i = 0; i < words.size() - 1; i++) {
		Path += words[i];
		Path += "/";
	}
	if(words.size() > 1) {
		try {
			Dir = Dir->cd(Path);
		} catch(cd_exception ex) {
			cout << "history: ‘" + path + "’: No such file or directory" << endl;
			return;
		}
	}
	int idx;
	file_or_dir type = check_type(words[words.size() - 1], Dir, idx, this);
	if(type != Filee) {
		if(type == Dirr)
			cout << "history: ‘" + path + "’: Is a directory" << endl;
		else
			cout << "history: ‘" + path + "’: No such file or directory" << endl;
		return;
	}
	vector<history> change_log = Dir->in_files[idx]->get_change_log();
	for(int i = 0; i < change_log.size(); i++) {
		cout << i << "." << "\n\t";
		cout << "Name: " << change_log[i].name << "\n\t";
		cout << "Modified by: " << change_log[i].username << "\n\t";
		cout << "Modified with: " << change_log[i].cmd << endl;

	}

}

void user::revert(string path, string index) {
	int temp = 0;
	for(int i = 0; i < index.length(); i++) {
		if(index[i] > '9' || index[i] < '0') {
			cout << "revert: ‘" + index + "’: Isn't an integer" << endl;
			return;
		}
		temp *= 10;
		temp += index[i] - 48;
	}
	vector<string> words = parse_path(path);
	if(!words[words.size() - 1].length()) {
		cout << "revert: ‘" + path + "’: No such file or directory" << endl;
		return;
	}
	string Path= "";
	dir* Dir = curr;
	for(int i = 0; i < words.size() - 1; i++) {
		Path += words[i];
		Path += "/";
	}
	if(words.size() > 1) {
		try {
			Dir = Dir->cd(Path);
		} catch(cd_exception ex) {
			cout << "revert: ‘" + path + "’: No such file or directory" << endl;
			return;
		}
	}
	int idx;
	file_or_dir type = check_type(words[words.size() - 1], Dir, idx, this);
	if(type != Filee) {
		if(type == Dirr)
			cout << "revert: ‘" + path + "’: Is a directory" << endl;
		else
			cout << "revert: ‘" + path + "’: No such file or directory" << endl;
		return;
	}
	if(temp < 0 || temp > (Dir->in_files[idx]->get_change_log()).size() - 2) {
		cout << "revert: ‘" + index + "’: Is out of range" << endl;
		return;
	}
	if(Dir->in_files[idx]->fd.get_owner() != username) {
		cout << "revert: Permission denied" << endl;
		return;
	}
	Dir->in_files[idx]->revert(temp, this);
}

void user::sharelist(string path, dir* Dir1) {
	vector<string> words = parse_path(path);
	dir* Dir = Dir1;
	string Path = "";
	if(!words[words.size() - 1].length() || words.size() > 1 || words[0] == "." || words[0] == "..") {
		for(int i = 0; i < words.size() - 1; i++) {
			Path += words[i];
			Path += "/";
		}
		if(words.size() == 1 && (words[0] == "." || words[0] == ".."))
			path += words[0];
		try {
			Dir = Dir->cd(Path);
		} catch(cd_exception ex) {
			cout << "sharelist: ‘" + path + "’: No such file or directory" << endl;
			return;
		}
	}
	else {
		for(int i = 0; i < Dir->in_files.size(); i++)
			if(Dir->in_files[i]->get_name() == words[0]) {
				vector<shared_file_detail> shfd = Dir->in_files[i]->fd.get_shared_with();
				for(int j = 0; j < shfd.size(); j++)
					cout << "In path: " << Dir->pwd() << "\nFile name: " << Dir->in_files[i]->get_name() << " : Shared with: " << shfd[i].us->get_username() << " : " <<  (shfd[i].write_perm ? 'w' : 'r') << endl;
				return;
			}
		for(int i = 0; i < Dir->in_dir.size(); i++) {
			if(Dir->in_dir[i]->get_name() == words[0]) {
				for(int j = 0; j < Dir->in_dir[i]->in_files.size(); j++)
					sharelist(Dir->in_dir[i]->in_files[j]->get_name(), Dir->in_dir[i]);
				for(int j = 0; j < Dir->in_dir[i]->in_dir.size(); j++)
					sharelist(Dir->in_dir[i]->in_dir[j]->get_name(), Dir->in_dir[i]);
				return;
			}
		}
		cout << "sharelist: ‘" + path + "’: No such file or directory" << endl;
		return;
	}
	for(int i = 0; i < Dir->in_files.size(); i++)
		if(Dir->in_files[i]->get_name() == words[words.size() - 1]) {
			vector<shared_file_detail> shfd = Dir->in_files[i]->fd.get_shared_with();
			for(int j = 0; j < shfd.size(); j++)
				cout << shfd[i].us->get_username() << ":" <<  (shfd[i].write_perm ? 'w' : 'r') << endl;
			return;
		}
	for(int i = 0; i < Dir->in_dir.size(); i++) {
		if(Dir->in_dir[i]->get_name() == words[words.size() - 1]) {
			for(int j = 0; j < Dir->in_dir[i]->in_files.size(); j++)
				sharelist(Dir->in_dir[i]->in_files[j]->get_name(), Dir->in_dir[i]);
			for(int j = 0; j < Dir->in_dir[i]->in_dir.size(); j++)
				sharelist(Dir->in_dir[i]->in_dir[j]->get_name(), Dir->in_dir[i]);
			return;
		}
	}
	cout << "sharelist: ‘" + path + "’: No such file or directory" << endl;
}

void user::unshare(string path, vector<string> US, users_medium* um, dir* Dir1) {
	for(int i = 0; i < US.size(); i++)
		if(!um->does_exist(US[i])) {
			cout << "unshare: ‘" + US[i] + "’: doesn't exist" << endl;
			return;
		}
	vector<string> words = parse_path(path);
	dir* Dir = Dir1;
	string Path = "";
	if(!words[words.size() - 1].length() && words.size() == 1) {
		cout << "unshare: ‘" + path + "’: you can't unshare root. Permission denied" << endl;
		return;
	}
	if(!words[words.size() - 1].length() || words.size() > 1 || words[0] == "." || words[0] == "..") {
		for(int i = 0; i < words.size() - 1; i++) {
			Path += words[i];
			Path += "/";
		}
		if(words.size() == 1 && (words[0] == "." || words[0] == ".."))
			path += words[0];
		try {
			Dir = Dir->cd(Path);
		} catch(cd_exception ex) {
			cout << "unshare: ‘" + path + "’: No such file or directory" << endl;
			return;
		}
	}
	else {
		for(int i = 0; i < Dir->in_files.size(); i++)
			if(Dir->in_files[i]->get_name() == words[0]) {
				for(int j = 0; j < US.size(); j++)
					if(Dir->in_files[i]->fd.is_shared_with(US[j]))
						um->unshare(Dir->in_files[i], US[j]);
				return;
			}
		for(int i = 0; i < Dir->in_dir.size(); i++) {
			if(Dir->in_dir[i]->get_name() == words[0]) {
				for(int j = 0; j < Dir->in_dir[i]->in_files.size(); j++)
					unshare(Dir->in_dir[i]->in_files[j]->get_name(), US, um, Dir->in_dir[i]);
				for(int j = 0; j < Dir->in_dir[i]->in_dir.size(); j++)
					unshare(Dir->in_dir[i]->in_dir[j]->get_name(), US, um, Dir->in_dir[i]);
				return;
			}
		}
		cout << "unshare: ‘" + path + "’: No such file or directory" << endl;
		return;
	}
	for(int i = 0; i < Dir->in_files.size(); i++)
		if(Dir->in_files[i]->get_name() == words[words.size() - 1]) {
			for(int j = 0; j < US.size(); j++)
				if(Dir->in_files[i]->fd.is_shared_with(US[j]))
						um->unshare(Dir->in_files[i], US[j]);
			return;
		}
	for(int i = 0; i < Dir->in_dir.size(); i++) {
		if(Dir->in_dir[i]->get_name() == words[words.size() - 1]) {
			for(int j = 0; j < Dir->in_dir[i]->in_files.size(); j++)
				unshare(Dir->in_dir[i]->in_files[j]->get_name(), US, um, Dir->in_dir[i]);
			for(int j = 0; j < Dir->in_dir[i]->in_dir.size(); j++)
				unshare(Dir->in_dir[i]->in_dir[j]->get_name(), US, um, Dir->in_dir[i]);
			return;
		}
	}
	cout << "unshare: ‘" + path + "’: No such file or directory" << endl;
}

void users_medium::unshare(file* F, string username) {
	vector<shared_file_detail> shfd = F->fd.get_shared_with();
	for(int i = 0; i < shfd.size(); i++)
		if(shfd[i].us->get_username() == username) {
			dir* Dir = shfd[i].us->root->in_dir[0]->cd(F->fd.get_owner());
			for (int j = 0; j < Dir->in_files.size(); j++)
				if (Dir->in_files[j] == F) {
					Dir->in_files.erase(Dir->in_files.begin() + j);
					break;
				}
			F->fd.delete_shfd(i);
		}
}

void user::share(string path, vector<user_perm> User_perms, users_medium* um, dir* Dir1) {
	vector<string> words = parse_path(path);
	dir* Dir = Dir1;
	string Path = "";
	for(int i = 0; i < User_perms.size(); i++)
		if(!um->is_user_perm_valid(User_perms[i])) {
			cout << "share: ‘" + User_perms[i].username + "’: user doesn't exist" << endl;
			return;
		}
	if(!words[words.size() - 1].length() && words.size() == 1) {
		cout << "share: ‘" + path + "’: you can't share root. Permission denied" << endl;
		return;
	}
	if(!words[words.size() - 1].length() || words.size() > 1 || words[0] == "." || words[0] == "..") {
		for(int i = 0; i < words.size() - 1; i++) {
			Path += words[i];
			Path += "/";
		}
		if(words.size() == 1 && (words[0] == "." || words[0] == ".."))
			path += words[0];
		try {
			Dir = Dir->cd(Path);
		} catch(cd_exception ex) {
			cout << "share: ‘" + path + "’: No such file or directory" << endl;
			return;
		}
	}
	else {
		if(!Dir->get_perm()) {
			cout << "share: Permission denied" << endl;
			return;
		}
		for(int i = 0; i < Dir->in_files.size(); i++)
			if(Dir->in_files[i]->get_name() == words[0]) {
				for(int j = 0; j < User_perms.size(); j++)
					um->share(Dir->in_files[i], User_perms[j].username, User_perms[j].write_perm);
				return;
			}
		for(int i = 0; i < Dir->in_dir.size(); i++) {
			if(Dir->in_dir[i]->get_name() == words[0]) {
				for(int j = 0; j < Dir->in_dir[i]->in_files.size(); j++)
					share(Dir->in_dir[i]->in_files[j]->get_name(), User_perms, um, Dir->in_dir[i]);
				for(int j = 0; j < Dir->in_dir[i]->in_dir.size(); j++)
					share(Dir->in_dir[i]->in_dir[j]->get_name(), User_perms, um, Dir->in_dir[i]);
				return;
			}
		}
		cout << "share: ‘" + path + "’: No such file or directory" << endl;
		return;
	}
	if(!Dir->get_perm()) {
		cout << "share: Permission denied" << endl;
		return;
	}
	for(int i = 0; i < Dir->in_files.size(); i++)
		if(Dir->in_files[i]->get_name() == words[words.size() - 1]) {
			for(int j = 0; j < User_perms.size(); j++)
				um->share(Dir->in_files[i], User_perms[j].username, User_perms[j].write_perm);
			return;
		}
	for(int i = 0; i < Dir->in_dir.size(); i++) {
		if(Dir->in_dir[i]->get_name() == words[words.size() - 1]) {
			for(int j = 0; j < Dir->in_dir[i]->in_files.size(); j++)
				share(Dir->in_dir[i]->in_files[j]->get_name(), User_perms, um, Dir->in_dir[i]);
			for(int j = 0; j < Dir->in_dir[i]->in_dir.size(); j++)
				share(Dir->in_dir[i]->in_dir[j]->get_name(), User_perms, um, Dir->in_dir[i]);
			return;
		}
	}
	cout << "share: ‘" + path + "’: No such file or directory" << endl;
}