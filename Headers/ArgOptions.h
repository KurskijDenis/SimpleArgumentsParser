#pragma once

namespace SimpleArgsParser
{

struct ArgOptions
{
public:
	ArgOptions& SetRequired();

public:
	bool required = false;
};

} // namespace SimpleArgsParser