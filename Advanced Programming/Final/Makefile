All: a.out

a.out: main.o handle_commands.o user.o file_and_dir.o storage
	g++ main.o handle_commands.o user.o file_and_dir.o

main.o: main.cpp user.h file_and_dir.h cd_exception.h
	g++ -c main.cpp

handle_commands.o: handle_commands.cpp user.h file_and_dir.h cd_exception.h sign_in_error.h
	g++ -c handle_commands.cpp

user.o: user.cpp user.h file_and_dir.h cd_exception.h sign_in_error.h
	g++ -c user.cpp

file_and_dir.o: file_and_dir.cpp user.h file_and_dir.h cd_exception.h
	g++ -c file_and_dir.cpp

clear:
	rm *.o

remove:
	rm a.out
	rm -r storage

storage:
	mkdir storage