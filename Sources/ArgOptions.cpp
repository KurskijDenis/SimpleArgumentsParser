#include "../Headers/ArgOptions.h"

namespace SimpleArgsParser
{

ArgOptions& ArgOptions::SetRequired()
{
	required = true;
	return *this;
}

} // namespace SimpleArgsParser