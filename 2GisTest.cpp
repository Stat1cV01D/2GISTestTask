#include "Arguments.h"
#include "Logic.h"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

template<class T>
static std::pair<std::string, std::shared_ptr<Argument>> createPair()
{
	static_assert(std::is_base_of_v<Argument, T>, "T is not a base class of Argument");
	return std::make_pair(std::string(T::name), std::make_shared<T>());
}

int main(int argc, char **argv)
{
	const ArgumentsMap arguments{
		createPair<HelpArg>(),
		createPair<FileArg>(),
		createPair<ModeArg>(),
		createPair<WordArg>()
	};

	try
	{
		arguments.parse(argc, argv);

		if (auto helpArg = arguments.get<HelpArg>(); arguments.empty() || helpArg->initialized())
		{
			for (const auto& str : helpArg->results())
				std::cout << str;
			return 0;
		}

		auto file = arguments.get<FileArg>()->results().at(1);

		switch (arguments.get<ModeArg>()->mode())
		{
		case ModeArg::Checksum: {
			auto result = getChecksum(file);
			std::cout << "The checksum of file \"" << file << "\" is " << std::setbase(16) << "0x" << result;
			break;
		}
		case ModeArg::SearchWord: {
			auto word = arguments.get<WordArg>()->results().at(1);
			auto result = countWords(file, word);
			std::cout << "The word \"" << word << "\" appears " << result << " times in file \"" << file << "\"";
			break;
		}
		}
	}
	catch (std::exception e)
	{
		std::cout << e.what() << "\n\n";
		std::cout << arguments.get<HelpArg>()->usage();
		return 1;
	}
	return 0;
}
