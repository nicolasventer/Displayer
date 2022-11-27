#include <iostream>

#define DISPLAYER_IMPLEMENTATION
#include "ExtraDisplayer.hpp"

static const struct
{
	std::string name = "name";
	std::string age = "age";
	std::string money = "money";
	std::string canDrive = "can drive";
	std::string phoneNumber = "phone number";
} PersonKeys;

static std::string dec_ = []()
{
	std::string key = "dec_";
	displayer::globalDisplayFuncMap.emplace(key, std::dec);
	return key;
}();

static std::string hex_ = []()
{
	std::string key = "hex_";
	displayer::globalDisplayFuncMap.emplace(key, std::hex);
	return key;
}();

static std::string toupper_(const std::string& key)
{
	displayer::globalEdfMap.emplace(
		key, EDF_LAMBDA() {
			std::string s = displayFunc.toString();
			std::transform(s.begin(), s.end(), s.begin(), toupper);
			os << s;
		});
	return key;
}

int main()
{
	std::cout << "--- Example 1 ---" << std::endl;
	ExtraDisplayer extraDisplayer(SL{
		PersonKeys.name,
		displayer::string_(" "),
		PersonKeys.age,
	});

	std::cout << extraDisplayer.display(extraDisplayer.headerDisplayFuncMap) << std::endl;
	std::cout << extraDisplayer.display(DisplayFuncMap(SPL{{PersonKeys.name, "Patrick"}, {PersonKeys.age, "23"}})) << std::endl;

	std::cout << "--- Example 2 ---" << std::endl;
	std::string myTitles = extraDisplayer.display(extraDisplayer.headerDisplayFuncMap).toString();
	std::cout << "titles: " << myTitles << std::endl;

	std::cout << "--- Example 3 ---" << std::endl;
	using namespace displayer;
	extraDisplayer = ExtraDisplayer(SL{
		left_,
		setw_(15),
		PersonKeys.name,
		PersonKeys.age,
	});

	std::cout << extraDisplayer.display(extraDisplayer.headerDisplayFuncMap) << std::endl;
	std::cout << extraDisplayer.display(DisplayFuncMap(SPL{{PersonKeys.name, "Patrick"}, {PersonKeys.age, "23"}})) << std::endl;

	std::cout << "--- Example 4 ---" << std::endl;
	extraDisplayer = ExtraDisplayer(SL{
		left_,
		dec_,
		string_("dec "),
		setw_(5),
		PersonKeys.age,
		hex_,
		string_(" hex "),
		PersonKeys.age,
	});

	auto ageLambda = DISPLAY_FUNC_LAMBDA() { os << 28; };
	std::cout << extraDisplayer.display(extraDisplayer.headerDisplayFuncMap) << std::endl;
	std::cout << extraDisplayer.display(DisplayFuncMap{{PersonKeys.age, ageLambda}}) << std::endl;

	std::cout << "--- Example 5 ---" << std::endl;
	extraDisplayer = ExtraDisplayer(SL{
		left_,
		setw_(15),
		toupper_(PersonKeys.name),
		PersonKeys.age,
	});

	std::cout << extraDisplayer.display(extraDisplayer.headerDisplayFuncMap) << std::endl;
	std::cout << extraDisplayer.display(DisplayFuncMap(SPL{{PersonKeys.name, "Patrick"}, {PersonKeys.age, "23"}})) << std::endl;
}
