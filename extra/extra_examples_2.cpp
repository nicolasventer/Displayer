#include <iostream>

#define DISPLAYER_IMPLEMENTATION
#include "BoxDisplayer.hpp"
#include "CsvDisplayer.hpp"
#include "JsonDisplayer.hpp"

static const struct
{
	std::string name = "name";
	std::string age = "age";
	std::string money = "money";
	std::string canDrive = "can drive";
	std::string phoneNumber = "phone number";
} PersonKeys;

struct Person
{
	std::string name;
	uint32_t age;
	uint32_t money;
	bool bCanDrive;
	std::vector<int> phoneNumber;

	DisplayFuncMap toDisplayFuncMap() const
	{
		return DisplayFuncMap(SPL{
			{PersonKeys.name, name},
			{PersonKeys.age, std::to_string(age)},
			{PersonKeys.money, std::to_string(money)},
			{PersonKeys.canDrive, bool_to_string(bCanDrive)},
			{PersonKeys.phoneNumber, s_getPhoneNumberAC().toString(phoneNumber)},
		});
	}

	// phone number array converter
	static const ArrayConverter& s_getPhoneNumberAC()
	{
		static ArrayConverter phoneNumberArrayConverter = []()
		{
			ArrayConverter result;
			result.ostreamFunc = OSTREAM_FUNC_LAMBDA() { return os << std::setfill('0') << std::setw(2); };
			result.prefix = "";
			result.suffix = "";
			result.separator = " ";
			return result;
		}();
		return phoneNumberArrayConverter;
	}
};

int main()
{
	using namespace displayer;

	std::vector<Person> personList{
		{"Craig", 25, 250, true, {06, 12, 20, 88, 14}},
		{"John", 17, 0, false, {06, 22, 14, 75, 77}},
		{"Paula", 53, 1930, true, {06, 45, 32, 98, 64}},
	};
	std::vector<DisplayFuncMap> displayFuncMapList;
	for (const auto& person : personList) displayFuncMapList.push_back(person.toDisplayFuncMap());

	SL sl{
		left_,
		setw_(10),
		PersonKeys.name,
		setw_(18),
		PersonKeys.phoneNumber,
		setw_(5),
		PersonKeys.age,
		right_,
		setw_(10),
		PersonKeys.money,
		setw_(12),
		PersonKeys.canDrive,
	};

	std::cout << "--- Example 6 ---" << std::endl;
	BoxDisplayer boxDisplayer(sl, BorderPreset::ALL);

	std::cout << boxDisplayer.displayHeader() << std::endl;
	for (uint32_t i = 0; i < personList.size() - 1; ++i)
		std::cout << boxDisplayer.display(personList[i].toDisplayFuncMap(), false) << std::endl;
	std::cout << boxDisplayer.display(personList.back().toDisplayFuncMap(), true) << std::endl;

	std::cout << "--- Example 7 ---" << std::endl;
	CsvDisplayer csvDisplayer(sl);

	std::cout << csvDisplayer.display(csvDisplayer.headerDisplayFuncMap) << std::endl;
	for (const auto& displayFuncMap : displayFuncMapList) std::cout << csvDisplayer.display(displayFuncMap) << std::endl;

	std::cout << "--- Example 8 ---" << std::endl;
	JsonDisplayer jsonDisplayer(sl, " ", ""); // no newline display

	jsonDisplayer.setKeyAsString(PersonKeys.name);
	for (const auto& person : personList) std::cout << jsonDisplayer.display(person.toDisplayFuncMap()) << std::endl;
}