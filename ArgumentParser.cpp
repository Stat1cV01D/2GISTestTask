#include "ArgumentParser.h"

#include <map>

const std::vector<std::string>& Argument::results() const
{
	return m_results;
}

bool Argument::initialized() const
{
	return !results().empty();
}

void ArgumentsMap::parse(int argc, char** argv) const
{
	for (int i = 1; i < argc; i++)
	{
		auto pos = find(argv[i]);
		if (pos == end())
			throw ArgUsageException();

		std::shared_ptr<Argument> arg;
		std::tie(std::ignore, arg) = *pos;
		auto [result, paramCount] = arg->init(i, argc, argv);
		if (result)
			i += paramCount;
	}

	// After we parse all the arguments 
	//  start looking for missing dependencies.
	for (const auto& [k, arg] : *this)
	{
		auto [result, missedRequirements] = arg->areRequirementsMet(*this);
		if (!result)
			throw ArgMissingException(missedRequirements);
	}
}