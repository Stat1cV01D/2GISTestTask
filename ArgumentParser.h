#pragma once

#include <algorithm>
#include <cassert>
#include <exception>
#include <memory>
#include <map>
#include <string>
#include <utility>
#include <vector>

class ArgUsageException: public std::exception
{
public:
	ArgUsageException()
		: std::exception("Invalid argument usage")
	{}
};

class ArgNumberException: public std::exception
{
public:
	ArgNumberException()
		: std::exception("Invalid number of arguments")
	{}
};

class ArgMissingException: public std::exception
{
public:
	ArgMissingException(const std::vector<std::string>& missingArguments)
		: std::exception([&]() {
			  std::string args;
			  for (const auto& arg: missingArguments)
				  args += arg + '\n';
			  return std::string("The following parameters are missing:\n") + args;
		  }().c_str())
	{
	}
};

class Argument;

// NOTE: Inheriting STL container usually considered a bad practice.
// I could have used a certain namespace for this, but
//  but I also wish to illustrate the C++17 feature of inheriting non-default constructors.
class ArgumentsMap : public std::map<std::string, std::shared_ptr<Argument>>
{
public:
	using std::map<std::string, std::shared_ptr<Argument>>::map; // "inherit" the constructors.

	template<class T>
	std::shared_ptr<T> const get() const
	{
		return std::dynamic_pointer_cast<T>(at(std::string(T::name)));
	}
	void parse(int argc, char** argv) const;
};

// Main class for parsing an argument
class Argument
{
public:
	// Contains the result and number of parameters for an argument
	using InitResult = std::tuple<bool, size_t>;

	Argument() = default;

	// initializes the argument at a certain position
	virtual InitResult init(int pos, int argCount, char** args) = 0;

	std::tuple<bool, std::vector<std::string>> areRequirementsMet(ArgumentsMap arguments)
	{
		bool result = true;
		std::vector<std::string> missing;
		for (const auto& req : m_requiredArguments)
		{
			auto pos = arguments.find(req);
			assert(pos != arguments.end());
			if (!pos->second->initialized())
			{
				result = false;
				missing.emplace_back(req);
			}
		};
		return std::make_tuple(result, missing);
	}

	// Returns vector with the parameter and its arguments
	const std::vector<std::string>& results() const;
	
	// Has the argument been supplied?
	bool initialized() const;

protected:
	// The inner method to be used by descendants as a init() trait call;
	//  requires _Name and _ParamCount to be available at compile time.
	template<const std::string_view &_Name, size_t _ParamCount>
	InitResult doInit(int pos, int argCount, char** args)
	{
		auto result = std::make_tuple(false, _ParamCount);
		int paramEnd = pos + _ParamCount;
		if (paramEnd >= argCount)
			throw ArgNumberException();

		if (std::string_view(args[pos]) != _Name)
			return result;

		for (int i = pos; i <= paramEnd; i++)
			m_results.push_back(args[i]);

		std::get<0>(result) = true;
		return result;
	}

	template<class... Args>
	void dependsOn()
	{
		m_requiredArguments = std::vector{ std::string(Args::name)... };
	}

protected:
	std::vector<std::string> m_results;
	std::vector<std::string> m_requiredArguments;
};