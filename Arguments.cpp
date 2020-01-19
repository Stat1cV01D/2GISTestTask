#include "Arguments.h"

auto FileArg::init(int pos, int argc, char** argv) -> InitResult
{
	return Argument::doInit<name, parametersCount>(pos, argc, argv);
}


auto ModeArg::init(int pos, int argc, char** argv) -> InitResult
{
	auto result = Argument::doInit<name, parametersCount>(pos, argc, argv);
	if (!std::get<0>(result))
		return result;

	static std::map<std::string, Mode> allowedModes{
		{"words", SearchWord},
		{"checksum", Checksum}
	};

	auto iMode = std::find_if(
		allowedModes.begin(),
		allowedModes.end(),
		[this](const auto& it)
		{
			return it.first == m_results.at(1);
		});

	if (iMode == allowedModes.end())
		throw ArgUsageException();

	std::tie(std::ignore, m_mode) = *iMode;

	switch (m_mode)
	{
	case SearchWord:
		dependsOn<FileArg, WordArg>();
		break;
	case Checksum:
		dependsOn<FileArg>();
		break;
	}

	return result;
}


auto WordArg::init(int pos, int argc, char** argv) -> InitResult
{
	dependsOn<ModeArg>();
	return Argument::doInit<name, parametersCount>(pos, argc, argv);
}


auto HelpArg::init(int pos, int argc, char** argv) -> InitResult
{
	auto result = Argument::doInit<name, parametersCount>(pos, argc, argv);
	if (std::get<0>(result))
		m_results.clear();

	m_results.push_back("2GIS test task\n");
	m_results.push_back(usage());
	return result;
}

std::string HelpArg::usage() const
{
	return
		"Usage:\n"
		"-f <file> : set an input file\n"
		"-m <mode> : a mode the program will work in\n"
		"            <mode> can be one of the following values:\n"
		"            \"words\"    : searches for a certain word in the file,\n"
		"                           requires argument \"-v\"\n"
		"            \"checksum\" : calculates 32-bit checksum of the file\n"
		"-v <word> : sets a word to search for, required by mode \"words\"\n"
		"-h        : prints this help";
}
