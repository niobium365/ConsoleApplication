#pragma once

#include "test_task.hpp"
#include <string>
#include <vector>
#include <iostream>
#include <any>
#include <map>
#include <functional>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

// map<typeid => 反序列化实现函数>
inline std::map<size_t, std::function<json::any(rapidjson::Value const&)>> s_map_deserialization;

// 反序列化实现函数
// 用RapidJson构造不同的T
template <typename T>
T f_ctor(id__<T> t, rapidjson::Value const& a);


inline std::string f_ctor_impl(id__<std::string>, rapidjson::Value const& a)
{
	return a.GetString();
}

inline int f_ctor_impl(id__<int>, rapidjson::Value const& a)
{
	return a.GetInt();
}

inline std::string f_ctor_impl(id__<const char*>, rapidjson::Value const& a)
{
	return a.GetString();
}

inline size_t f_ctor_impl(id__<size_t>, rapidjson::Value const& a)
{
	return a.GetUint64();
}

// 构造json::any
inline json::any f_ctor_impl(id__<json::any>, rapidjson::Value const& a)
{
	if (!a.HasMember("any_data"))
	{
		throw std::runtime_error("Unmatched any_data");
	}

	auto&& v = a["any_data"];
	if (v.IsNull())
	{
		// 可能为空值构造
		return {};
	}

	// 用真实的反序列化函数构造
	auto k = v["tag_type"].GetUint64();

	auto it = s_map_deserialization.find(k);
	if (it == s_map_deserialization.end())
	{
		char buf[512];
		sprintf(buf, "Please register deserialization function for typeid_hash=%zd", k);
		throw std::runtime_error(buf);
	}

	return it->second(v);
}

inline Friend f_ctor_impl(id__<Friend>, rapidjson::Value const& a)
{
	assert(a.IsObject());
	return { f_ctor(id__<std::string>{}, a["relation"]), f_ctor(id__<json::any>{}, a["secret"]) };
}

inline Singer f_ctor_impl(id__<Singer>, rapidjson::Value const& a)
{
	assert(a.IsObject());
	return {
		f_ctor(id__<std::string>{}, a["type"]),
		f_ctor(id__<int>{}, a["age"])
	};
}

inline Person f_ctor_impl(id__<Person>, rapidjson::Value const& a)
{
	assert(a.IsObject());
	return { 
			f_ctor(id__<std::string>{}, a["name"]), 
			f_ctor(id__<int>{}, a["age"]),
			f_ctor(id__<Address>{}, a["address"]),
			f_ctor(id__<std::vector<Friend>>{}, a["_friends"]),
			f_ctor(id__<json::any>{}, a["secret"]) 
	};
}

inline Address f_ctor_impl(id__<Address>, rapidjson::Value const& a)
{
	assert(a.IsObject());
	return {
		f_ctor(id__<std::string>{}, a["country"]),
		f_ctor(id__<std::string>{}, a["city"]),
		f_ctor(id__<std::string>{}, a["street"]),
		f_ctor(id__<std::vector<Person>>{}, a["neighbors"]),
	};
}

template <typename T>
std::vector<T> f_ctor_impl(id__<std::vector<T>>, rapidjson::Value const& a)
{
	assert(a.IsArray());

	std::vector<T> r;
	for (rapidjson::Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr)
	{
		r.push_back(f_ctor(id__<T>{}, * itr));
	}
	return r;
}

template <typename T>
T f_ctor(id__<T> t, rapidjson::Value const& a)
{
	return f_ctor_impl(t, a["tag_data"]);
}

inline auto f_parse(rapidjson::Value const& a)
{
	size_t k = a["tag_type"].GetUint64();

	auto it = s_map_deserialization.find(k);
	if (it == s_map_deserialization.end())
	{
		char buf[512];
		sprintf(buf, "Please register deserialization function for typeid_hash=%zd", k);
		throw std::runtime_error(buf);
	}

	return it->second(a);
}


template <typename T>
void register_deserialization()
{
	s_map_deserialization[typeid(T).hash_code()] = [](auto&& v)
	{
		return f_ctor(id__<T>{}, v);
	};
}

inline auto parse(std::string const& a)
{
	rapidjson::Document document;
	document.Parse(a.c_str());

	return f_parse(document);
}

