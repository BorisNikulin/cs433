#include "shell.h"

#include <iostream>

#include <unistd.h>
#include <fcntl.h>

int main()
{
	using namespace shell;

	//int outFile = open("out.txt", O_CREAT | O_TRUNC | O_WRONLY, 0640);

	//SingleProgram sp;

	//char prog[3][10] = {"sleep", "3s", {}};
	//char** top = (char**) malloc(3 * sizeof(char*));
	//for(size_t i = 0; i < 3; ++i)
	//{
		//top[i] = prog[i];
	//}
	//top[2] = nullptr;
	////execvp(prog[0], top);

	//sp.prog = top;
	//sp.runInBackground = false;
	//sp.out = outFile;
	//sp.run();
	//delete sp.prog;

	Shell shell;
	std::string inLine;
	bool doQuit;
	int returnVal;

	do
	{
		std::cout << "bsh> ";
		std::getline(std::cin, inLine);
		auto ret = shell.processCommand(inLine);
		doQuit = ret.first;
		returnVal = ret.second;
	} while(!doQuit);

	return returnVal;
}
