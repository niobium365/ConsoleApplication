#define _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING 1
#define RAPIDJSON_HAS_STDSTRING 1
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING 1
#define RAPIDJSON_HAS_CXX11_RVALUE_REFS 1
#define _CRT_SECURE_NO_WARNINGS 1

#include "test_task.hpp"
#include "test_task_boost.hpp"
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
std::string dump_v(Value const& v);
ostream& operator<<(ostream& os, json::any const& d)
{
	os << dump_v(f_json_dump_value(d));
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

template <typename T>
auto parse(Value v)
{
	T t;
	f_json_parse_value(M__(v), t);
	return t;
}

std::string dump_v(Value const& v)
{
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	v.Accept(writer);

	auto r = buffer.GetString();
	std::cout << r << std::endl;
	return r;
};

int main_impl2()
{
	{
		json::any f1("rocker"s);

		auto r = f_json_dump_value(f1);

		json_iarchive_t ia{M__(r)};
		f1 = {};
		ia >> f1;

		auto x = json::any_cast<string const&>(f1);

		dump_v(f_json_dump_value(f1));
	}
	{
		Friend f1{"my best friend", Singer{"rocker"s, 18}};
		auto r = f_json_dump_value(f1);

		json_iarchive_t ia{M__(r)};
		f1 = {};
		ia >> f1;

		auto x = json::any_cast<Singer const&>(f1.secret);

		dump_v(f_json_dump_value(f1));
	}

	Friend f1{"my best friend", Singer{"rocker", 18}};
	Friend f2{"new friend", "little girl"};
	Friend f3{"third friend", 3};
	Person p2{"p2", 3, Address{"china", "shanghai", "putuo"}};
	Address addr1{"china", "beijing", "wangjing", {p2}};
	Person p1{"p1", 4, addr1, {f1, f2, f3}, "the kind!"};

	auto json = f_json_dump_value(p1); // 序列化
	dump_v(json);                      // 打印序列化结果

	auto pp = parse<Person>(M__(json));

	assert(p1 == pp); // 反序列化的结果是对的

	auto& x = json::any_cast<string const&>(pp.secret);

	dump_v(f_json_dump_value(pp));
	return 0;
}

int main()
{
	try
	{
		return main_impl2();
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
	}
}