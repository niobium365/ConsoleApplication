#pragma once

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include "test_task.hpp"


#define JSON_SERIALIZATION_NVP(name)                              \
    boost::serialization::make_nvp(BOOST_PP_STRINGIZE(name), g.name)


namespace boost
{
namespace serialization
{

template <class Archive>
void serialize(Archive& ar, Singer& g, const unsigned int version)
{
	ar& JSON_SERIALIZATION_NVP(type);
	ar& JSON_SERIALIZATION_NVP(age);
}

template <class Archive>
void serialize(Archive& ar, Address& g, const unsigned int version)
{
	ar& JSON_SERIALIZATION_NVP(country);
	ar& JSON_SERIALIZATION_NVP(city);
	ar& JSON_SERIALIZATION_NVP(street);
	ar& JSON_SERIALIZATION_NVP(neighbors);
}
template <class Archive>
void serialize(Archive& ar, Friend& g, const unsigned int version)
{
	ar& JSON_SERIALIZATION_NVP(relation);
	ar& JSON_SERIALIZATION_NVP(secret);
}
template <class Archive>
void serialize(Archive& ar, Person& g, const unsigned int version)
{
	ar& JSON_SERIALIZATION_NVP(name);
	ar& JSON_SERIALIZATION_NVP(age);
	ar& JSON_SERIALIZATION_NVP(address);
	ar& JSON_SERIALIZATION_NVP(_friends);
	ar& JSON_SERIALIZATION_NVP(secret);
}

} // namespace serialization
} // namespace boost

#include "json_oarchive.hpp"
#include "json_iarchive.hpp"
