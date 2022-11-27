// Copyright (c) Nicolas VENTER All rights reserved.

#pragma once

#include "../Displayer.hpp"

class CsvDisplayer : public Displayer
{
public:
	// simplified constructor with std::vector<std::string>
	// to use like this: CsvDisplayer(SL{"myStr1", "myStr2"})
	explicit CsvDisplayer(const SL& keyList);

	// to use in order to construct a copy
	// to use like this: CsvDisplayer(oldCsvDisplayer.getBaseKeyList())
	const std::vector<std::string>& getBaseKeyList() const;

public:
	DisplayFuncMap headerDisplayFuncMap;

private:
	std::vector<std::string> baseKeyList;
};

// ============================================================
// ============================================================
// ===================== Implementations ======================
// ============================================================
// ============================================================

#ifdef DISPLAYER_IMPLEMENTATION

CsvDisplayer::CsvDisplayer(const SL& keyList)
{
	using namespace displayer;
	for (const auto& key : keyList)
	{
		baseKeyList.push_back(key);
		if (globalDisplayFuncMap.count(key)) continue;
		push_back(key);
		headerDisplayFuncMap.emplace(key, DisplayFunc(key));
		push_back(string_(", "));
	}
	if (!globalDisplayFuncMap.count(keyList.back())) pop_back();
}

const std::vector<std::string>& CsvDisplayer::getBaseKeyList() const { return baseKeyList; }

#endif // DISPLAYER_IMPLEMENTATION
