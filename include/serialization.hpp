#ifndef PROJECT_2__SERIALIZATION_HPP_
#define PROJECT_2__SERIALIZATION_HPP_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>
#include <sstream>

template<typename T>
std::string serialize(const T& object){
	std::ostringstream ofs;
	boost::archive::text_oarchive oa(ofs);
	oa & object;
	return ofs.str();
}

template<>
inline std::string serialize<bool>(const bool& object){
	return object?"1":"0";
}

template<typename T>
T deserialize(const std::string& serialized){
	if(serialized.empty())
		return T();
	T object;
	std::istringstream ifs(serialized);
	boost::archive::text_iarchive ia(ifs);
	ia & object;
	return object;
}

template<>
inline bool deserialize<bool>(const std::string& serialized) {
	return serialized=="1";
}

#endif //PROJECT_2__SERIALIZATION_HPP_
