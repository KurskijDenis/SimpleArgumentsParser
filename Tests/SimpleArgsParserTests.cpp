#include <ArgsParser.h>
#include <gtest/gtest.h>

namespace SimpleArgsParser
{

TEST(ArgsParser, EmptyArgs)
{
	ArgsInitializer args_initializer;

	const int argc = 1;
	const char* argv1 = "program";
	const char* argv[argc] = { argv1 };

	const auto args = ParseArgs(argc, argv, args_initializer);
	EXPECT_EQ(args.Count(), 1);
}

TEST(ArgsParser, OneSimpleArg)
{
	ArgsInitializer args_initializer;
	args_initializer("arg", "Arg info");

	const int argc = 2;
	const char* argv1 = "program";
	const char* argv2 = "--arg";
	const char* argv[argc] = { argv1, argv2 };

	const auto args = ParseArgs(argc, argv, args_initializer);
	EXPECT_EQ(args.Count(), 2);

	EXPECT_TRUE(args.Exist("-h"));
	EXPECT_TRUE(args.Exist("--help"));

	EXPECT_TRUE(args.Exist("--arg"));
}

TEST(ArgsParser, OneArgWithValue)
{
	ArgsInitializer args_initializer;
	args_initializer("arg", "Arg info", ArgValue<int>());

	const int argc = 3;
	const char* argv1 = "program";
	const char* argv2 = "--arg";
	const char* argv3 = "-5";
	const char* argv[argc] = { argv1, argv2, argv3 };

	const auto args = ParseArgs(argc, argv, args_initializer);
	EXPECT_EQ(args.Count(), 2);

	EXPECT_TRUE(args.Exist("-h"));
	EXPECT_TRUE(args.Exist("--help"));

	EXPECT_TRUE(args.Exist("--arg"));
	EXPECT_EQ(args.GetValue<int>("--arg"), -5);
}

TEST(ArgsParser, OneArgWithDefaultAndConsoleValue)
{
	ArgsInitializer args_initializer;
	args_initializer("arg", "Arg info", ArgValue<int>().SetDefault(34));

	const int argc = 3;
	const char* argv1 = "program";
	const char* argv2 = "--arg";
	const char* argv3 = "-5";
	const char* argv[argc] = { argv1, argv2, argv3 };

	const auto args = ParseArgs(argc, argv, args_initializer);
	EXPECT_EQ(args.Count(), 2);

	EXPECT_TRUE(args.Exist("-h"));
	EXPECT_TRUE(args.Exist("--help"));

	EXPECT_TRUE(args.Exist("--arg"));
	EXPECT_EQ(args.GetValue<int>("--arg"), -5);
}

TEST(ArgsParser, OneArgWithDefaultValueOnly)
{
	ArgsInitializer args_initializer;
	args_initializer("arg", "Arg info", ArgValue<int>().SetDefault(34));

	const int argc = 1;
	const char* argv1 = "program";
	const char* argv[argc] = { argv1};

	const auto args = ParseArgs(argc, argv, args_initializer);

	EXPECT_TRUE(args.Exist("-h"));
	EXPECT_TRUE(args.Exist("--help"));

	EXPECT_EQ(args.Count(), 2);
	EXPECT_TRUE(args.Exist("--arg"));
	EXPECT_EQ(args.GetValue<int>("--arg"), 34);
}

TEST(ArgsParser, OneArgWithDefaultAndConsoleShortValue)
{
	ArgsInitializer args_initializer;
	args_initializer("arg, a", "Arg info", ArgValue<int>().SetDefault(34));

	const int argc = 3;
	const char* argv1 = "program";
	const char* argv2 = "-a";
	const char* argv3 = "-5";
	const char* argv[argc] = { argv1, argv2, argv3 };

	const auto args = ParseArgs(argc, argv, args_initializer);
	EXPECT_EQ(args.Count(), 2);

	EXPECT_TRUE(args.Exist("-h"));
	EXPECT_TRUE(args.Exist("--help"));

	EXPECT_TRUE(args.Exist("-a"));
	EXPECT_TRUE(args.Exist("--arg"));

	EXPECT_EQ(args.GetValue<int>("-a"), -5);
	EXPECT_EQ(args.GetValue<int>("--arg"), -5);
}

TEST(ArgsParser, TestSeveralArgs)
{
	ArgsInitializer args_initializer;
	args_initializer("arg1, a1", " Arg info1", ArgValue<int>().SetDefault(34))
	                ("arg2", "Arg info2")
	                ("arg3, a3", "Arg info3", ArgValue<std::string>(), ArgOptions().SetRequired())
	                ("arg4, a4", "Arg info4", ArgValue<double>());

	const int argc = 6;
	const char* argv1 = "program";
	const char* argv2 = "--arg2";
	const char* argv3 = "--arg3";
	const char* argv4 = "hello";
	const char* argv5 = "-a4";
	const char* argv6 = "5.67";
	const char* argv[argc] = { argv1, argv2, argv3, argv4, argv5, argv6 };

	const auto args = ParseArgs(argc, argv, args_initializer);
	EXPECT_EQ(args.Count(), 5);

	EXPECT_TRUE(args.Exist("-h"));
	EXPECT_TRUE(args.Exist("--help"));

	EXPECT_TRUE(args.Exist("-a1"));
	EXPECT_TRUE(args.Exist("--arg1"));
	EXPECT_EQ(args.GetValue<int>("-a1"), 34);
	EXPECT_EQ(args.GetValue<int>("--arg1"), 34);

	EXPECT_TRUE(args.Exist("--arg2"));

	EXPECT_TRUE(args.Exist("-a3"));
	EXPECT_TRUE(args.Exist("--arg3"));
	EXPECT_EQ(args.GetValue<std::string>("-a3"), "hello");
	EXPECT_EQ(args.GetValue<std::string>("--arg3"), "hello");

	EXPECT_TRUE(args.Exist("-a4"));
	EXPECT_TRUE(args.Exist("--arg4"));
	EXPECT_EQ(args.GetValue<double>("-a4"), 5.67);
	EXPECT_EQ(args.GetValue<double>("--arg4"), 5.67);
}

TEST(ArgsParser, TestHelp)
{
	ArgsInitializer args_initializer("Program desc.", 15, 20);
	args_initializer("arg1, a1", "3aaa 4bbbb 9ccccccccc 20dddddddddddddddddddd 6vvvvvv", ArgValue<int>().SetDefault(34))
		("arg2", "Arg info2")
		("a3", "20dddddddddddddddddddd 9ccccccccc 6vvvvvv 3aaa 4bbbb", ArgValue<std::string>(), ArgOptions().SetRequired())
		("arg4, a4", "Arg info4", ArgValue<double>());

	const int argc = 3;
	const char* argv1 = "program";
	const char* argv2 = "--a3";
	const char* argv3 = "hello";
	const char* argv[argc] = { argv1, argv2, argv3 };

	const std::string expected_string =
		"Usage: program [options]\n"
        "Program desc.\n"
		"Available options:\n"
        "  --a3 arg     20dddddddddddddddddddd\n"
		"               9ccccccccc 6vvvvvv\n"
		"               3aaa 4bbbb\n"
        "  --arg1,-a1 arg(=34)\n"
		"               3aaa 4bbbb\n"
		"               9ccccccccc\n"
		"               20dddddddddddddddddddd\n"
		"               6vvvvvv\n"
		"  --arg2       Arg info2\n"
		"  --arg4,-a4 arg\n"
		"               Arg info4\n";

	const auto args = ParseArgs(argc, argv, args_initializer);
	EXPECT_EQ(args.Count(), 3);
	EXPECT_EQ(expected_string, args.GetValue<std::string>("-h"));
	EXPECT_EQ(expected_string, args.GetValue<std::string>("--help"));
}

TEST(ArgsParser, TestDublicateFullNameArg)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("arg1, a1", "Help1")
			("arg1", "Help1");
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Duplicate full option name --arg1.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestDublicateShortNameArg)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("arg1, a1", "Help1")
			("arg2, a1", "Help1");
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Duplicate short option name -a1.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestIncorrectFullArgName1)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("--arg1, a1", "Help1");
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Incorrect full option name (please remove '-') --arg1, a1.");
		return;
	}
	FAIL();
}
TEST(ArgsParser, TestIncorrectFullArgName2)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("-arg1, a1", "Help1");
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Incorrect full option name (please remove '-') -arg1, a1.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestIncorrectShortArgName1)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("arg1,-a1", "Help1");
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Incorrect short option name (please remove '-') -a1.");
		return;
	}
	FAIL();
}
TEST(ArgsParser, TestEmptyNames)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer(",", "Help1");
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Empty full option name.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestEmptyHelpStringDescSize)
{
	try
	{
		ArgsInitializer args_initializer("", 0, 10);
	}
	catch (const ArgsInitializerException& exc)
	{
		EXPECT_STREQ(exc.what(), "Max string size for argument description must be > 0.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestEmptyHelpStringInfoSize)
{
	try
	{
		ArgsInitializer args_initializer("", 10, 0);
	}
	catch (const ArgsInitializerException& exc)
	{
		EXPECT_STREQ(exc.what(), "Max string size for argument info must be > 0.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestOverrideHelpOptionFullName)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("help", "Help info");
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "--help, -h reserved args.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestOverrideHelpOptionShortName)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("harg, h", "Help info");
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "--help, -h reserved args.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestIncorrectArgcValue)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("arg", "Help info");

		const int argc = 0;
		const char** argv;

		const auto args = ParseArgs(argc, argv, args_initializer);
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Incorrect value of argc param.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestIncorrectArgvValue)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("arg", "Help info");

		const int argc = 1;
		const char** argv = nullptr;

		const auto args = ParseArgs(argc, argv, args_initializer);
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Incorrect value of argv param.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestUnknownParam)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("arg, a", "Help info");

		const int argc = 3;
		const char* argv1 = "program";
		const char* argv2 = "--ar";
		const char* argv3 = "hello";
		const char* argv[argc] = { argv1, argv2, argv3 };

		const auto args = ParseArgs(argc, argv, args_initializer);
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Unknown param: --ar.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestUnknownParamValue)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("arg, a", "Help info", ArgValue<int>());

		const int argc = 2;
		const char* argv1 = "program";
		const char* argv2 = "--arg";
		const char* argv[argc] = { argv1, argv2 };

		const auto args = ParseArgs(argc, argv, args_initializer);
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Please set param value: --arg.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestRequaredParamNotSet)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("arg, a", "Help info", ArgValue<int>(), ArgOptions().SetRequired());

		const int argc = 1;
		const char* argv1 = "program";
		const char* argv[argc] = { argv1 };

		const auto args = ParseArgs(argc, argv, args_initializer);
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Please set required param --arg.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestGetValueEmptyParam)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("arg, a", "Help info");

		const int argc = 1;
		const char* argv1 = "program";
		const char* argv[argc] = { argv1 };

		const auto args = ParseArgs(argc, argv, args_initializer);
		args.GetValue<int>("--arg");
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Value not set.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestGetValueUnsetParam)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("arg, a", "Help info", ArgValue<int>());

		const int argc = 1;
		const char* argv1 = "program";
		const char* argv[argc] = { argv1 };

		const auto args = ParseArgs(argc, argv, args_initializer);
		args.GetValue<int>("--arg");
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Value not set.");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestGetValueDifferentTypeParam)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("arg, a", "Help info", ArgValue<int>().SetDefault(23));

		const int argc = 3;
		const char* argv1 = "program";
		const char* argv2 = "--arg";
		const char* argv3 = "32";
		const char* argv[argc] = { argv1, argv2, argv3 };

		const auto args = ParseArgs(argc, argv, args_initializer);
		args.GetValue<double>("--arg");
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "bad any_cast");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestIncorrectTypeInArgsInitializer)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("arg, a", "Help info", ArgValue<int>().SetDefault(23));

		const int argc = 3;
		const char* argv1 = "program";
		const char* argv2 = "--arg";
		const char* argv3 = "abs";
		const char* argv[argc] = { argv1, argv2, argv3 };

		const auto args = ParseArgs(argc, argv, args_initializer);
		args.GetValue<int>("--arg");
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "stoll");
		return;
	}
	FAIL();
}

TEST(ArgsParser, TestOutOfRangeValue)
{
	try
	{
		ArgsInitializer args_initializer;
		args_initializer("arg, a", "Help info", ArgValue<uint8_t>().SetDefault(23));

		const int argc = 3;
		const char* argv1 = "program";
		const char* argv2 = "--arg";
		const char* argv3 = "2000";
		const char* argv[argc] = { argv1, argv2, argv3 };

		const auto args = ParseArgs(argc, argv, args_initializer);
		args.GetValue<uint8_t>("--arg");
	}
	catch (const ArgsParserException& exc)
	{
		EXPECT_STREQ(exc.what(), "Value out of range.");
		return;
	}
	FAIL();
}

} // namespace SimpleArgsParser