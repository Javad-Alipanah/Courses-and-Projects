#include <vector>
#include <sstream>
#include <iomanip> 
#include "user.h"
#include "sign_in_error.h"
using namespace std;

vector<string> parse_command(string line) {
	stringstream ss;
	vector<string> result;
	line += ' ';
	while(line.length()) {
		ss << line;
		string temp;
		ss >> temp;
		line.erase(line.begin(), line.begin() + temp.length());
		while(line.size() && line[0] == ' ')
			line.erase(line.begin());
		result.push_back(temp);
	}
	return result;
}

void handle_one_partial_command(string command, users* US, users_medium* um, admin* Admin) {
	bool signed_in = US->is_any_signed_in();
	if(!signed_in) {
		cout << "Please log in first" << endl;
		return;
	}
	user* u = US->signed_in_user();
	if(command == "logout")
		US->sign_out_everyone();
	else if(command == "storage") {
		user* u = US->signed_in_user();
		vector<request*> requests = u->get_requests();
		cout.precision(2);
		cout << "Allocated space: " << u->get_allocated_storage_amount() << " MBs" << endl;
		cout << "Used space: " << u->get_used_space() << " MBs" << endl;
		if(requests.size())
			cout << "List of your requests: " << endl;
		for(int i = 0; i < requests.size(); i++)
			if(u->get_username() == requests[i]->get_user())
				cout << "\tYou have requested: " << requests[i]->get_amount() << '\t' << "Request " + requests[i]->get_status() << endl;
	}
	else if(command == "pwd")
		cout << u->curr->pwd() << endl;
	else if(command == "userlist") {
		if(!US->is_signed_in("admin")) {
			cout << "Permission denied" << endl;
			return;
		}
		cout.unsetf(ios::floatfield);
		cout.precision(2);
		vector<user_inf> ui = Admin->userlist();
		for(int i = 0; i < ui.size(); i++) {
			cout << "username: " << ui[i].username << endl;
			cout << "\tAllocated space: " << ui[i].allocated << " MBs" << endl;
			cout << "\tUsed space: " << ui[i].used << " MBs" << endl;
		}
	}
	else if(command == "requests") {
		if(!US->is_signed_in("admin")) {
			cout << "Permission denied" << endl;
			return;
		}
		vector<request*> requests = (*US)[0].get_requests();
		for(int i = 0; i < requests.size(); i++) {
			if(requests[i]->get_user() != "admin" || requests[i]->get_status() == "pending")
				cout << i + 1 << ".\tusername: " << requests[i]->get_user() << "\n\tAmount requested: " << requests[i]->get_amount() << endl;
		}
		cout << "You can accept or reject a request this way:\n\taccept/reject <request No.>" << endl;
	}
	else if(command == "ls") {
		vector<string> words = u->curr->ls(u);
		for(int i = 0; i < words.size(); i++)
			cout << words[i] << endl;
	}
	else if(command == "cd")
		u->curr = u->curr->cd();
	else
		cout << "‘" + command + "’: Invalid command" << endl;
}

void handle_two_partial_command(vector<string> command, users* US, users_medium* um, admin* Admin) {
	bool signed_in = US->is_any_signed_in();
	if(!signed_in) {
		cout << "Please log in first" << endl;
		return;
	}
	user* u = US->signed_in_user();
	if(command[0] == "storagerequest") {
		int amount = 0;
		for(int i = 0; i < command[1].length(); i++) {
			if((command[1])[i] > '9' || (command[1])[i] < '0') {
				cout << "storagerequest: ‘" + command[1] + "’: Is not a number" << endl;
				return;
			}
			else {
				amount *= 10;
				amount += (command[1])[i] - 48;
			}
		}
		u->make_request(amount, Admin);
	}
	else if(command[0] == "mkdir")
		u->curr->mkdir(command[1]);
	else if(command[0] == "ls") {
		vector<string> words = u->curr->ls(u, command[1]);
		for(int i = 0; i < words.size(); i++)
			cout << words[i] << endl;
	}
	else if(command[0] == "cd") {
		try {
			u->curr = u->curr->cd(command[1]);
		} catch(cd_exception ex) {
			cout << "cd: ‘" + command[1] + "’: No such file or directory" << endl;
		}
	}
	else if(command[0] == "rm") {
		if(command[1] == "-r") {
			cout << "Invalid use of rm -r" << endl;
			return;
		}
		u->curr->rm(command[1], u);
	}
	else if(command[0] == "cat")
		u->curr->cat(command[1], u);
	else if(command[0] == "destroy")
		Admin->destroy_user(command[1]);
	else if(command[0] == "sharelist")
		u->sharelist(command[1], u->curr);
	else if(command[0] == "history")
		u->show_history(command[1]);
	else if(command[0] == "accept") {
		if(!US->is_signed_in("admin")) {
			cout << "Permission denied" << endl;
			return;
		}
		int idx = 0;
		for(int i = 0; i < command[1].length(); i++)
			if((command[1])[i] < '0' || (command[1])[i] > '9') {
				cout << "accept: ‘" + command[1] + "’: Is not a number" << endl;
				return;
			}
			else {
				idx *= 10;
				idx += (command[1])[i] - 48;
			}
		Admin->accept_request(idx);
	}
	else if(command[0] == "reject") {
		if(!US->is_signed_in("admin")) {
			cout << "Permission denied" << endl;
			return;
		}
		int idx = 0;
		for(int i = 0; i < command[1].length(); i++)
			if((command[1])[i] < '0' || (command[1])[i] > '9') {
				cout << "reject: ‘" + command[1] + "’: Is not a number" << endl;
				return;
			}
			else {
				idx *= 10;
				idx += (command[1])[i] - 48;
			}
		Admin->reject_request(idx);
	}
	else 
		cout << "‘" + command[0] + "’: Invalid command" << endl;
}

