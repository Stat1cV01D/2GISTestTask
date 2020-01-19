#pragma once

#include "ArgumentParser.h"

class FileArg : public Argument
{
public:
	// name and parametersCount are needed before object construction
	//  so they are defined as "static constexpr" and we don't waste
	//  memory by creating temporary objects just to get a const.
	static constexpr std::string_view name = "-f";
	static constexpr int parametersCount = 1;

	InitResult init(int pos, int argc, char** argv) override;
};

class WordArg : public Argument
{
public:
	static constexpr std::string_view name = "-v";
	static constexpr int parametersCount = 1;

	InitResult init(int pos, int argc, char** argv) override;
};

class ModeArg : public Argument
{
public:
	static constexpr std::string_view name = "-m";
	static constexpr int parametersCount = 1;

	enum Mode
	{
		Invalid,
		SearchWord,
		Checksum
	};
	Mode mode() const { return m_mode; }

	InitResult init(int pos, int argc, char** argv) override;

private:
	Mode m_mode{ SearchWord };
};

class HelpArg : public Argument
{
public:
	static constexpr std::string_view name = "-h";
	static constexpr int parametersCount = 0;

	InitResult init(int pos, int argc, char** argv) override;
	std::string usage() const;
};

