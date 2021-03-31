#pragma once

#include "nvp.hpp"

#include "test_task.hpp"


#define JSON_SERIALIZATION_NVP(name)                              \
    json_serialization::make_nvp(#name, g.name)


namespace json_serialization
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

} // namespace json_serialization

#include "json_oarchive.hpp"
#include "json_iarchive.hpp"
