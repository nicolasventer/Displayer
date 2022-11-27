// Copyright (c) Nicolas VENTER All rights reserved.

#pragma once

#include "../Displayer.hpp"

class ExtraDisplayer : public Displayer
{
public:
	// simplified constructor with std::vector<std::string>
	// to use like this: ExtraDisplayer(SL{"myStr1", "myStr2"})
	explicit ExtraDisplayer(const SL& keyList);

protected:
	ExtraDisplayer() = default;

public:
	// to use in order to change the categories display
	DisplayFuncMap headerDisplayFuncMap;
};

// ============================================================
// ============================================================
// ===================== Implementations ======================
// ============================================================
// ============================================================

#ifdef DISPLAYER_IMPLEMENTATION

ExtraDisplayer::ExtraDisplayer(const SL& keyList)
{
	for (const auto& key : keyList)
	{
		push_back(key);
		if (displayer::globalDisplayFuncMap.count(key)) continue;
		headerDisplayFuncMap.emplace(key, DisplayFunc(key));
	}
}

#endif // DISPLAYER_IMPLEMENTATION
