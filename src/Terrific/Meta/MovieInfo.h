#pragma once

#include <string>
#include <cereal/types/string.hpp> 

struct MovieInfo {
    std::string name;
    float rating;
  template <class Archive>
  void serialize( Archive & ar )
  {
    ar(name, rating); 
  }
};


#include <MetaStuff/Meta.h>

namespace meta {

template <>
inline auto registerMembers<MovieInfo>()
{
    return members(
        member("name", &MovieInfo::name),
        member("rating", &MovieInfo::rating)
    );
}

} // end of namespace meta
