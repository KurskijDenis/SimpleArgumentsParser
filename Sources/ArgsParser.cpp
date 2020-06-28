#include "../Headers/ArgsParser.h"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace SimpleArgsParser
{

namespace
{

std::string GetHelpString(
	const std::string& program_name,
	const ArgsInitializer& args_infos)
{
	std::ostringstream result;
	result << "Usage: " << program_name << " [options]\n";

	if (const auto& desc = args_infos.GetDescription(); !desc.empty())
	{
		result << desc << "\n";
	}

	const auto& infos = args_infos.GetArgsInfos();
	const auto max_size_arg_help_desc = args_infos.GetMaxSizeArgHelpDesc();
	const auto max_size_arg_help_info = args_infos.GetMaxSizeArgHelpInfo();

	result << "Available options:\n";

	for (const auto& info : infos)
	{
		const auto& arg_info = info.second;
		std::string arg_string = "  " + arg_info.GetFullName();

		if (!arg_info.GetShortName().empty())
		{
			arg_string += "," + arg_info.GetShortName();
		}

		if (arg_info.HasValue())
		{
			arg_string += " arg";
			if (arg_info.GetValue().HasDefaultValue())
			{
				arg_string += "(=" + arg_info.GetValue().GetStringDefaultValue() + ")";
			}
		}

		if (arg_string.size() + 1 > max_size_arg_help_desc)
		{
			result << arg_string << "\n";
			result << std::setw(max_size_arg_help_desc) << "";
		}
		else
		{
			result << arg_string << std::setw(max_size_arg_help_desc - arg_string.size()) << "";
		}

		const auto& help = arg_info.GetHelp();
		size_t prev_local_pos = 0;
		size_t start_line_pos = 0;

		auto pos = help.find(' ', start_line_pos + 1);
		while (pos != std::string::npos)
		{
			if (pos > start_line_pos + max_size_arg_help_info)
			{
				if (start_line_pos != 0)
				{
					result << std::setw(max_size_arg_help_desc) << "";
				}
				if (prev_local_pos == 0)
				{
					result << help.substr(start_line_pos, pos - start_line_pos) << "\n";
					start_line_pos = pos + 1;
					pos = help.find(' ', start_line_pos);
				}
				else
				{
					result << help.substr(start_line_pos, prev_local_pos) << "\n";
					start_line_pos += prev_local_pos + 1;
					prev_local_pos = 0;
				}
			}
			else
			{
				prev_local_pos = pos - start_line_pos;
				pos = help.find(' ', pos + 1);
			}
		}

		if (help.size() > start_line_pos)
		{
			if (start_line_pos != 0)
			{
				result << std::setw(max_size_arg_help_desc) << "";
			}
			if (help.size() > max_size_arg_help_info + start_line_pos && prev_local_pos != 0)
			{
				result << help.substr(start_line_pos, prev_local_pos) << "\n";
				start_line_pos += prev_local_pos + 1;
				prev_local_pos = 0;
				if (help.size() > start_line_pos)
				{
					result << std::setw(max_size_arg_help_desc) << "";
					result << help.substr(start_line_pos, help.size() - start_line_pos) << "\n";
				}
			}
			else
			{
				result << help.substr(start_line_pos, help.size() - start_line_pos) << "\n";
			}
		}
	}
	return result.str();
}

std::pair<std::string, std::string> SplitOptionName(std::string value)
{
	if (value.empty())
	{
		throw ArgsParserException("Empty option name.");
	}
	if (value.front() == '-')
	{
		throw ArgsParserException("Incorrect full option name (please remove '-') " + value + ".");
	}
	value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
	const auto pos = value.find(',');
	if (pos != std::string::npos)
	{
		if (pos == 0)
		{
			throw ArgsParserException("Empty full option name.");
		}
		const auto full_name = value.substr(0, pos);
		const auto short_name = value.substr(pos + 1);
		if (short_name.empty())
		{
			throw ArgsParserException("Empty short option name.");
		}
		if (short_name.front() == '-')
		{
			throw ArgsParserException("Incorrect short option name (please remove '-') " + short_name + ".");
		}
		return std::make_pair("--" + full_name, "-" + short_name);
	}
	return std::make_pair("--" + value, "");
}

} // namespace


ArgsInitializer::ArgsInitializer(
	std::string description,
	size_t max_size_arg_help_desc,
	size_t max_size_arg_help_info)
	: description_(std::move(description))
	, max_size_arg_help_desc_(max_size_arg_help_desc)
	, max_size_arg_help_info_(max_size_arg_help_info)
{
	if (max_size_arg_help_desc_ == 0)
	{
		throw ArgsInitializerException("Max string size for argument description must be > 0.");
	}
	if (max_size_arg_help_info_ == 0)
	{
		throw ArgsInitializerException("Max string size for argument info must be > 0.");
	}
}

ArgsInitializer& ArgsInitializer::operator()(
	std::string option_name,
	std::string help,
	ArgOptions arg_options)
{
	AddArg(
		std::move(option_name),
		std::move(help),
		nullptr,
		std::move(arg_options));
	return *this;
}

const ArgInfos& ArgsInitializer::GetArgInfos(std::string value) const
{
	if (value.empty())
	{
		throw ArgsParserException("Unknown empty param.");
	}
	if (value.size() == 2 || value[1] != '-')
	{
		const auto it = short_to_full_name_.find(value);
		if (it == short_to_full_name_.cend())
		{
			throw ArgsParserException("Unknown param: " + std::string(value) + ".");
		}
		value = it->second;
	}
	const auto it = args_infos_.find(value);
	if (it == args_infos_.cend())
	{
		throw ArgsParserException("Unknown param: " + std::string(value) + ".");
	}
	return it->second;
}

const std::map<std::string, ArgInfos>& ArgsInitializer::GetArgsInfos() const
{
	return args_infos_;
}

const std::string& ArgsInitializer::GetDescription() const
{
	return description_;
}

size_t ArgsInitializer::GetMaxSizeArgHelpDesc() const
{
	return max_size_arg_help_desc_;
}

size_t ArgsInitializer::GetMaxSizeArgHelpInfo() const
{
	return max_size_arg_help_info_;
}

void ArgsInitializer::AddArg(
	std::string option_name,
	std::string help,
	std::unique_ptr<IArgValue> arg_value,
	ArgOptions arg_options)
{
	auto [full_name, short_name] = SplitOptionName(option_name);
	if (full_name == "--help" || short_name == "-h")
	{
		throw ArgsParserException("--help, -h reserved args.");
	}
	if (args_infos_.count(full_name) != 0)
	{
		throw ArgsParserException("Duplicate full option name " + full_name + ".");
	}

	if (!short_name.empty())
	{
		if (short_to_full_name_.count(short_name) != 0)
		{
			throw ArgsParserException("Duplicate short option name " + short_name + ".");
		}
		short_to_full_name_.emplace(short_name, full_name);
	}
	args_infos_.emplace(
		full_name,
		ArgInfos(
			full_name,
			std::move(help),
			std::move(arg_value),
			std::move(arg_options),
			std::move(short_name)));
}

ArgsContainer::ArgsContainer(
	std::map<std::string, std::any> args,
	std::map<std::string, std::string> short_to_full_name)
	: args_(std::move(args))
	, short_to_full_name_(std::move(short_to_full_name))
{}

bool ArgsContainer::Exist(const std::string& key) const
{
	if (key.size() <= 1)
	{
		return false;
	}
	if (key.size() == 2 || key[1] != '-')
	{
		const auto it = short_to_full_name_.find(key);
		if (it == short_to_full_name_.cend())
		{
			return false;
		}
		return args_.count(it->second) != 0;
	}
	return args_.count(key) != 0;
}

size_t ArgsContainer::Count() const
{
	return args_.size();
}

ArgsContainer ParseArgs(
	const int argc,
	const char* const* argv,
	const ArgsInitializer& argument_initializer)
{
	if (argc <= 0)
	{
		throw ArgsParserException("Incorrect value of argc param.");
	}
	if (argv == nullptr)
	{
		throw ArgsParserException("Incorrect value of argv param.");
	}

	std::map<std::string, std::any> filled_options;
	std::map<std::string, std::string> short_to_full_name;
	filled_options.emplace("--help", GetHelpString(argv[0], argument_initializer));
	short_to_full_name.emplace("-h", "--help");
	const auto u_argc = static_cast<size_t>(argc);
	for (size_t i = 1; i < u_argc; ++i)
	{
		const auto* param = argv[i];

		if (param == nullptr)
		{
			throw ArgsParserException("Incorrect value of argv param.");
		}

		const auto& arg_info = argument_initializer.GetArgInfos(param);
		const auto& full_option_name = arg_info.GetFullName();
		if (!arg_info.GetShortName().empty())
		{
			short_to_full_name.emplace(arg_info.GetShortName(), full_option_name);
		}

		if (!arg_info.HasValue())
		{
			filled_options.emplace(full_option_name, true);
			continue;
		}

		if (i + 1 == u_argc)
		{
			throw ArgsParserException("Please set param value: " + std::string(param) + ".");
		}
		++i;

		auto value = arg_info.GetValue().GetFromString(argv[i]);
		filled_options.emplace(full_option_name, std::move(value));
	}

	for (const auto& [key, value] : argument_initializer.GetArgsInfos())
	{
		if (filled_options.count(key) != 0)
		{
			continue;
		}

		const auto& options = value.GetOptions();
		if (options.required)
		{
			throw ArgsParserException("Please set required param " + key + ".");
		}

		if (!value.HasValue() || !value.GetValue().HasDefaultValue())
		{
			continue;
		}

		if (!value.GetShortName().empty())
		{
			short_to_full_name.emplace(value.GetShortName(), key);
		}

		filled_options.emplace(key, value.GetValue().GetDefault());
	}
	return ArgsContainer(std::move(filled_options), std::move(short_to_full_name));
}

} // namespace SimpleArgsParser