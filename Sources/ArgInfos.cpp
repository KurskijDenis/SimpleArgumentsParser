#include "../Headers/ArgInfos.h"
#include "../Headers/ArgsParserException.h"
#include "../Headers/ArgValue.h"

namespace SimpleArgsParser
{

ArgInfos::ArgInfos(
	std::string full_name,
	std::string help,
	std::unique_ptr<IArgValue> arg_value,
	ArgOptions arg_options,
	std::string short_name)
	: full_name_(std::move(full_name))
	, help_(std::move(help))
	, arg_value_(std::move(arg_value))
	, arg_options_(std::move(arg_options))
	, short_name_(std::move(short_name))
{}

const IArgValue& ArgInfos::GetValue() const
{
	if (!HasValue())
	{
		throw ArgsParserException("Can't get empty param value.");
	}
	return *arg_value_;
}

const ArgOptions& ArgInfos::GetOptions() const
{
	return arg_options_;
}

const std::string& ArgInfos::GetHelp() const
{
	return help_;
}

const std::string& ArgInfos::GetShortName() const
{
	return short_name_;
}

const std::string& ArgInfos::GetFullName() const
{
	return full_name_;
}

bool ArgInfos::HasValue() const
{
	return static_cast<bool>(arg_value_);
}

ArgInfos::~ArgInfos() = default;

} // namespace SimpleArgsParser