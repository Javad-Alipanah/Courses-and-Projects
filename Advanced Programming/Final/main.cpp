#include "user.h"
void handle_command(users* US, admin* Admin, users_medium* UM, string line);

int main() {
	users US;
	bool x = false;
	admin Admin = admin::access_admin_acc(&US);
	users_medium UM = users_medium::access_medium(&US);
	US.add(Admin);
	string line;
	while(getline(cin, line)) {
		if(US.is_signed_in("admin"))
			x = true;
		else
			x = false;
		Admin.set_used_space(US[0].get_used_space());
		(*US(0)) = Admin;
		if(x)
			(*US(0)).sign_in();
		if(line == "exit")
			break;
		handle_command(&US, &Admin, &UM, line);
		if(US.is_any_signed_in()) {
			user* u = US.signed_in_user();
			cout << u->get_username() << "@root:" << u->curr->pwd() << ' ';
		}
	}
	return 0;
}