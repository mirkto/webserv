#include <iostream>
#include <string>
#include <fstream>
#include "config.hpp"

int main(int argc, char** argv) {
	Config config;
	
	std::string file;
	if(argc == 2)
		file = argv[1];
	else
		file = "serv.conf";
	config.loop(file);
	//config.see_config();
	return 0;
}