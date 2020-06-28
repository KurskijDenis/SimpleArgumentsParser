#pragma once

#include "ArgOptions.h"

#include <memory>
#include <string>

namespace SimpleArgsParser
{

struct IArgValue;

class ArgInfos
{

public:
	ArgInfos(
		std::string full_name,
		std::string help,
		std::unique_ptr<IArgValue> arg_value,
		ArgOptions arg_options,
		std::string short_name);

	ArgInfos(ArgInfos&&) = default;

	const IArgValue& GetValue() const;

	const ArgOptions& GetOptions() const;

	const std::string& GetHelp() const;

	const std::string& GetShortName() const;

	const std::string& GetFullName() const;

	bool HasValue() const;

	~ArgInfos();

private:
	std::string full_name_;
	std::string help_;
	std::unique_ptr<IArgValue> arg_value_;
	ArgOptions arg_options_;
	std::string short_name_;
};

} // namespace SimpleArgsParser