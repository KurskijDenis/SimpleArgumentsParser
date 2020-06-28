#include "../Headers/ArgsParserException.h"

namespace SimpleArgsParser
{

ArgsParserException::ArgsParserException(std::string what_str)
	: what_(std::move(what_str))
{}

const char* ArgsParserException::what() const noexcept
{
	return what_.c_str();
}

ArgsInitializerException::ArgsInitializerException(std::string what_str)
	: what_(std::move(what_str))
{}

const char* ArgsInitializerException::what() const noexcept
{
	return what_.c_str();
}

} // namespace SimpleArgsParser