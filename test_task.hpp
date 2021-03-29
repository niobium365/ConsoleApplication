#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <any>

// 从c++20 库中抄过来
template <class _Ty>
struct type_identity {
	using type = _Ty;
};

// 帮助overload 不同类型 等功能
template <typename T>
using id__ = type_identity<T>;

namespace json
{
	struct any : std::any
	{
		using parent_t = std::any;
		using parent_t::parent_t;

		any() = default;

		// 特殊处理 const char*
		// const char* 反序列化时类型应该是string 否则无法分配内存
		any(const char* s)
			:parent_t(std::string(s))
		{}
	};

}; // namespace json

struct Person;
struct Singer
{
	std::string type;
	int age;
};
struct Address
{
	std::string country;
	std::string city;
	std::string street;
	std::vector<Person> neighbors;
};
struct Friend
{
	std::string relation;
	json::any secret;
};
struct Person
{
	std::string name;
	int age;
	Address address;
	std::vector<Friend> _friends;
	json::any secret;
};

