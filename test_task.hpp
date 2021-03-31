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

#include "test_task_any.hpp"

// 从c++20 库中抄过来
template <class _Ty>
struct type_identity
{
	using type = _Ty;
};

// 帮助overload 不同类型 等功能
template <typename T>
using id__ = type_identity<T>;

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

inline bool operator==(Address const& lhs, Address const& rhs);
inline bool operator==(Friend const& lhs, Friend const& rhs)
{
	return std::tie(lhs.relation, lhs.secret) == std::tie(rhs.relation, rhs.secret);
}

inline bool operator==(Singer const& lhs, Singer const& rhs)
{
	return std::tie(lhs.age, lhs.type) == std::tie(rhs.age, rhs.type);
}

inline bool operator==(Person const& lhs, Person const& rhs)
{
	return std::tie(lhs.address, lhs.age, lhs.name, lhs.secret, lhs._friends) ==
		   std::tie(rhs.address, rhs.age, rhs.name, rhs.secret, rhs._friends);
}

inline bool operator==(Address const& lhs, Address const& rhs)
{
	return std::tie(lhs.city, lhs.country, lhs.neighbors, lhs.street) ==
		   std::tie(rhs.city, rhs.country, rhs.neighbors, rhs.street);
}
