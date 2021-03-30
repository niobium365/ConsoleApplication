#pragma once

#include "common_json_archive.hpp"
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/nvp.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"

#include "test_task_any.hpp"

struct json_oarchive_t;

template <typename T>
rapidjson::Value f_json_dump_value(T&& a);

struct json_oarchive_t
{
	auto get()
	{
		return static_cast<rapidjson::Value&&>(doc);
	}

	rapidjson::Value doc;

	json_oarchive_t() = default;

	using is_saving = boost::true_type;

	template <typename T>
	json_oarchive_t& operator<<(boost::serialization::nvp<T> const& nv)
	{
		save(nv.name(), nv.value());
		return *this;
	}

	template <typename T>
	json_oarchive_t& operator<<(T const& v)
	{
		return operator<<(const_cast<T&>(v));
	}

	template <typename T>
	json_oarchive_t& operator<<(T& v)
	{
		save(v);
		return *this;
	}

	template <typename T>
	json_oarchive_t& operator&(T const& v)
	{
		return operator<<(v);
	}

	template <typename T>
	void save(char const* name, T& v)
	{
		doc.AddMember(rapidjson::StringRef(name), f_json_dump_value(v), s_doc_ins.GetAllocator()); //数据
	}
	void save(bool b)
	{
		doc = b;
	}
	void save(int i)
	{
		doc = i;
	}
	void save(json::any& v)
	{
		doc.CopyFrom(*v.get_json(), s_doc_ins.GetAllocator());
	}
	void save(unsigned long i)
	{
		doc = i;
	}
	void save(char* s)
	{
		doc = rapidjson::Value(s, s_doc_ins.GetAllocator());
	}
	void save(std::string& s)
	{
		doc = rapidjson::Value(s, s_doc_ins.GetAllocator());
	}

	template <typename T>
	void save(T& v2)
	{
		auto& v = const_cast<std::decay_t<T>&>(v2);
		using boost::serialization::serialize;
		doc = rapidjson::Value(rapidjson::kObjectType);
		serialize(*this, v, 0u);
	}

    template<template<typename...> typename C, typename...ARGS>
    auto save(C<ARGS...>& vec_) -> decltype(vec_.size(), void())
	{
		using boost::serialization::serialize;
		doc = rapidjson::Value(rapidjson::kArrayType);
		for (auto&& itm : vec_)
		{
			doc.PushBack(f_json_dump_value(itm), s_doc_ins.GetAllocator());
		}
	}
};

template <typename T>
rapidjson::Value f_json_dump_value(T&& a)
{
	json_oarchive_t oa;
	oa << a;
	return oa.get();
}
