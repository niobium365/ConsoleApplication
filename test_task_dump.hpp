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
#include "rapidjson/prettywriter.h"


inline rapidjson::Document s_doc;

#define V_ADD_MEMBER(a,b,c) a.AddMember(#c, f_mk_v(b.c), s_doc.GetAllocator())

// map<typeid => 序列化实现函数>
inline std::map<size_t, std::function<rapidjson::Value(json::any const&)>>s_map_serialization;

// 高阶函数用来生成实际的序列化函数
inline auto const ff_mk_v = [](auto&& f)
{
	return [f = std::forward<decltype(f)>(f)](auto&& d)
	{
		using v_t = std::decay_t<decltype(d)>;
		rapidjson::Value vJ(rapidjson::kObjectType);
		vJ.AddMember("tag_data", f(d), s_doc.GetAllocator()); //数据
		vJ.AddMember("tag_type", typeid(v_t).hash_code(), s_doc.GetAllocator()); //类型
		return vJ;
	};
};


inline rapidjson::Value f_mk_v(std::string const& v)
{
	return ff_mk_v([](auto const& d)
		{
			return rapidjson::Value(d, s_doc.GetAllocator());
		})(v);
}

inline rapidjson::Value f_mk_v(const char* v)
{
	return ff_mk_v([](auto const& d)
		{
			return rapidjson::Value(d, s_doc.GetAllocator());
		})(v);
}


inline rapidjson::Value f_mk_v(int const& v)
{
	return ff_mk_v([](auto const& d)
		{
			return std::move(rapidjson::Value().SetInt(d));
		})(v);
}

inline rapidjson::Value f_mk_v(size_t const& v)
{
	return ff_mk_v([](auto const& d)
		{
			return std::move(rapidjson::Value().SetUint(d));
		})(v);
}

// any的序列化函数
inline auto const f_mk_v_any = [](size_t x, json::any const& a)
{
	if (!a.has_value())
	{
		return rapidjson::Value(rapidjson::kNullType);
	}
	auto it = s_map_serialization.find(x);
	if (it == s_map_serialization.end())
	{
		char buf[512];

		sprintf(buf, "Please register serialization function for typeid_hash=%zd", x);
		throw std::runtime_error(buf);
	}

	return it->second(a);
};

template<typename T>
void register_serialization(id__<T>)
{
	s_map_serialization[typeid(T).hash_code()] = [](auto const& anyObj)
	{
		return f_mk_v(std::any_cast<T const&>(anyObj));
	};
}

inline rapidjson::Value f_mk_v(json::any const& v)
{
	return ff_mk_v([](auto const& v)
		{
			rapidjson::Value vData(rapidjson::kObjectType);

			if (v.has_value())
			{
				auto r = v.type().hash_code();
				vData.AddMember("any_data", f_mk_v_any(r, v), s_doc.GetAllocator());
			}
			else
			{
				// 空值
				vData.AddMember("any_data", f_mk_v_any(0, {}), s_doc.GetAllocator());
			}

			return vData;
		})(v);
}

inline void register_serialization(id__<json::any>)
{
	s_map_serialization[typeid(json::any).hash_code()] = [](auto const& anyObj)
	{
		return f_mk_v(anyObj);
	};
}

template<typename T>
rapidjson::Value f_mk_v(std::vector<T> const& v)
{
	return ff_mk_v([](auto const& v)
		{
			rapidjson::Value vData(rapidjson::kArrayType);

			for (auto&& itm : v)
			{
				vData.PushBack(f_mk_v(itm), s_doc.GetAllocator());
			}
			return vData;
		})(v);
}


inline rapidjson::Value f_mk_v(Singer const& v)
{
	return ff_mk_v([](auto const& v)
		{
			rapidjson::Value vData(rapidjson::kObjectType);
			V_ADD_MEMBER(vData, v, type);
			V_ADD_MEMBER(vData, v, age);
			return vData;
		})(v);
}

inline rapidjson::Value f_mk_v(Friend const& v)
{
	return ff_mk_v([](auto const& v)
		{
			rapidjson::Value vData(rapidjson::kObjectType);
			V_ADD_MEMBER(vData, v, relation);
			V_ADD_MEMBER(vData, v, secret);
			return vData;
		})(v);
}

inline rapidjson::Value f_mk_v(Address const& v)
{
	return ff_mk_v([](auto const& v)
		{
			rapidjson::Value vData(rapidjson::kObjectType);
			V_ADD_MEMBER(vData, v, country);
			V_ADD_MEMBER(vData, v, city);
			V_ADD_MEMBER(vData, v, street);
			V_ADD_MEMBER(vData, v, neighbors);
			return vData;
		})(v);
}

inline rapidjson::Value f_mk_v(Person const& v)
{
	return ff_mk_v([](auto const& v)
		{
			rapidjson::Value vData(rapidjson::kObjectType);
			V_ADD_MEMBER(vData, v, name);
			V_ADD_MEMBER(vData, v, age);
			V_ADD_MEMBER(vData, v, address);
			V_ADD_MEMBER(vData, v, _friends);
			V_ADD_MEMBER(vData, v, secret);
			return vData;
		})(v);
}


template<typename T>
std::string dump(T&& a)
{
	auto v = f_mk_v(a);
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	v.Accept(writer);

	return buffer.GetString();
};

