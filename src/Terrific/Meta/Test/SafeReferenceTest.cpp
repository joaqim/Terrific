#include <sol.hpp>
#include <entityplus/entity.h>
#include <string>
#include <iostream>

using EntityId = int;

class Entity {
 public:
  explicit Entity(EntityId id) :
      name("John"), id(id)
  {}

  const std::string& getName() const { return name; }
  void setName(const std::string& n) { name = n; }
  EntityId getId() const { return id; }
 private:
  std::string name;
  EntityId id;
};

sol::state lua; // globals are bad, but we'll use it for simpler implementation

class EntityManager {
 public: 
  EntityManager() : idCounter(0) {}

  Entity& createEntity()
  {
    auto id = idCounter;
    ++idCounter;

    auto inserted = entities.emplace(id, std::make_unique<Entity>(id));
    auto it = inserted.first; // iterator to created id/Entity pair
    auto& e = *it->second; // created entity
    lua["createHandle"](e);
    return e;
  }

  void removeEntity(EntityId id)
  {
    lua["onEntityRemoved"](id);
    entities.erase(id);
  }
 private:
  std::unordered_map<EntityId, std::unique_ptr<Entity>> entities;
  EntityId idCounter;
};
#include <Corrade/TestSuite/TestSuite.h>
#include <Corrade/TestSuite/Tester.h>
#include <Corrade/TestSuite/Compare/StringToFile.h>
#include <Corrade/Utility/Directory.h>

int my_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
  // L is the lua state, which you can wrap in a state_view if necessary
  // maybe_exception will contain exception, if it exists
  // description will either be the what() of the exception or a description saying that we hit the general-case catch(...)
  std::cout << "An exception occurred in a function, here's what it says ";
  if (maybe_exception) {
    std::cout << "(straight from the exception): ";
    const std::exception& ex = *maybe_exception;
    std::cout << ex.what() << std::endl;
  }
  else {
    std::cout << "(from the description parameter): ";
    std::cout.write(description.data(), description.size());
    std::cout << std::endl;
  }

  // you must push 1 element onto the stack to be 
  // transported through as the error object in Lua
  // note that Lua -- and 99.5% of all Lua users and libraries -- expects a string
  // so we push a single string (in our case, the description of the error)
  return sol::stack::push(L, description);
}

void inline will_throw(sol::optional<std::string> maybe_msg) {
  throw std::runtime_error("Error");
  //std::cout << "exception" << std::endl;
}

using namespace Corrade::Utility::Directory;
  struct SafeReference : Corrade::TestSuite::Tester 
  {
    explicit SafeReference() {
      addTests({&SafeReference::TestOpenScript});
      lua.open_libraries();
    }

    void TestOpenScript();
  };

  void SafeReference::TestOpenScript() {

    //lua.set_exception_handler(&my_exception_handler);

	  std::cout << "Testing sol error catch \n";
    lua.set_function("will_throw", &will_throw);

    lua_atpanic(lua, sol::c_call<decltype(&will_throw), &will_throw>);

    sol::protected_function_result pfr = lua.safe_script("will_throw()", &sol::script_pass_on_error);

    CORRADE_VERIFY(!pfr.valid());

    sol::error err = pfr;

    //    CORRADE_EXPECT_FAIL("Testing error handling");

    lua.new_usertype<Entity>("Entity",
                             "getName", &Entity::getName,
                             "setName", &Entity::setName,
                             "getId", &Entity::getId);

    std::string exe_path = executableLocation().substr(0, executableLocation().size()-1);
    std::string file = join(exe_path.substr(0, exe_path.rfind("/")), "safe_reference.lua");
    std::cout << "Opening: " << file << "\n";
    bool fileFound = fileExists(file);
    CORRADE_VERIFY(fileFound);
   //    lua.do_file("safe_reference.lua");
    lua.safe_script_file(file);

    EntityManager entityManager;
    auto& entity = entityManager.createEntity();

    entity.setName("TestName");
    lua["test"](entity);

    //TODO: Get entity from lua
    //Entity ent;
    //CORRADE_VERIFY(ent.getName() == entity.getName());

    //CORRADE_EXPECT_FAIL("Testing bad reference");
        //    err = lua["testBadReference"](entity);
  }

CORRADE_TEST_MAIN(SafeReference);