void handle_three_partial_command(vector<string> command, users* US, users_medium* um, admin* Admin) {
	if(command[0] == "useradd") {
		if(US->does_exist(command[1])) {
			cout << "useradd: ‘" + command[1] + "’: Username has taken before" << endl;
			return;
		}
		US->add(command[1], command[2]);
		return;
	}
	else if(command[0] == "login") {
		if(US->is_any_signed_in()) {
			cout << "login: You have logged in before" << endl;
			return;
		}
		try {
			US->sign_in_user(command[1], command[2]);
		} catch(sign_in_error ex) {
			cerr << "login: username or password is incorrect" << endl;
		}
		return;
	}
	bool signed_in = US->is_any_signed_in();
	if(!signed_in) {
		cout << "Please log in first" << endl;
		return;
	}
	user* u = US->signed_in_user();
	if(command[0] == "rm" && command[1] == "-r")
		u->rm_r(command[2]);
	else if(command[0] == "mv")
		u->curr->mv(command[1], command[2], u);
	else if(command[0] == "cp")
		u->curr->cp(command[1], command[2], u, US);
	else if(command[0] == "put")
		u->curr->put(command[1], command[2], u, US);
	else if(command[0] == "get")
		u->curr->get(command[1], command[2], u);
	else if(command[0] == "revert")
		u->revert(command[1], command[2]);
	else
		cout << "‘" + command[0] + "’: Invalid command" << endl;
}

user_perm build_user_perm(string up) {
	bool perm;
	string username = "";
	string Permission = "";
	bool column = false;
	for(int i = 0; i < up.length(); i++) {
		if(!column)
			username += up[i];
		if(up[i + 1] == ':') {
			column = true;
			i++;
			continue;
		}
		if(column)
			Permission += up[i];
	}
	if(Permission != "w" && Permission!= "r")
		throw -1;
	if(Permission == "w")
		perm = true;
	else
		perm = false;
	return user_perm(username, perm);
}

void handle_share_commands(vector<string> command, users* US, users_medium* um, admin* Admin) {
	bool signed_in = US->is_any_signed_in();
	if(!signed_in) {
		cout << "Please log in first" << endl;
		return;
	}
	user* u = US->signed_in_user();
	if(command[0] == "share") {
		if(command.size() < 3) {
			cout << "share must have at least three arguments" << endl;
			return;
		}
		vector<user_perm> up;
		for(int i = 2; i < command.size(); i++) {
			try {
				up.push_back(build_user_perm(command[i]));
			}
			catch(int a) {
				cout << "share: ‘" + command[i] + "’: Is not valid" << endl;
				return;
			}
		}
		u->share(command[1], up, um, u->curr);
	}
	else if(command[0] == "unshare") {
		if(command.size() < 3) {
			cout << "unshare must have at least three arguments" << endl;
			return;
		}
		vector<string> us;
		for(int i = 2; i < command.size(); i++)
				us.push_back(command[i]);
		u->unshare(command[1], us, um, u->curr);
	}
}

void handle_command(users* US, admin* Admin, users_medium* UM, string line) {
	vector<string> command = parse_command(line);
	if(line == "\n" || !line.size())
		return;
	if(!command.size())
		return;
	else if(command[0] == "share" || command[0] == "unshare")
		handle_share_commands(command, US, UM, Admin);
	else if(command.size() == 1)
		handle_one_partial_command(command[0], US, UM, Admin);
	else if(command.size() == 2)
		handle_two_partial_command(command, US, UM, Admin);
	else if(command.size() == 3)
		handle_three_partial_command(command, US, UM, Admin);
	else
		cout << "‘" + command[0] + "’: Invalid command" << endl;
}