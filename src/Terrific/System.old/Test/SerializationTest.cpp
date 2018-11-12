#include <iostream>

#include <json.hpp>
#include <Terrific/Impl/Person.h>
#include <Terrific/Impl/JsonCast.h>
#include <Terrific/Impl/MovieInfo.h>
#include <Terrific/Impl/StringCast.cpp>


#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/Assert.h>

class Unregistered
{ };

#include <MetaStuff/Meta.h>

struct SerializationTest : Corrade::TestSuite::Tester{
  explicit SerializationTest() {
    addTests({
              &SerializationTest::initialize,
              &SerializationTest::verifyMeta,
              &SerializationTest::verifyMembers,
              &SerializationTest::verifySettingMembers

      });
  }

  void initialize() {
    person1.age = 25;

    person1.salary = 3.50f;

    person1.name = "Alex"; // I'm a person!


    person1.favouriteMovies["Nostalgia Critic"] = { MovieInfo{ "The Room", 8.5f } };
    person1.favouriteMovies["John Tron"] = { MovieInfo{ "Goosebumps", 10.0f },
                                             MovieInfo{ "Talking Cat", 9.0f } };

    CORRADE_VERIFY(person1.name == "Alex");
  }

  void verifyMeta() {
    CORRADE_VERIFY(meta::isRegistered<Person>());
    CORRADE_VERIFY(meta::getMemberCount<Person>() == 4);

    CORRADE_VERIFY(meta::isRegistered<Unregistered>() == false);
    CORRADE_VERIFY(meta::getMemberCount<Unregistered>() == 0);

    // checking if class has a member
    CORRADE_VERIFY(meta::hasMember<Person>("age"));


  }


  void verifyMembers() {
    auto age = meta::getMemberValue<int>(person1, "age");
    CORRADE_VERIFY(age == 25);

    auto name = meta::getMemberValue<std::string>(person1, "name");
    CORRADE_VERIFY(name == "Alex");

#if 0
    {
      std::cout << "Members of class Person:\n";
      meta::doForAllMembers<Person>([](const auto& member) {
                                      std::cout << "* " << member.getName() << '\n';
                                    });


      std::cout << "Members of class MovieInfo:\n";
      meta::doForAllMembers<MovieInfo>([](const auto& member) {
                                         std::cout << "* " << member.getName() << '\n';
                                       });
    }
#endif
  }

  void verifySettingMembers() {
    meta::setMemberValue<std::string>(person1, "name", "Ron Burgundy");
    auto name = meta::getMemberValue<std::string>(person1, "name");
    CORRADE_VERIFY(name == "Ron Burgundy");
  }

  void verifySerialization() {
    // And here's how you can serialize/deserialize
    // (if you write a function for your type)
    //std::cout << "Serializing person into json: " << '\n';
    json root = person1;
    //std::cout << std::setw(4) << root << std::endl;

    // Serializing Person 2 from JSON:
    person2 = root.get<Person>();

    //std::cout << "Person 2 name is " << person2.getName() << " too!" << '\n';
    CORRADE_VERIFY(person2.getName() == person1.getName());
  }


  Person person1;
  Person person2;
};

CORRADE_TEST_MAIN(SerializationTest)
