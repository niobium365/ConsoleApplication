#pragma once

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "test_task.hpp"

namespace boost
{
namespace serialization
{

template <class Archive>
void serialize(Archive& ar, Singer& g, const unsigned int version)
{
	auto& [type, age] = g;
	ar& BOOST_SERIALIZATION_NVP(type);
	ar& BOOST_SERIALIZATION_NVP(age);
}

template <class Archive>
void serialize(Archive& ar, Address& g, const unsigned int version)
{
	auto& [country, city, street, neighbors] = g;
	ar& BOOST_SERIALIZATION_NVP(country);
	ar& BOOST_SERIALIZATION_NVP(city);
	ar& BOOST_SERIALIZATION_NVP(street);
	ar& BOOST_SERIALIZATION_NVP(neighbors);
}
template <class Archive>
void serialize(Archive& ar, Friend& g, const unsigned int version)
{
	auto& [relation, secret] = g;
	ar& BOOST_SERIALIZATION_NVP(relation);
	ar& BOOST_SERIALIZATION_NVP(secret);
}
template <class Archive>
void serialize(Archive& ar, Person& g, const unsigned int version)
{
	auto& [name, age, address, _friends, secret] = g;
	ar& BOOST_SERIALIZATION_NVP(name);
	ar& BOOST_SERIALIZATION_NVP(age);
	ar& BOOST_SERIALIZATION_NVP(address);
	ar& BOOST_SERIALIZATION_NVP(_friends);
	ar& BOOST_SERIALIZATION_NVP(secret);
}

} // namespace serialization
} // namespace boost

#include "json_oarchive.hpp"
