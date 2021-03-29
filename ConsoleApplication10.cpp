#include <string>
#include <vector>
#include <iostream>
//#include <boost/any.hpp>
#include <any>
#include <map>
#include <functional>
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace rapidjson;

using namespace std;

//template <class _Ty>
//struct type_identity {
//	using type = _Ty;
//};

template <typename T>
using id__ = type_identity<T>;

template <typename T>
auto f_parent(T&& a) -> typename T::parent_t&&
{
	return M__(a);
};
template <typename T>
auto f_parent(T const&& a) -> typename T::parent_t const&&
{
	return M__(a);
};

template <typename T>
auto f_parent(T& a) -> typename T::parent_t&
{
	return a;
};
template <typename T>
auto f_parent(T const& a) -> typename T::parent_t const&
{
	return a;
};

//=====================

namespace json
{
	struct any : std::any
	{
		using parent_t = std::any;
		using parent_t::parent_t;
	};

	auto f_mk_json(any const&) -> rapidjson::Document
	{
		return {};
	}

	bool operator==(any const& lhs, any const& rhs)
	{
		return f_mk_json(lhs) == f_mk_json(rhs);
	}

}; // namespace json



string dump_v(Value const& v)
{
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	v.Accept(writer);

	return buffer.GetString();
};

static map<uint64_t, function<json::any(Value const&)>> s_g;
static map<size_t, function<Value(json::any const&)>>s_kk;

