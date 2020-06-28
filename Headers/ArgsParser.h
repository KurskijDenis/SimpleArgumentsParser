#pragma once

#include "ArgInfos.h"
#include "ArgOptions.h"
#include "ArgValue.h"
#include "ArgsParserException.h"
#include "ArgsParserHelpStruct.h"

#include <any>
#include <map>
#include <memory>

namespace SimpleArgsParser
{

class ArgsInitializer
{

public:
	explicit ArgsInitializer(
		std::string description = "",
		size_t max_size_arg_help_desc = 40,
		size_t max_size_arg_help_info = 40);

	template<typename Type>
	ArgsInitializer& operator()(
		std::string option_name,
		std::string help,
		ArgValue<Type> value,
		ArgOptions arg_options = ArgOptions())
	{
		AddArg(
			std::move(option_name),
			std::move(help),
			std::make_unique<ArgValue<Type>>(std::move(value)),
			std::move(arg_options));
		return *this;
	}

	ArgsInitializer& operator()(
		std::string option_name,
		std::string help,
		ArgOptions arg_options = ArgOptions());

	const ArgInfos& GetArgInfos(std::string value) const;
	const std::map<std::string, ArgInfos>& GetArgsInfos() const;
	const std::string& GetDescription() const;
	size_t GetMaxSizeArgHelpDesc() const;
	size_t GetMaxSizeArgHelpInfo() const;

private:
	void AddArg(
		std::string option_name,
		std::string help,
		std::unique_ptr<IArgValue> arg_value,
		ArgOptions arg_options);

private:
	std::map<std::string, ArgInfos> args_infos_;
	std::map<std::string, std::string> short_to_full_name_;
	std::string description_;
	size_t max_size_arg_help_desc_;
	size_t max_size_arg_help_info_;
};

class ArgsContainer
{

public:
	ArgsContainer(
		std::map<std::string, std::any> args,
		std::map<std::string, std::string> short_to_full_name);

	bool Exist(const std::string& key) const;

	template<typename Type>
	Type GetValue(std::string key) const
	{
		if (key.size() <= 1)
		{
			throw ArgsParserException("Value with key " + key + " not set.");
		}
		if (key.size() == 2 || key[1] != '-')
		{
			const auto it = short_to_full_name_.find(key);
			if (it == short_to_full_name_.cend())
			{
				throw ArgsParserException("Value with key " + key + " not set.");
			}
			key = it->second;
		}
		const auto it = args_.find(key);
		if (it == args_.cend())
		{
			throw ArgsParserException("Value not set.");
		}
		try
		{
			return std::any_cast<Type>(it->second);
		}
		catch (const std::bad_any_cast& exc)
		{
			throw ArgsParserException(exc.what());
		}
	}

	size_t Count() const;

private:
	const std::map<std::string, std::any> args_;
	const std::map<std::string, std::string> short_to_full_name_;
};

ArgsContainer ParseArgs(
	const int argc,
	const char* const* argv,
	const ArgsInitializer& argument_initializer);

} // namespace SimpleArgsParser