#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <any>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"

#include <boost/core/enable_if.hpp>

template <typename T>
rapidjson::Value f_json_dump_value(T&& a);

#define F__(x) std::forward<decltype(x)>(x)
#define M__(x) std::move(x)

namespace json
{

struct any
{
	any() = default;

	template <typename ValueType>
	any(ValueType&& a,
		typename boost::disable_if<std::is_same<any&, ValueType>>::type* = 0, // disable if value has type `any&`
		typename boost::disable_if<std::is_const<ValueType>>::type* = 0) // disable if value has type `const ValueType&&`
		: json_obj(new rapidjson::Value(f_json_dump_value(a))), obj(F__(a))
	{}

	bool has_value() const
	{
		return obj.has_value();
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
	// 这种情况下 无法监控到obj的变化 从而实时修改 json_obj
	// 所以暂时 这里只支持const any
	// TODO: 研究如何监控any object 被修改(any_cast成实际类型后)
	const std::any obj;
};

template <typename T, typename ANY>
auto any_cast(ANY&& a) -> decltype(std::any_cast<T>(F__(a).get_obj()))
{
	return std::any_cast<T>(F__(a).get_obj());
}
}; // namespace json
