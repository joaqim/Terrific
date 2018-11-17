#include <iostream>
#include <fstream>

#include <json.hpp>
#include <Terrific/Meta/Person.h>
#include <Terrific/Meta/JsonCast.h>
#include <Terrific/Meta/MovieInfo.h>
#include <Terrific/Meta/StringCast.cpp>

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp> 

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
            &SerializationTest::verifySettingMembers,
            &SerializationTest::cerealSerialization

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

  void cerealSerialization() {
    {
      std::ofstream ofs("out.json");
      cereal::JSONOutputArchive archive( ofs ); 

      archive(person1);
    }
    {
      Person newPerson;
      std::ifstream ifs("out.json");
      cereal::JSONInputArchive archive_json( ifs );
      archive_json(newPerson);

      CORRADE_VERIFY(newPerson.name == person1.name);
      CORRADE_VERIFY(newPerson.age == person1.age);
      CORRADE_VERIFY(newPerson.salary == person1.salary);
      CORRADE_VERIFY(newPerson.favouriteMovies["John Tron"][0].name == "Goosebumps");
      CORRADE_VERIFY(newPerson.favouriteMovies["John Tron"][0].rating == 10.f);
      CORRADE_VERIFY(newPerson.favouriteMovies["John Tron"][1].name == "Talking Cat");
      CORRADE_VERIFY(newPerson.favouriteMovies["John Tron"][1].rating == 9.0f);
      CORRADE_VERIFY(newPerson.favouriteMovies["Nostalgia Critic"][0].name == "The Room");
      CORRADE_VERIFY(newPerson.favouriteMovies["Nostalgia Critic"][0].rating == 8.5f);
    }
  }

  void verifySettingMembers() {
    std::string unused; 
    meta::setMemberValue<decltype(unused)>(person1, "name", "Ron Burgundy");
    //meta::setMemberValue<std::string>(person1, "name", "Ron Burgundy");
    //auto name = meta::getMemberValue<std::string>(person1, "name");
    auto name = meta::getMemberValue<decltype(unused)>(person1, "name");
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
