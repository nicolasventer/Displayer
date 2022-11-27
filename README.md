# Displayer

# Description

Displayer is a **c++ library** that allows you to **format the display of an object**.

# Features

- `setw`, `left`, `right`, `setfill` functions
- `string` function
- Simplified constructors
- Use of `ostream` and `istream`

## Extra Features

- `BoxDisplayer`
- `JsonDisplayer`
- `CsvDisplayer`

More details on Extra Displayers [here](extra/README.md).

# Installation

Get the file `AllDisplayers.hpp` (generate it with `python AllInOne.py` or download it from the [release](https://github.com/nicolasventer/Cpp-Displayer/releases).

### Header only

Include the file `AllDisplayers.hpp` anywhere you want to use it.  
And above only one include, define `DISPLAYER_IMPLEMENTATION` like this:

```cpp
#define DISPLAYER_IMPLEMENTATION
#include "AllDisplayers.hpp"
```

### Header and source

Include the file `AllDisplayers.hpp` anywhere you want to use it.  
Compile with the file `AllDisplayers.cpp` or with the static build `AllDisplayers.o` available in the [release](https://github.com/nicolasventer/Cpp-Displayer/releases).

### Static build

```bash
g++ -c -O3 AllDisplayers.cpp
```

c++11 or later compilation required.
No external dependencies.

# Example

*Content of [example.cpp](example.cpp):*

```cpp
#include <iostream>

#define DISPLAYER_IMPLEMENTATION
#include "Displayer.hpp"

#include "extra/BoxDisplayer.hpp"
#include "extra/CsvDisplayer.hpp"
#include "extra/ExtraDisplayer.hpp"
#include "extra/JsonDisplayer.hpp"

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
		static ArrayConverter phoneNumberArrayConverter = ArrayConverter(
			OSTREAM_FUNC_LAMBDA() { return os << std::setfill('0') << std::setw(2); }, // ostreamFunc
			" ",																	   // separator
			"",																		   // prefix
			""																		   // suffix
		);
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

	std::cout << "\n=========== BASIC DISPLAY ===========\n" << std::endl;

	ExtraDisplayer extraDisplayer = ExtraDisplayer(sl);
	std::cout << extraDisplayer.display(extraDisplayer.headerDisplayFuncMap) << std::endl;
	for (const auto& person : personList) std::cout << extraDisplayer.display(person.toDisplayFuncMap()) << std::endl;

	std::cout << "\n=========== BOX DISPLAY ===========\n" << std::endl;

	BoxDisplayer boxDisplayer(extraDisplayer);

	std::cout << boxDisplayer.displayHeader() << std::endl;
	auto beforeEnd = std::prev(personList.end());
	for (auto it = personList.begin(); it != beforeEnd; ++it)
		std::cout << boxDisplayer.display(it->toDisplayFuncMap(), false) << std::endl;
	std::cout << boxDisplayer.display(beforeEnd->toDisplayFuncMap(), true) << std::endl;

	std::cout << "\n=========== CSV DISPLAY ===========\n" << std::endl;

	CsvDisplayer csvDisplayer(boxDisplayer.getBaseKeyList());
	std::cout << csvDisplayer.display(csvDisplayer.headerDisplayFuncMap) << std::endl;
	for (const auto& person : personList) std::cout << csvDisplayer.display(person.toDisplayFuncMap()) << std::endl;

	std::cout << "\n=========== JSON DISPLAY ===========\n" << std::endl;

	JsonDisplayer jsonDisplayer(extraDisplayer, " ", ""); // no newline display
	jsonDisplayer.setKeyAsString(PersonKeys.name);
	for (const auto& person : personList) std::cout << jsonDisplayer.display(person.toDisplayFuncMap()) << std::endl;

	return 0;
}
```

Output:
```
=========== BASIC DISPLAY ===========

name      phone number      age       money   can drive
Craig     06 12 20 88 14    25          250        true
John      06 22 14 75 77    17            0       false
Paula     06 45 32 98 64    53         1930        true

=========== BOX DISPLAY ===========

-----------------------------------------------------------------------
| name       | phone number       | age   |      money |    can drive |
|============|====================|=======|============|==============|
| Craig      | 06 12 20 88 14     | 25    |        250 |         true |
| John       | 06 22 14 75 77     | 17    |          0 |        false |
| Paula      | 06 45 32 98 64     | 53    |       1930 |         true |
-----------------------------------------------------------------------

=========== CSV DISPLAY ===========

name, phone number, age, money, can drive
Craig, 06 12 20 88 14, 25, 250, true
John, 06 22 14 75 77, 17, 0, false
Paula, 06 45 32 98 64, 53, 1930, true

=========== JSON DISPLAY ===========

{ "name": "Craig", "phone number": 06 12 20 88 14, "age": 25, "money": 250, "can drive": true }
{ "name": "John", "phone number": 06 22 14 75 77, "age": 17, "money": 0, "can drive": false }
{ "name": "Paula", "phone number": 06 45 32 98 64, "age": 53, "money": 1930, "can drive": true }
```

# Tutorial

## Step 1 : Select the data to display

```cpp
struct Person
{
	std::string name;
	uint32_t age;
	uint32_t money;
	bool bCanDrive;
	std::vector<int> phoneNumber;
}
```

## Step 2 : Select the keys to display

```cpp
static const struct
{
	std::string name = "name";
	std::string age = "age";
	std::string money = "money";
	std::string canDrive = "can drive";
	std::string phoneNumber = "phone number";
} PersonKeys;
```

## Step 3 : Create the conversion member function

```cpp
struct Person
{
	// attributes there...

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
}
```

<details><summary>How to create a DisplayFuncMap</summary>

3 ways:
- using SL *(StringList)*:
	```cpp
	DisplayFuncMap(SL{"myStr1", "myStr2"});
	```
- using SPL *(StringPairList)*:
	```cpp
	DisplayFuncMap(SL{{"myKey1", "myValue1"}, {"myKey2", "myValue2"}})
	```
- using base constructor:
	```cpp
	DisplayFuncMap(SL{{"myKey1", DisplayFunc("myValue1")}, {"myKey2", DisplayFunc("myValue2")}})
	```
	<details><summary>How to create a DisplayFunc</summary>

	3 ways:
	- using string:
		```cpp
		DisplayFunc myDisplayFunc = DisplayFunc("myValue1");
		```
	- using lambda:
		```cpp
		auto myLambda = DISPLAY_FUNC_LAMBDA(myCapture1, &myCapture2) {};
		```
	- using function:
		```cpp
		void myFunc(DISPLAY_FUNC_PARAM) {}
		```
	</details>

</details>

## Step 4 : Create the Displayer

```cpp
ExtraDisplayer extraDisplayer = ExtraDisplayer(SL{
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
});
```

More details on how to create an Extra Displayer [here](extra/README.md).

## Step 5 : Display data

```cpp
// data and displayer creation...
std::cout << extraDisplayer.display(extraDisplayer.headerDisplayFuncMap) << std::endl;
for (const auto& person : personList) std::cout << extraDisplayer.display(person.toDisplayFuncMap()) << std::endl;
```

# Licence

MIT Licence. See [LICENSE file](LICENSE).
Please refer me with:

	Copyright (c) Nicolas VENTER All rights reserved.
