#pragma once

#include <string>

namespace SimpleArgsParser
{

class ArgsParserException : public std::exception
{
public:
	explicit ArgsParserException(std::string what_str);
	const char* what() const noexcept override;

private:
	const std::string what_;
};

class ArgsInitializerException : public std::exception
{
public:
	explicit ArgsInitializerException(std::string what_str);
	const char* what() const noexcept override;

private:
	const std::string what_;
};

} // namespace SimpleArgsParser