// Copyright (c) Nicolas VENTER All rights reserved.

#pragma once

#include <functional>
#include <sstream>
#include <string>
#include <vector>

#define OSTREAM_FUNC_PARAM std::ostream& os
// parameters are catpures
#define OSTREAM_FUNC_LAMBDA(...) [__VA_ARGS__](OSTREAM_FUNC_PARAM) -> std::ostream&

class OstreamFunc : public std::function<std::ostream&(OSTREAM_FUNC_PARAM)>
{
	using parentType = std::function<std::ostream&(OSTREAM_FUNC_PARAM)>;

public:
	using parentType::parentType;

	friend std::ostream& operator<<(std::ostream& os, const OstreamFunc& ostreamFunc);
	std::string toString();
};

static OstreamFunc defaultOstreamFunc = OSTREAM_FUNC_LAMBDA() { return os; };

struct ArrayConverter
{
	explicit ArrayConverter(const OstreamFunc& ostreamFunc_ = defaultOstreamFunc,
		const std::string& separator_ = ", ",
		const std::string& prefix_ = "[",
		const std::string& suffix_ = "]");

	template <typename T> std::string toString(const std::vector<T>& tList) const
	{
		std::ostringstream oss;
		ostreamFunc(oss << prefix) << *tList.begin();
		auto it = tList.begin();
		for (++it; it != tList.end(); ++it) ostreamFunc(oss << separator) << *it;
		oss << suffix;
		return oss.str();
	}

	OstreamFunc ostreamFunc;
	std::string separator;
	std::string prefix;
	std::string suffix;
};

// ============================================================
// ============================================================
// ===================== Implementations ======================
// ============================================================
// ============================================================

#ifdef DISPLAYER_IMPLEMENTATION

std::ostream& operator<<(std::ostream& os, const OstreamFunc& ostreamFunc) { return ostreamFunc(os); }

std::string OstreamFunc::toString()
{
	std::ostringstream oss;
	(*this)(oss);
	return oss.str();
}

ArrayConverter::ArrayConverter(
	const OstreamFunc& ostreamFunc_, const std::string& separator_, const std::string& prefix_, const std::string& suffix_) :
	ostreamFunc(ostreamFunc_),
	separator(separator_), prefix(prefix_), suffix(suffix_)
{
}

#endif // DISPLAYER_IMPLEMENTATION
