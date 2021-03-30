#pragma once

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "rapidjson/document.h"

#define F__(x) std::forward<decltype(x)>(x)
#define M__(x) std::move(x)

inline rapidjson::Document s_doc_ins;

