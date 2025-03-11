#include <iostream>
#include <chrono>
#include <stdlib.h>
#include <string.h>
#include <sstream>

class Test{
	private:
		std::string name;
	public:
		Test();
		~Test();
		std::string const& getName();
		void myFunction();
		void myFunction2();
};

/*
	void Irc::receiveRequest(int targetFd) {
	char buffer[30000];
	bzero(buffer, sizeof(buffer));
	istringstream ss;
	Client *client = findClient(targetFd);

	if (recv(targetFd, &buffer, sizeof(buffer), 0) <= 0)
		quitCmd(ss, client);
	client->_buffer += string (buffer);
	if (client->_buffer.find('\n') == string::npos - 1)
		return;
	requests.insert(make_pair(targetFd, client->_buffer));
	epfds->modFd(targetFd, EPOLLOUT);
	client->_buffer.clear();	
}
*/

Test::Test() {
	// Your code here
}

Test::~Test() {
	// Your code here
}

void writeBuffer(char* buffer){
	char str[] = "hello guys\n";
	strcpy(buffer, str);
}


void Test::myFunction() {
    // Your code here
	char buffer[3000];
	bzero(buffer, sizeof(buffer));
	std::istringstream ss;

	writeBuffer(buffer);
	this->name += std::string(buffer);

	this->name.clear();
}

void Test::myFunction2() {
	// Your code here
	char buffer[3000];
	bzero(buffer, sizeof(buffer));
	std::istringstream ss;
	std::string name;

	writeBuffer(buffer);
	name += std::string(buffer);

	name.clear();
}

int main() {
	Test test;
    auto start = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 1000000; i++){
		//test.myFunction();  // Function you want to test
		test.myFunction2();  // Function you want to test
	}

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double, std::milli>(end - start);

    std::cout << "Execution time: " << duration.count() << " ms" << std::endl;
    return 0;
}