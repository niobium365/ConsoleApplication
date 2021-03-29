#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING 1
#define RAPIDJSON_HAS_STDSTRING 1
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING 1
#define RAPIDJSON_HAS_CXX11_RVALUE_REFS 1
#define _CRT_SECURE_NO_WARNINGS 1

#include "test_task.hpp"
#include "test_task_parse.hpp"
#include "test_task_dump.hpp"
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

using rapidjson::Document;
using rapidjson::Value;
using rapidjson::StringBuffer;
using rapidjson::kObjectType;
using rapidjson::kArrayType;
using rapidjson::kNullType;

using std::string;
using std::function;
using std::map;
using std::vector;
using std::decay_t;
using std::ostream;
using std::operator""s;

template <typename T>
void register_it()
{
	register_deserialization<T>();
	register_serialization(id__<T>{});
}


inline auto dummy = []()
{
	register_it<Person>();
	register_it<Friend>();
	register_it<Address>();
	register_it<Singer>();
	register_it<string>();
	register_it<int>();
	register_it<size_t>();
	register_it<json::any>();
	//register_it<const char*>();

	return true;
}();

template<typename T>
ostream& operator<<(ostream& os, vector<T> const& d)
{
	os << "[";
	for (auto a : d)
	{
		os << a;
	}
	os << "]";
	return os;
}


ostream& operator<<(ostream& os, int d)
{
	os << std::to_string(d);
	return os;
}
ostream& operator<<(ostream& os, Friend const& d);
ostream& operator<<(ostream& os, Address const& d);
ostream& operator<<(ostream& os, json::any const& d);

ostream& operator<<(ostream& os, json::any const& d)
{
	os << dump(d);
	return os;
}

ostream& operator<<(ostream& os, Person const& d)
{
	os << d.name << std::to_string(d.age) << d.address << d._friends << d.secret;
	return os;
}
ostream& operator<<(ostream& os, Friend const& d)
{
	os << d.relation << d.secret;
	return os;
}
ostream& operator<<(ostream& os, Address const& d)
{
	os << d.country << d.city << d.street << d.neighbors;
	return os;
}

bool operator==(json::any const& lhs, json::any const& rhs)
{
	return dump(lhs) == dump(rhs);
}

int main_impl()
{
	Friend f1{ "my best friend", Singer{"rocker", 18} };
	Friend f2{ "new friend", "little girl" };
	Friend f3{ "third friend", 3 };
	Person p2{ "p2", 3, Address{"china", "shanghai", "putuo"} };
	Address addr1{ "china", "beijing", "wangjing", {p2} };
	Person p1{ "p1", 4, addr1, {f1, f2, f3}, "the kind!" };

	// TODO. 以下是伪代码，需要笔试者具体实现

	{
		json::any aaa = string("Rocker!");
		auto s = dump(aaa);
		std::cout << s << std::endl; // 打印序列化结果
		auto m = parse(s);
		auto& kkk = std::any_cast<string&>(m);
		assert(aaa == m); // 反序列化的结果是对的
	}

	{
		Singer aaa{ "aaa", 100 };
		auto s = dump(aaa);
		std::cout << s << std::endl; // 打印序列化结果
		auto m = parse(s);
		auto& kkk = std::any_cast<Singer&>(m);
		assert(json::any(aaa) == m); // 反序列化的结果是对的
	}


	auto json = dump(p1);           // 序列化
	std::cout << json << std::endl; // 打印序列化结果

	std::cout << p1 << std::endl; // 打印 Person 对象

	auto pp = parse(json); // 反序列化

	auto& kkk2 = std::any_cast<Person&>(pp);

	assert(json::any(p1) == pp); // 反序列化的结果是对的

	return 0;
}

int main()
{
	try
	{
		return main_impl();
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
	}
}