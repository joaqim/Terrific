#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "MovieInfo.h"

#include <iostream>

#include <cereal/types/unordered_map.hpp> 
#include <cereal/types/string.hpp> 
#include <cereal/types/vector.hpp> 

struct Person {
    // add this if you want to register private members:
    // template <>
    // auto meta::registerMembers<Person>();

  void setAge(int a) {
    age = a;
    }

  int getAge() const {
    return age;
  }

  void setName(const std::string& name)
  {
    this->name = name;
  }

  const std::string& getName() const
  {
    return name;
  }

	template <class Archive>
  void serialize( Archive & ar )
  {
    ar( age, name, salary, favouriteMovies );
  }

  int age;
  std::string name;
  float salary;
  std::unordered_map<std::string, std::vector<MovieInfo>> favouriteMovies;
};

#include <MetaStuff/Meta.h>

namespace meta {

template <>
inline auto registerMembers<Person>()
{
    return members(
        member("age", &Person::getAge, &Person::setAge), // access through getter/setter only!
        member("name", &Person::getName, &Person::setName), // same, but ref getter/setter
        member("salary", &Person::salary),
        member("favouriteMovies", &Person::favouriteMovies)
    );
}

} // end of namespace meta
