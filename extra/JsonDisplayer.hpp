// Copyright (c) Nicolas VENTER All rights reserved.

#pragma once

#include <algorithm>
#include <unordered_set>

#include "../Displayer.hpp"

class JsonDisplayer : public Displayer
{
public:
	// use to set the key as string (and then to put it in quotes)
	static std::string string_(const std::string& key);

private:
	static std::unordered_set<std::string>& s_tmpStringKeySet();

public:
	// simplified constructor with std::vector<std::string>
	// to use like this: JsonDisplayer(SL{"myStr1", "myStr2"})
	explicit JsonDisplayer(const SL& keyList, const std::string& newline = "\n", const std::string& tab = "\t");
	explicit JsonDisplayer(const SL& keyList);

	void setKeyAsString(const std::string& key);
	void unsetKeyAsString(const std::string& key);

	void setStringKeySet(const std::unordered_set<std::string>& newStringKeySet);

	const std::unordered_set<std::string>& getStringKeySet() const;

	// to use in order to construct a copy
	// to use like this: JsonDisplayer(oldJsonDisplayer.getBaseKeyList(), newNewline, newNewtab)
	const std::vector<std::string>& getBaseKeyList() const;

public:
	DisplayFuncMap headerDisplayFuncMap;

private:
	std::vector<std::string> baseKeyList;
	std::unordered_set<std::string> stringKeySet; // set of key that are strings
};

// ============================================================
// ============================================================
// ===================== Implementations ======================
// ============================================================
// ============================================================

#ifdef DISPLAYER_IMPLEMENTATION

std::string JsonDisplayer::string_(const std::string& key)
{
	s_tmpStringKeySet().insert(key);
	return key;
}

std::unordered_set<std::string>& JsonDisplayer::s_tmpStringKeySet()
{
	static std::unordered_set<std::string> tmpStringKeySet; // automatically cleared on JsonDisplayer construction
	return tmpStringKeySet;
}

JsonDisplayer::JsonDisplayer(const SL& keyList) : JsonDisplayer(keyList, "\n", "\t") {}

JsonDisplayer::JsonDisplayer(const SL& keyList, const std::string& newline, const std::string& tab)
{
	push_back(displayer::string_("{"));
	for (const auto& key : keyList)
	{
		baseKeyList.push_back(key);
		if (displayer::globalDisplayFuncMap.count(key)) continue;
		headerDisplayFuncMap.emplace(key, DisplayFunc(key));
		push_back(displayer::string_(newline + tab + "\"" + key + "\": "));
		if (stringKeySet.count(key))
		{
			push_back(displayer::string_("\""));
			push_back(key);
			push_back(displayer::string_("\","));
		}
		else
		{
			push_back(key);
			push_back(displayer::string_(","));
		}
	}
	if (!keyList.empty())
	{
		pop_back();
		if (stringKeySet.count(keyList.back())) push_back(displayer::string_("\""));
	}
	push_back(displayer::string_(newline + "}"));
	stringKeySet = s_tmpStringKeySet();
	s_tmpStringKeySet().clear();
}

void JsonDisplayer::setKeyAsString(const std::string& key)
{
	auto it = begin();
	it = std::find(it, end(), key);
	while (it != end())
	{
		it = insert(it, displayer::string_("\""));
		++it;
		++it;
		*it = displayer::string_("\",");
		it = std::find(it, end(), key);
	}
}

void JsonDisplayer::unsetKeyAsString(const std::string& key)
{
	auto it = begin();
	it = std::find(it, end(), key);
	while (it != end())
	{
		it = erase(std::prev(it));
		*it = displayer::string_(",");
		it = std::find(it, end(), key);
	}
}

void JsonDisplayer::setStringKeySet(const std::unordered_set<std::string>& newStringKeySet)
{
	std::unordered_set<std::string> toSetAsStringSet;
	std::set_difference(stringKeySet.begin(),
		stringKeySet.end(),
		newStringKeySet.begin(),
		newStringKeySet.end(),
		std::inserter(toSetAsStringSet, toSetAsStringSet.end()));
	for (const auto& toSetAsString : toSetAsStringSet) setKeyAsString(toSetAsString);
	std::unordered_set<std::string> toUnsetAsStringSet;
	std::set_difference(stringKeySet.begin(),
		stringKeySet.end(),
		newStringKeySet.begin(),
		newStringKeySet.end(),
		std::inserter(toUnsetAsStringSet, toUnsetAsStringSet.end()));
	for (const auto& toUnsetAsString : toUnsetAsStringSet) unsetKeyAsString(toUnsetAsString);
	stringKeySet = newStringKeySet;
}

const std::unordered_set<std::string>& JsonDisplayer::getStringKeySet() const { return stringKeySet; }

const std::vector<std::string>& JsonDisplayer::getBaseKeyList() const { return baseKeyList; }

#endif // DISPLAYER_IMPLEMENTATION
