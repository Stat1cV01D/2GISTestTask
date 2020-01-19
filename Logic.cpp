#include "Logic.h"

#include <exception>
#include <fstream>

class FileNotFoundException: public std::exception
{
public:
	FileNotFoundException(const std::string& file)
		: std::exception((std::string("Unable to open file ") + file).c_str())
	{}
};

uint32_t countWords(const std::string& file, const std::string& word)
{
	std::ifstream srcFile(file);
	uint32_t count = 0;

	if (!srcFile.is_open())
		throw FileNotFoundException(file);

	while (srcFile)
	{
		std::string str;
		std::getline(srcFile, str);

		for (size_t i = 0, size = str.size(), wordSize = word.size(); i < size; /*empty*/)
		{
			i = str.find(word, i) + wordSize;
			if (i == std::string::npos)
				break;
			count++;
		}
	}

	return count;
}

uint32_t getChecksum(const std::string& file)
{
	std::ifstream srcFile(file, std::ios::binary);

	if (!srcFile.is_open())
		throw FileNotFoundException(file);

	uint32_t checksum = 0;

	uint32_t dwordBuf;
	while (srcFile.read((char*)&dwordBuf, sizeof(dwordBuf)))
	{
		checksum += dwordBuf;
	}

	return checksum;
}
