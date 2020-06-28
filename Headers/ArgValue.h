#pragma once

#include "ArgsParserException.h"
#include "ArgStringParsers.h"

#include <any>
#include <string>

namespace SimpleArgsParser
{

struct IArgValue
{
	virtual std::any GetFromString(const std::string& value) const = 0;
	virtual const std::any& GetDefault() const = 0;
	virtual std::string GetStringDefaultValue() const = 0;
	virtual bool HasDefaultValue() const = 0;

	virtual ~IArgValue() = default;
};

template<typename T>
class ArgValue : public IArgValue
{

public:
	ArgValue& SetDefault(T value)
	{
		default_value_ = std::move(value);
		return *this;
	}

	std::any GetFromString(const std::string& value) const override
	{
		return ParseFromString(value, ArgsParserHelpStruct<T>());
	}

	const std::any& GetDefault() const override
	{
		if (!HasDefaultValue())
		{
			throw ArgsParserException("Value not set.");
		}
		return default_value_;
	}

	std::string GetStringDefaultValue() const override
	{
		if (!HasDefaultValue())
		{
			throw ArgsParserException("Value not set.");
		}
		return ConvertToString(std::any_cast<T>(default_value_));
	}

	bool HasDefaultValue() const override
	{
		return default_value_.has_value();
	}

private:
	std::any default_value_;
};

} // namespace SimpleArgsParser
