#pragma once

#include "common_json_archive.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"

#include "test_task_any.hpp"

struct json_iarchive_t;

template <typename T>
void f_json_parse_value(rapidjson::Value&& v, T& a);

struct json_iarchive_t
{
	json_iarchive_t(rapidjson::Value&& v) : doc(M__(v)){};

	using is_saving = std::false_type;

	template <typename T>
	json_iarchive_t& operator>>(json_serialization::nvp<T> const& nv)
	{
		load(nv.name(), nv.value());
		return *this;
	}

	template <typename T>
	json_iarchive_t& operator>>(T& v)
	{
		load(v);
		return *this;
	}

	template <typename T>
	json_iarchive_t& operator&(T& v)
	{
		return operator>>(v);
	}

	template <typename T>
	void load(char const* name, T& v)
	{
		f_json_parse_value(M__(doc[name]), v);
	}
	void load(json::any& b)
	{
		json::any temp(M__(doc));
		b.swap(temp);
	}

	void load(bool& b)
	{
		b = doc.GetBool();
	}
	void load(int& i)
	{
		i = doc.GetInt();
	}
	void load(unsigned long& i)
	{
		i = doc.GetUint();
	}
	void load(std::string& s)
	{
		s = doc.GetString();
	}

	template <typename T>
	void load(T& v2)
	{
		auto& v = const_cast<std::decay_t<T>&>(v2);
		using json_serialization::serialize;
		serialize(*this, v, 0u);
	}

	template <typename T, typename... ARGS>
	void load(std::vector<T, ARGS...>& vec_)
	{
		for (rapidjson::Value::ValueIterator itr = doc.Begin(); itr != doc.End(); ++itr)
		{
			T temp;
			f_json_parse_value(M__(*itr), temp);
			vec_.push_back(M__(temp));
		}
	}

  private:
	rapidjson::Value doc;
};

template <typename T>
void f_json_parse_value(rapidjson::Value&& v, T& a)
{
	json_iarchive_t oa{M__(v)};
	oa >> a;
}
