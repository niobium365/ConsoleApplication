#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include "rapidjson/document.h"

#define F__(x) std::forward<decltype(x)>(x)
#define M__(x) std::move(x)

decltype(std::declval<rapidjson::Document>().GetAllocator()) f_alloc();

