#pragma once

#include <utility>

namespace json_serialization
{

template <class T>
struct nvp : public std::pair<const char*, T*>
{
    // private:
	nvp(const nvp& rhs) : std::pair<const char*, T*>(rhs.first, rhs.second)
	{}

  public:
	explicit nvp(const char* name_, T& t) : std::pair<const char*, T*>(name_, std::addressof(t))
	{}

	const char* name() const
	{
		return this->first;
	}
	T& value() const
	{
		return *(this->second);
	}

	const T& const_value() const
	{
		return *(this->second);
	}

	template <class Archive>
	void save(Archive& ar, const unsigned int /* file_version */
	) const
	{
		ar.operator<<(const_value());
	}
	template <class Archive>
	void load(Archive& ar, const unsigned int /* file_version */
	)
	{
		ar.operator>>(value());
	}
};

template <class T>
inline const nvp<T> make_nvp(const char* name, T& t)
{
	return nvp<T>(name, t);
}

} // namespace json_serialization