struct Person;
struct Singer
{
	std::string type;
	int age;
	json::any xxx;
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

template <typename T>
T f_construct_from_doc(id__<T>, Value const& a);

string f_construct_from_doc_impl(id__<string>, Value const& a)
{
	return a.GetString();
}
int f_construct_from_doc_impl(id__<int>, Value const& a)
{
	return a.GetInt();
}

size_t f_construct_from_doc_impl(id__<size_t>, Value const& a)
{
	return a.GetUint();
}

Friend f_construct_from_doc_impl(id__<Friend>, Value const& a)
{
	assert(a.IsObject());
	return { f_construct_from_doc(id__<string>{}, a["relation"]), f_construct_from_doc(id__<json::any>{}, a["secret"]) };
}

json::any f_construct_from_doc_impl(id__<json::any>, Value const& a)
{
	std::cout << dump_v(a) << endl;
	if (!a.HasMember("any_tag"))
	{
		return {};
	}
	if (!a.HasMember("any_data"))
	{
		return {};
	}
	auto&& v = a["any_data"];
	auto k = v["tag_type"].GetUint();

	
	cout << k << "===" << dump_v(v) << endl;
	auto&& v2 = v["tag_data"];
	cout << k << "===" << dump_v(v) << dump_v(v2)<<endl;

	auto& f = s_g[k];

	auto r = f(v);

	//auto r = s_g[k](v["tag_data"]);

	return r;
}

Singer f_construct_from_doc_impl(id__<Singer>, Value const& a)
{
	assert(a.IsObject());
	return { 
		f_construct_from_doc(id__<string>{}, a["type"]), 
		f_construct_from_doc(id__<int>{}, a["age"]),
		f_construct_from_doc(id__<json::any>{}, a["xxx"])

			};
}

Person f_construct_from_doc_impl(id__<Person>, Value const& a)
{
	assert(a.IsObject());
	return { f_construct_from_doc(id__<string>{}, a["name"]), f_construct_from_doc(id__<int>{}, a["age"]),

			f_construct_from_doc(id__<Address>{}, a["address"]),
			f_construct_from_doc(id__<vector<Friend>>{}, a["_friends"]),
			f_construct_from_doc(id__<json::any>{}, a["secret"]) };
}

Address f_construct_from_doc_impl(id__<Address>, Value const& a)
{
	assert(a.IsObject());
	return {
		f_construct_from_doc(id__<string>{}, a["country"]),
		f_construct_from_doc(id__<string>{}, a["city"]),
		f_construct_from_doc(id__<string>{}, a["street"]),
		f_construct_from_doc(id__<vector<Person>>{}, a["neighbors"]),
	};
}

template <typename T>
vector<T> f_construct_from_doc_impl(id__<vector<T>>, Value const& a)
{
	assert(a.IsArray());

	vector<T> r;
	for (Value::ConstValueIterator itr = a.Begin(); itr != a.End(); ++itr)
	{
		r.push_back(f_construct_from_doc(id__<T>{}, * itr));
	}
	return r;
}

template <typename T>
T f_construct_from_doc(id__<T> t, Value const& a)
{
	return f_construct_from_doc_impl(t, a["tag_data"]);
}


auto f_construct_from_doc2(Value const& a)
{
	auto r = a["tag_type"].GetUint();

	return s_g[r](a);
	//return f_construct_from_doc_impl(t, a["tag_data"]);
}


struct dump_t
{
	dump_t()
	{
		doc.SetObject();
		v.SetObject();
	}
	Document doc;
	Value v;
	string d;
};

static Document s_doc;


auto const f_mk_v = [](auto f) 
{
	return [=](auto&& d) 
	{
		using v_t = decay_t<decltype(d)>;
		Value vJ(kObjectType);
		vJ.AddMember("tag_type", typeid(v_t).hash_code(), s_doc.GetAllocator());
		vJ.AddMember("tag_data", f(d), s_doc.GetAllocator());

		return vJ;

	};
};


Value mk_v(string const& v)
{
	return f_mk_v([](auto const&d) 
		{
			return std::move(Value().SetString(d.c_str(), d.size()));
		})(v);
}
Value mk_v(int const& v)
{
	return f_mk_v([](auto const& d)
		{
			return std::move(Value().SetInt(d));
		})(v);
}

Value mk_v(size_t const& v)
{
	return f_mk_v([](auto const& d)
		{
			return std::move(Value().SetUint(d));
		})(v);
}


//static map<size_t, function<Value(json::any const&)>>s_kk;
auto const f_aaa = [](size_t x, json::any a) 
{
	return s_kk[x](a);
};

template<typename T>
void reg_foo()
{
	s_kk[typeid(T).hash_code()] = [](auto const& anyObj) 
	{
		return mk_v(any_cast<T const&>(anyObj));
	};
}

Value mk_v(json::any const& v)
{
	return f_mk_v([](auto const& v)
		{
			Value vData(kObjectType);

			auto r = v.type().hash_code();
			vData.AddMember(StringRef("any_tag"), mk_v(v.type().hash_code()), s_doc.GetAllocator());
			vData.AddMember(StringRef("any_data"), f_aaa(r, v), s_doc.GetAllocator());
			return vData;
		})(v);
}

Value mk_v(Singer const& v)
{
	return f_mk_v([](auto const& v)
		{
			Value vData(kObjectType);

			vData.AddMember(StringRef("type"), mk_v(v.type), s_doc.GetAllocator());
			vData.AddMember(StringRef("age"), mk_v(v.age), s_doc.GetAllocator());
			vData.AddMember(StringRef("xxx"), mk_v(v.xxx), s_doc.GetAllocator());
			return vData;
		})(v);
}


dump_t& operator<<(dump_t& os, pair<string, string> const& d)
{
	auto& [k, v] = d;
	
	Value vJ(kObjectType);
	vJ.AddMember("tag_type", typeid(string).hash_code(), os.doc.GetAllocator());
	vJ.AddMember("tag_data", Value().SetString(v.c_str(), v.size()), os.doc.GetAllocator());

	os.v.AddMember(StringRef(k.c_str()), vJ, os.doc.GetAllocator());

	os.d.append(v);
	return os;
}

dump_t& operator<<(dump_t& os, pair<string, int> const& d)
{
	auto& [k, v] = d;

	Value vJ(kObjectType);
	vJ.AddMember("tag_type", typeid(int).hash_code(), os.doc.GetAllocator());
	vJ.AddMember("tag_data", Value().SetInt(v), os.doc.GetAllocator());

	os.doc.AddMember(StringRef(k.c_str()), vJ, os.doc.GetAllocator());

	return os;
}


dump_t& operator<<(dump_t& os, pair<string, Singer> const& d)
{
	auto& [k, v] = d;

	Value vJ(kObjectType);
	vJ.AddMember("tag_type", typeid(Singer).hash_code(), os.doc.GetAllocator());

	Value vObj;
	vObj.SetObject();

	vObj.AddMember("type", vObj, os.doc.GetAllocator());
	vObj.AddMember("age", vObj, os.doc.GetAllocator());



	vJ.AddMember("tag_data", vObj, os.doc.GetAllocator());

	os.doc.AddMember(StringRef(k.c_str()), vJ, os.doc.GetAllocator());

	//os.d.append(v);
	return os;
}


dump_t& operator<<(dump_t& os, string const& d)
{
	//os.d.append(v);
	return os;
}




template<typename T>
dump_t& operator<<(dump_t& os, vector<T> const& d)
{
	for (auto a : d)
	{
		os << a;
	}
	return os;
}


dump_t& operator<<(dump_t& os, int const& d)
{
	// os.d.append(d);
	return os;
}
dump_t& operator<<(dump_t& os, Friend const& d);
dump_t& operator<<(dump_t& os, Address const& d);
dump_t& operator<<(dump_t& os, json::any const& d);

dump_t& operator<<(dump_t& os, json::any const& d)
{
	// os.d.append(d);
	return os;
}

dump_t& operator<<(dump_t& os, Person const& d)
{
	os << d.name << d.age << d.address << d._friends << d.secret;
	return os;
}
dump_t& operator<<(dump_t& os, Friend const& d)
{
	os << d.relation << d.secret;
	return os;
}
dump_t& operator<<(dump_t& os, Address const& d)
{
	os << d.country << d.city << d.street << d.neighbors;
	return os;
}

template<typename T>
auto dump(T&& a)
{
	dump_t x;
	x << a;
	
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	x.doc.Accept(writer);

	return buffer.GetString();
};

template<typename T>
string dump2(T&& a)
{
	auto v = mk_v(a);
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	v.Accept(writer);

	return buffer.GetString();
};

ostream& operator<<(ostream& os, Person const& d)
{
	dump_t x;
	x << d;
	os << x.d;
	return os;
}

template <typename T>
void register_it_basic()
{
	s_g[typeid(T).hash_code()] = [](auto&& v)
	{
		return f_construct_from_doc(id__<T>{}, v);
	};
}

template <typename T>
void register_it()
{
	register_it_basic<T>();
	reg_foo<T>();
}

static auto dummy = []()
{
	register_it<Person>();
	register_it<Friend>();
	register_it<Address>();
	register_it<Singer>();
	register_it<string>();
	register_it<int>();
	register_it<size_t>();

	//reg_foo<Singer>();
	return true;
}();

auto parse(string const& a)
{
	Document document;
	document.Parse(a.c_str());

	return f_construct_from_doc2(document);

	//return f_construct_from_doc(id__<json::any>{}, document);
}

int main()
{
	Friend f1{ "my best friend", Singer{"rocker", 18} };
	Friend f2{ "new friend", "little girl" };
	Friend f3{ "third friend", 3 };
	Person p2{ "p2", 3, Address{"china", "shanghai", "putuo"} };
	Address addr1{ "china", "beijing", "wangjing", {p2} };
	Person p1{ "p1", 4, addr1, {f1, f2, f3}, "the kind!" };

	// TODO. 以下是伪代码，需要笔试者具体实现

	Singer a{ "aaa", 100, 1200 };

	auto s = dump2(a);
	std::cout << s << std::endl; // 打印序列化结果

	auto m = parse(s);

	auto& kkk = any_cast<Singer&>(m);


	auto json = dump(p1);           // 序列化
	std::cout << json << std::endl; // 打印序列化结果

	std::cout << p1 << std::endl; // 打印 Person 对象

	auto pp = parse(json); // 反序列化

	assert(json::any(p1) == pp); // 反序列化的结果是对的
}
