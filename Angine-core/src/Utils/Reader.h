#pragma once
#include <string>
#include <stdio.h>
#include <iostream>
namespace Utils {
	std::string static read_file(const char * fileLocation)
	{
		FILE * file = fopen(fileLocation, "rt");
		if (file == nullptr)
		{
			std::cout << "couldn't open file : " << fileLocation << std::endl;
			return "";
		}
		fseek(file, 0, SEEK_END);
		long length = ftell(file);
		fseek(file, 0, SEEK_SET);
		char* out = (char *)malloc((length+1) * sizeof(char));
		memset(out, 0, length+1);

		fread(out, 1, length, file);
		std::string output = out;
		free(out);
		fclose(file);
		return output;
	};
}