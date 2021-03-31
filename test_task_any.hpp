#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <experimental/any>
#include <memory>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"

#include "common_json_archive.hpp"

#include <boost/core/enable_if.hpp>

template <typename T>
rapidjson::Value f_json_dump_value(T&& a);

template <typename T>
void f_json_parse_value(rapidjson::Value&& v, T& a);

#define F__(x) std::forward<decltype(x)>(x)
#define M__(x) std::move(x)

namespace json
{

struct any
{
	any() = default;

	void swap(any& other)
	{
		obj.swap(other.obj);
		std::swap(json_obj, other.json_obj);
	}

	any(rapidjson::Value&& v) : json_obj(new rapidjson::Value(M__(v)))
	{}

	template <typename ValueType>
	any(ValueType&& a,
		typename boost::disable_if<std::is_same<any&, ValueType>>::type* = 0, // disable if value has type `any&`
		typename boost::disable_if<std::is_const<ValueType>>::type* =
			0) // disable if value has type `const ValueType&&`
		: json_obj(new rapidjson::Value(f_json_dump_value(a))), obj(F__(a))
	{}

	bool has_value() const
	{
		return !obj.empty();
	}
	decltype(auto) type() const
	{
		return obj.type();
	}
	auto get_json() const
	{
		return json_obj;
	}

	decltype(auto) get_obj() const
	{
		return (obj);
	}

  private:
	std::shared_ptr<rapidjson::Value> json_obj{new rapidjson::Value()}; // 默认为Null

	// 因为any object 可以被any_cast成实际类型 然后被修改
	// 这种情况下 无法监控到obj的变化 从而同步修改 json_obj
	// TODO: 研究如何监控any object 被修改(any_cast成实际类型后)
	mutable std::experimental::any obj;
};

template <typename T, typename ANY>
auto any_cast(ANY&& a) -> decltype(std::experimental::any_cast<T>(F__(a).get_obj()))
{
	if (!a.has_value() && !a.get_json()->IsNull())
	{
		// 利用any_cast的时机构造实际对象
		std::decay_t<T> temp;
		f_json_parse_value(M__(rapidjson::Value().CopyFrom(*a.get_json(), f_alloc())), temp);
		a.get_obj() = M__(temp);
	}
	return std::experimental::any_cast<T>(F__(a).get_obj());
}

bool operator==(any const& lhs, any const& rhs)
{
	return *lhs.get_json() == *rhs.get_json();
}

}; // namespace json
