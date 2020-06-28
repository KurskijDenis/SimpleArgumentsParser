#pragma once

#include "ArgsParserException.h"
#include "ArgsParserHelpStruct.h"

#include <stdexcept>
#include <string>

namespace SimpleArgsParser
{

template<typename Type>
std::enable_if_t<std::is_integral_v<Type>, Type>
ParseFromString(const std::string& value, const ArgsParserHelpStruct<Type>)
{
	try
	{
		if constexpr (std::is_unsigned<Type>::value)
		{
			const auto result = std::stoull(value);
			if (result > std::numeric_limits<Type>::max())
			{
				throw ArgsParserException("Value out of range.");
			}
			return static_cast<Type>(result);
		}
		else
		{
			const auto result = std::stoll(value);
			if (result > std::numeric_limits<Type>::max() || result < std::numeric_limits<Type>::min())
			{
				throw ArgsParserException("Value out of range.");
			}
			return static_cast<Type>(result);
		}
	}
	catch (const std::invalid_argument& exc)
	{
		throw ArgsParserException(exc.what());
	}
	catch (const std::out_of_range& exc)
	{
		throw ArgsParserException(exc.what());
	}
}

template<typename Type>
std::enable_if_t<std::is_floating_point_v<Type>, Type>
ParseFromString(const std::string& value, const ArgsParserHelpStruct<Type>)
{
	try
	{
		const auto result = std::stold(value);
		if (result > std::numeric_limits<Type>::max() || result < std::numeric_limits<Type>::min())
		{
			throw ArgsParserException("Value out of range.");
		}
		return static_cast<Type>(result);
	}
	catch (const std::invalid_argument& exc)
	{
		throw ArgsParserException(exc.what());
	}
	catch (const std::out_of_range& exc)
	{
		throw ArgsParserException(exc.what());
	}
}

template<typename Type>
std::enable_if_t<std::is_arithmetic_v<Type>, std::string>
ConvertToString(const Type& value)
{
	return std::to_string(value);
}

inline std::string ParseFromString(const std::string& value, const ArgsParserHelpStruct<std::string>)
{
	return value;
}

inline std::string ConvertToString(const std::string& value)
{
	return value;
}

} // namespace SimpleArgsParser