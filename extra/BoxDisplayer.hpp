// Copyright (c) Nicolas VENTER All rights reserved.

#pragma once

#include "../Displayer.hpp"

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

class BoxDisplayer : public Displayer
{
public:
	// simplified constructor with std::vector<std::string>
	// to use like this: BoxDisplayer(SL{"myStr1", "myStr2"}, BorderPreset::ALL ^ BorderFlag::V_SPLIT)
	explicit BoxDisplayer(const SL& keyList, BorderPreset borderPreset = BorderPreset::DEFAULT);
	BoxDisplayer(const SL& keyList, BorderFlag borderFlag);
	explicit BoxDisplayer(const SL& keyList, uint8_t borderType_);

	OstreamFunc displayHeader();
	OstreamFunc display(const DisplayFuncMap& displayFuncMap, bool isLast);

	// to use in order to change the categories display
	void setHeaderDisplayFuncMap(const DisplayFuncMap& headerDisplayFuncMap_);

	const DisplayFuncMap& getHeaderDisplayFuncMap() const;

	// to use in order to construct a copy
	// to use like this: BoxDisplayer(oldBoxDisplayer.getBaseKeyList(), newBorderType)
	const std::vector<std::string>& getBaseKeyList() const;

protected:
	using Displayer::display;

private:
	std::string headerStr;
	std::string lineStr;
	std::string splitStr;
	uint8_t borderType;

	std::vector<std::string> baseKeyList;
	DisplayFuncMap headerDisplayFuncMap;
};

// ============================================================
// ============================================================
// ===================== Implementations ======================
// ============================================================
// ============================================================

#ifdef DISPLAYER_IMPLEMENTATION

BoxDisplayer::BoxDisplayer(const SL& keyList, BorderPreset borderPreset) :
	BoxDisplayer(keyList, static_cast<uint8_t>(borderPreset))
{
}

BoxDisplayer::BoxDisplayer(const SL& keyList, BorderFlag borderFlag) : BoxDisplayer(keyList, static_cast<uint8_t>(borderFlag)) {}

BoxDisplayer::BoxDisplayer(const SL& keyList, uint8_t borderType_) : borderType(borderType_)
{
	using namespace displayer;
	if (borderType & BorderFlag::LEFT) push_back(string_("| "));
	DisplayFuncMap headerDisplayFuncMap_;
	uint32_t lastIndex = keyList.size() - 1;
	bool isFirstCol = true;
	for (uint32_t i = 0; i < keyList.size(); ++i)
	{
		auto& key = keyList[i];
		push_back(key);
		baseKeyList.push_back(key);
		if (globalDisplayFuncMap.count(key)) continue;
		headerDisplayFuncMap_.emplace(key, DisplayFunc(key));

		if (i == lastIndex) continue;
		bool bVsplit = borderType & BorderFlag::V_SPLIT;
		bool bFirstCol = borderType & BorderFlag::FIRST_COL;
		if (isFirstCol && bFirstCol) push_back(string_(" || "));
		else if (bVsplit)
			push_back(string_(" | "));
		isFirstCol = false;
	}
	if (borderType & BorderFlag::RIGHT) push_back(string_(" |"));
	setHeaderDisplayFuncMap(headerDisplayFuncMap_);
}

OstreamFunc BoxDisplayer::displayHeader()
{
	return OSTREAM_FUNC_LAMBDA(this)
	{
		if (borderType & BorderFlag::TOP) os << lineStr.substr(1) << "\n";
		os << headerStr.substr(1);
		if (borderType & BorderFlag::FIRST_LINE)
		{
			std::string headerSplitStr = splitStr;
			std::replace_if(
				headerSplitStr.begin(), headerSplitStr.end(), [](char c) { return c != '|'; }, '=');
			headerSplitStr[0] = '\n';
			os << headerSplitStr;
		}
		else if (borderType & BorderFlag::H_SPLIT)
			os << splitStr;
		return os;
	};
}

OstreamFunc BoxDisplayer::display(const DisplayFuncMap& displayFuncMap, bool isLast)
{
	return OSTREAM_FUNC_LAMBDA(this, &displayFuncMap, isLast)
	{
		Displayer::display(displayFuncMap)(os);
		if (isLast)
		{
			if (borderType & BorderFlag::BOTTOM) os << lineStr;
		}
		else
		{
			if (borderType & BorderFlag::H_SPLIT) os << splitStr;
		}
		return os;
	};
}

void BoxDisplayer::setHeaderDisplayFuncMap(const DisplayFuncMap& headerDisplayFuncMap_)
{
	headerDisplayFuncMap = headerDisplayFuncMap_;
	headerStr = display(headerDisplayFuncMap).toString();
	splitStr = headerStr;
	std::replace_if(
		splitStr.begin(), splitStr.end(), [](char c) { return c != '|'; }, '-');
	lineStr = std::string(headerStr.size(), '-');
	headerStr = "\n" + headerStr;
	splitStr = "\n" + splitStr;
	lineStr = "\n" + lineStr;
}

const DisplayFuncMap& BoxDisplayer::getHeaderDisplayFuncMap() const { return headerDisplayFuncMap; }

const std::vector<std::string>& BoxDisplayer::getBaseKeyList() const { return baseKeyList; }

#endif // DISPLAYER_IMPLEMENTATION
