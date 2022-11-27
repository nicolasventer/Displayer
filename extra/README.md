# Extra Displayers


`left_`, `right_`, `setw_` and `setfill_` use the STL corresponding functions.

`string_` displays the specified string.

## Extra Diplayer

Extra Diplayer is the base Displayer with `headerDisplayFuncMap` attribute. This attribute is used to display the header of the table.

*Example 1:*
```cpp
ExtraDisplayer extraDisplayer(SL{
	PersonKeys.name,
	displayer::string_(" "),
	PersonKeys.age,
});

std::cout << extraDisplayer.display(extraDisplayer.headerDisplayFuncMap) << std::endl;
std::cout << extraDisplayer.display(DisplayFuncMap(SPL{{PersonKeys.name, "Patrick"}, {PersonKeys.age, "23"}})) << std::endl;
```

Output:
```
name age
Patrick 23
```

Get a string from the call of `display` by using `toString`.

*Example 2:*
```cpp
std::string myTitles = extraDisplayer.display(extraDisplayer.headerDisplayFuncMap).toString();
std::cout << "titles: " << myTitles << std::endl;
```

Output:
```
titles: name age
```

The corresponding STL functions like `left_` or `setw` are also used in title.

*Example 3:*

```cpp
extraDisplayer = ExtraDisplayer(SL{
	left_,
	setw_(15),
	PersonKeys.name,
	PersonKeys.age,
});

std::cout << extraDisplayer.display(extraDisplayer.headerDisplayFuncMap) << std::endl;
std::cout << extraDisplayer.display(DisplayFuncMap(SPL{{PersonKeys.name, "Patrick"}, {PersonKeys.age, "23"}})) << std::endl;
```

Output:
```
name           age
Patrick        23
```


Get a string from `DisplayFunc` by using `toString` like this: `std::string s = displayFunc.toString();`.

<details><summary><b>How to create a custom key</b></summary>

There are 2 types of custom key:

- **DisplayFunc key: key that only modify the stream**

*Example 4:*

```cpp
// out of main
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
// in main...
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
```

Output:
```
dec age   hex age
dec 28    hex 1c
```

*Example 5:*

- **ExtensionDisplayFunc key: key that modify the display of the value associated to the key**

```cpp
// out of main
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
// in main
extraDisplayer = ExtraDisplayer(SL{
	left_,
	setw_(15),
	toupper_(PersonKeys.name),
	PersonKeys.age,
});

std::cout << extraDisplayer.display(extraDisplayer.headerDisplayFuncMap) << std::endl;
std::cout << extraDisplayer.display(DisplayFuncMap(SPL{{PersonKeys.name, "Patrick"}, {PersonKeys.age, "23"}})) << std::endl;
```

Output:
```
NAME           age
PATRICK        23
```

</details>

## Box Displayer

Box Displayer is a displayer that display a box around the table according to the set of `BorderFlag` or the `BorderPreset`.

<details><summary>List of BorderFlag and BorderPreset</summary>

```cpp
enum BorderFlag : uint8_t
{
	TOP = 1 << 0,
	BOTTOM = 1 << 1,
	LEFT = 1 << 2,
	RIGHT = 1 << 3,
	FIRST_LINE = 1 << 4,
	FIRST_COL = 1 << 5,
	H_SPLIT = 1 << 6,
	V_SPLIT = 1 << 7,
};

enum BorderPreset : uint8_t
{
	ALL = uint8_t(-1),
	NONE = 0,
	ALL_BORDERS = TOP | BOTTOM | LEFT | RIGHT,
	ALL_SPLITS = H_SPLIT | V_SPLIT,
	ALL_FIRSTS = FIRST_LINE | FIRST_COL,
	H_BORDERS = TOP | BOTTOM,
	V_BORDERS = LEFT | RIGHT,
	DEFAULT = ALL ^ H_SPLIT ^ FIRST_COL,
};
```

</details>

*Example 6:*

```cpp
BoxDisplayer boxDisplayer(sl, BorderPreset::ALL);

std::cout << boxDisplayer.displayHeader() << std::endl;
for (uint32_t i = 0; i < personList.size() - 1; ++i)
	std::cout << boxDisplayer.display(personList[i].toDisplayFuncMap(), false) << std::endl;
std::cout << boxDisplayer.display(personList.back().toDisplayFuncMap(), true) << std::endl;
```

Output:
```
------------------------------------------------------------------------
| name       || phone number       | age   |      money |    can drive |
|============||====================|=======|============|==============|
| Craig      || 06 12 20 88 14     | 25    |        250 |         true |
|------------||--------------------|-------|------------|--------------|
| John       || 06 22 14 75 77     | 17    |          0 |        false |
|------------||--------------------|-------|------------|--------------|
| Paula      || 06 45 32 98 64     | 53    |       1930 |         true |
------------------------------------------------------------------------
```

To combine BorderFlags, use the `|` operator.  
To remove BorderFlags, use the `^` operator.

Construct another Displayer from a BoxDisplayer by using `BoxDisplayer::getBaseKeyList` as key list *(it is not possible to directly use the BoxDisplayer since keyList is modified)*.

## Csv Displayer

Csv Displayer is a displayer that display a table in csv format.

*Example 7:*

```cpp
CsvDisplayer csvDisplayer(sl);

std::cout << csvDisplayer.display(csvDisplayer.headerDisplayFuncMap) << std::endl;
for (const auto& displayFuncMap : displayFuncMapList) std::cout << csvDisplayer.display(displayFuncMap) << std::endl;
```

Output:
```
name, phone number, age, money, can drive
Craig, 06 12 20 88 14, 25, 250, true
John, 06 22 14 75 77, 17, 0, false
Paula, 06 45 32 98 64, 53, 1930, true
```

As same as BoxDisplayer, construct another Displayer from a CsvDisplayer by using `CsvDisplayer::getBaseKeyList` as key list.

## Json Displayer

Json Displayer is a displayer that display a table in json format.

*Example 8:*

```cpp
JsonDisplayer jsonDisplayer(sl, " ", ""); // no newline display

jsonDisplayer.setKeyAsString(PersonKeys.name);
for (const auto& person : personList) std::cout << jsonDisplayer.display(person.toDisplayFuncMap()) << std::endl;
```

Output:
```
{ "name": "Craig", "phone number": 06 12 20 88 14, "age": 25, "money": 250, "can drive": true }
{ "name": "John", "phone number": 06 22 14 75 77, "age": 17, "money": 0, "can drive": false }
{ "name": "Paula", "phone number": 06 45 32 98 64, "age": 53, "money": 1930, "can drive": true }
```

Set a key as string with `setKeyAsString`.  
Unset a key as string with `unsetKeyAsString`.  
A set of keys can be set as string with `setStringKeySet`.

Be careful, the keys are set as string for all the table.

As same as BoxDisplayer, construct another Displayer from a JsonDisplayer by using `JsonDisplayer::getBaseKeyList` as key list.
