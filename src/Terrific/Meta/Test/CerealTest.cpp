#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>

#include <json.hpp>
#include <Terrific/Impl/Person.h>
#include <Terrific/Impl/JsonCast.h>
#include <Terrific/Impl/MovieInfo.h>
#include <Terrific/Impl/StringCast.cpp>


#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/Assert.h>

#include <MetaStuff/Meta.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <assert.h>

struct MyRecord {
	uint8_t x, y;
	float z;

	template <class Archive>
		void serialize( Archive & ar )
		{
			ar( x, y, z );
		}
};

struct SomeData {
	int32_t id;
	std::shared_ptr<std::unordered_map<uint32_t, MyRecord>> data;

	template <class Archive>
  void save( Archive & ar ) const {
    ar( data );
		}

	template <class Archive>
  void load( Archive & ar ) {
    static int32_t idGen = 0;
			id = idGen++;
			ar( data );
		}
};

void TestSerialization() {
  Person person1; 
  person1.age = 25; 
  person1.salary = 3.50f; 
  person1.name = "Alex";

  person1.favouriteMovies["Nostalgia Critic"] = { MovieInfo{ "The Room", 8.5f } };
  person1.favouriteMovies["John Tron"] = { MovieInfo{ "Goosebumps", 10.0f },
                                           MovieInfo{ "Talking Cat", 9.0f } }; 

}

int main() {
	MyRecord myRecord;
	myRecord.x = 5;
	SomeData myData;
  std::unordered_map<uint32_t, MyRecord> data;
  data.insert({0, myRecord}); 

  myData.data = std::make_shared<std::unordered_map<uint32_t, MyRecord>>(data);
  {
    std::ofstream ofs("out.cereal", std::ios::binary);
    cereal::BinaryOutputArchive archive( ofs );

    archive( myData );
  }

  {
    std::ofstream ofs("out.json");
    cereal::JSONOutputArchive archive( ofs ); 

    archive( myData ); 
  }
  //std::ifstream ifs("out.cereal", std::ios::binary);
  SomeData newData;

  std::ifstream ifs("out.json");
  cereal::JSONInputArchive archive_json( ifs );
  archive_json(newData);
  assert(newData.x == myData.x);

  return 0;
}
