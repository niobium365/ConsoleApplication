#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <experimental/any>
#include <memory>
#include <functional>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"

#include "common_json_archive.hpp"

template <typename T>
rapidjson::Value f_json_dump_value(T&& a);

template <typename T>
void f_json_parse_value(rapidjson::Value&& v, T& a);

#define F__(x) std::forward<decltype(x)>(x)
#define M__(x) std::move(x)

namespace json
{

template <bool B, class T = void>
struct disable_if_c
{
	typedef T type;
};

template <class T>
struct disable_if_c<true, T>
{};

template <class Cond, class T = void>
struct disable_if : public disable_if_c<Cond::value, T>
{};

struct tag_unknown_any_t
{};

struct any
{
	// ctorA: 构造一个空的any
	any() : json_obj{new rapidjson::Value()}
	{}

	auto get_json() const
	{
		if (!json_obj)
		{
			json_obj = std::make_shared<rapidjson::Value>(json_obj_fun());
			json_obj_fun = {};
		}
		return json_obj;
	}

	void swap(any& other)
	{
		obj.swap(other.obj);

		// json_obj_fun capture this pointer!
		// TODO 尽量不要现在序列化
		if (!json_obj)
		{
			get_json();
			json_obj_fun = {};
		}
		if (!other.json_obj)
		{
			other.get_json();
			other.json_obj_fun = {};
		}

		std::swap(json_obj, other.json_obj);
	}

	// ctorB: 构造一个unknown类型的的any, 实际类型要等用户any_cast 后 或者用户explicit_parse 后获得
	any(rapidjson::Value&& v) : json_obj(new rapidjson::Value(M__(v)))
	{}

	// ctorC: 构造一个已知类型的的any
	template <typename ValueType>
	any(ValueType&& a,
		typename disable_if<std::is_same<any&, ValueType>>::type* = 0, // disable if value has type `any&`
		typename disable_if<std::is_const<ValueType>>::type* = 0)      // disable if value has type `const ValueType&&`
		: json_obj_fun(
			  [=]()
			  {
				  if (obj.empty())
				  {
					  return rapidjson::Value();
				  }
				  return f_json_dump_value(any_cast<std::decay_t<ValueType> const&>(obj));
			  }),
		  obj(F__(a))
	{}

	// TODO 赋值功能
	// auto& operator=(any const&);
	// auto& operator=(any&&);
	// template <typename T>
	// auto& operator=(T&&);

	template <typename T>
	decltype(auto) explicit_parse()
	{
		if (!has_value() && json_obj && !json_obj->IsNull())
		{
			std::decay_t<T> temp;
			f_json_parse_value(M__(rapidjson::Value().CopyFrom(*json_obj, f_alloc())), temp);
			obj = M__(temp);
		}
		return *this;
	}

	bool has_value() const
	{
		return !obj.empty();
	}

	// 检测是否为NULL
	bool is_null() const
	{
		if (json_obj)
		{
			return json_obj->IsNull();
		}

		if (json_obj_fun)
		{
			return !has_value();
		}

		return true;
	}

	decltype(auto) type() const
	{
		if (!has_value() && json_obj && !json_obj->IsNull())
		{
			return typeid(tag_unknown_any_t);
		}
		return obj.type();
	}
	decltype(auto) get_obj() const
	{
		return (obj);
	}

  private:
	// TODO 尽量去掉mutable
	mutable std::shared_ptr<rapidjson::Value> json_obj; //{new rapidjson::Value()}; // 默认为Null
	mutable std::function<rapidjson::Value()> json_obj_fun;

	// 因为any object 可以被any_cast成实际类型 然后被修改
	// 这种情况下 无法监控到obj的变化 从而同步修改 json_obj
	// TODO: 研究如何监控any object 被修改(any_cast成实际类型后)
	mutable std::experimental::any obj;
};

template <typename T, typename ANY>
auto any_cast(ANY&& a) -> decltype(std::experimental::any_cast<T>(F__(a).get_obj()))
{
	// 利用any_cast的时机构造实际对象
	a.template explicit_parse<std::decay_t<T>>();

	return std::experimental::any_cast<T>(F__(a).get_obj());
}

bool operator==(any const& lhs, any const& rhs)
{
	return *lhs.get_json() == *rhs.get_json();
}

}; // namespace json
