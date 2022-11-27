// Copyright (c) Nicolas VENTER All rights reserved.

#pragma once

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "ArrayConverter.hpp"

#define DISPLAY_FUNC_PARAM std::ostream& os
// parameters are catpures
#define DISPLAY_FUNC_LAMBDA(...) [__VA_ARGS__](DISPLAY_FUNC_PARAM)

class DisplayFunc : public std::function<void(DISPLAY_FUNC_PARAM)>
{
	using parentType = std::function<void(DISPLAY_FUNC_PARAM)>;

public:
	using parentType::parentType;

	// simplified constructor with string
	// to use like this: DisplayFunc("myStr")
	explicit DisplayFunc(const std::string& s);

	std::string toString() const;
};

// shortcut to use in simplified constructor
using SL = std::vector<std::string>;						  // stringList
using SPL = std::vector<std::pair<std::string, std::string>>; // stringPairList

// object to display
class DisplayFuncMap : public std::unordered_map<std::string, DisplayFunc>
{
	using parentType = std::unordered_map<std::string, DisplayFunc>;

public:
	using parentType::parentType;

	// simplified constructor with std::vector<std::string>
	// to use like this: DisplayFuncMap(SL{"myStr1", "myStr2"})
	explicit DisplayFuncMap(const SL& keyList);

	// simplified constructor with std::vector<std::string>
	// to use like this: DisplayFuncMap(SL{{"myKey1", "myValue1"}, {"myKey2", "myValue2"}})
	explicit DisplayFuncMap(const SPL& keyValueList);

	// simplified find which return nullptr if key not found
	const DisplayFunc* pFind(const std::string& key) const;
};

// EDF = Extension Display Func
#define EDF_PARAM std::ostream &os, DisplayFunc displayFunc
// parameters are catpures
// the displayFunc param is the displayFunc retrieved from the object
#define EDF_LAMBDA(...) [__VA_ARGS__](EDF_PARAM)

// EDF = Extension Display Func
// the displayFunc param is the displayFunc retrieved from the object
class ExtensionDisplayFunc : public std::function<void(EDF_PARAM)>
{
	using parentType = std::function<void(EDF_PARAM)>;

public:
	using parentType::parentType;

	// simplified constructor with string
	// to use like this: ExtensionDisplayFunc(DisplayFunc("myStr"))
	explicit ExtensionDisplayFunc(const DisplayFunc& displayFunc);
};

#define KEY_NOT_FOUND_PARAM std::ostream &os, const std::string &key
// parameters are catpures
#define KEY_NOT_FOUND_LAMBDA(...) [__VA_ARGS__](KEY_NOT_FOUND_PARAM)

using KeyNotFoundCallback = std::function<void(KEY_NOT_FOUND_PARAM)>;

std::string bool_to_string(bool b);

namespace displayer
{
	static const std::string left_ = "left_";
	static const std::string right_ = "right_";
	std::string setw_(long long streamsize);
	std::string setfill_(char c);
	std::string string_(const std::string& s);

	// object used to customize the display of a key not found in the object to display
	static DisplayFuncMap globalDisplayFuncMap{
		{left_, std::left},
		{right_, std::right},
	};

	// object used to extend the display of a key by using the display func stored in object
	static std::unordered_map<std::string, ExtensionDisplayFunc> globalEdfMap;
} // namespace displayer

// class that contains the list of keys to display
class Displayer : public std::vector<std::string>
{
	using parentType = std::vector<std::string>;

private:
	static void defaultKeyNotFound(KEY_NOT_FOUND_PARAM);

public:
	using parentType::parentType;

	// function executed when a key to display is not found, neither in object, nor in displayer::globalDisplayFuncMap
	KeyNotFoundCallback onKeyNotFound = defaultKeyNotFound;

	// function used to display an object
	// to use like this: std::cout << myDisplayer.display(myDisplayFuncMap) << std::endl;
	OstreamFunc display(const DisplayFuncMap& displayFuncMap);

private:
	DisplayFunc getKeyNotFoundDisplayFunc(const std::string& key) const;
};

// ============================================================
// ============================================================
// ===================== Implementations ======================
// ============================================================
// ============================================================

#ifdef DISPLAYER_IMPLEMENTATION

DisplayFunc::DisplayFunc(const std::string& s) : DisplayFunc(DISPLAY_FUNC_LAMBDA(s) { os << s; }) {}

std::string DisplayFunc::toString() const
{
	std::ostringstream oss;
	(*this)(oss);
	return oss.str();
}

DisplayFuncMap::DisplayFuncMap(const SL& keyList)
{
	for (const auto& key : keyList) emplace(key, DisplayFunc(key));
}

DisplayFuncMap::DisplayFuncMap(const SPL& keyValueList)
{
	for (const auto& keyValue : keyValueList) emplace(keyValue.first, DisplayFunc(keyValue.second));
}

const DisplayFunc* DisplayFuncMap::pFind(const std::string& key) const
{
	auto it = find(key);
	return it == end() ? nullptr : &it->second;
}

ExtensionDisplayFunc::ExtensionDisplayFunc(const DisplayFunc& displayFunc_) :
	ExtensionDisplayFunc(EDF_LAMBDA(displayFunc_) { displayFunc_(os); })
{
}

std::string bool_to_string(bool b) { return b ? "true" : "false"; }

namespace displayer
{
	std::string setw_(long long streamsize)
	{
		std::string key = "setw:" + std::to_string(streamsize);
		globalDisplayFuncMap.emplace(
			key, DISPLAY_FUNC_LAMBDA(streamsize) { os << std::setw(streamsize); });
		return key;
	}

	std::string setfill_(char c)
	{
		std::string key = "setfill:" + c;
		globalDisplayFuncMap.emplace(
			key, DISPLAY_FUNC_LAMBDA(c) { os << std::setfill(c); });
		return key;
	}

	std::string string_(const std::string& s)
	{
		std::string key = "string:" + s;
		globalDisplayFuncMap.emplace(key, DisplayFunc(s));
		return key;
	}
} // namespace displayer

void Displayer::defaultKeyNotFound(KEY_NOT_FOUND_PARAM) { os << std::string(key + ":???"); }

OstreamFunc Displayer::display(const DisplayFuncMap& displayFuncMap)
{
	return OSTREAM_FUNC_LAMBDA(this, &displayFuncMap)
	{
		for (const auto& key : *this)
		{
			auto displayFunc = displayFuncMap.pFind(key);
			auto it = displayer::globalEdfMap.find(key);
			if (it != displayer::globalEdfMap.end()) it->second(os, displayFunc ? *displayFunc : getKeyNotFoundDisplayFunc(key));
			else if (displayFunc)
				(*displayFunc)(os);
			else if (auto globalDisplayFunc = displayer::globalDisplayFuncMap.pFind(key))
				(*globalDisplayFunc)(os);
			else
				onKeyNotFound(os, key);
		}
		return os;
	};
}

DisplayFunc Displayer::getKeyNotFoundDisplayFunc(const std::string& key) const
{
	return DISPLAY_FUNC_LAMBDA(this, key) { onKeyNotFound(os, key); };
}

#endif // DISPLAYER_IMPLEMENTATION
