#include <ecst.hpp>

namespace example {

using vrm::core::uint;
using vrm::core::sz_t;

namespace component {
struct position {
  float _x;
  float _y;
};

struct velocity {
  float _x;
  float _y;
};

} // namespace component

namespace c = example::component;

namespace ct {
using namespace ecst;

constexpr auto position = tag::component::v<c::position>;
constexpr auto velocity = tag::component::v<c::velocity>;
} // namespace ct

namespace actions {

void move(float dt, c::position& p, const c::velocity& a);
void move(float dt, c::position& p, const c::velocity& a)
{
  p._x += a._x * dt;
  p._y += a._y * dt;
}
}
#define SYS_TAG(x)                                      \
  namespace system                                      \
  {                                                     \
    struct x;                                           \
  }                                                     \
  namespace st                                          \
  {                                                     \
    constexpr auto x = ecst::tag::system::v<system::x>; \
  }

SYS_TAG(velocity)
SYS_TAG(acceleration)


struct velocity {
  template <typename TData>
  void process(float dt, TData& data) {
    /*
      data.for_previous_outputs(st::acceleration, [](auto&, auto&&)
                              {
                              });
    */

    data.for_entities([&](auto eid)
                      {
                        auto& p = data.get(ct::position, eid);
                        const auto& v = data.get(ct::velocity, eid);
                        actions::move(dt, p, v);
                      });
  }
}; 

} // namespace example
#if 1
namespace example {
namespace ecst_setup {

constexpr auto entity_count = ecst::sz_v<1>;
constexpr auto make_csl() {
  namespace cs = ecst::signature::component;
  namespace csl = ecst::signature_list::component;

  return csl::make(               // .
      cs::make(ct::position),     // .
      cs::make(ct::velocity)     // .
                                  );
}

constexpr auto make_ssl() {
  using ecst::sz_v;

  namespace c = example::component;
  namespace s = example::system;
  namespace ss = ecst::signature::system;
  namespace sls = ecst::signature_list::system;

  namespace ips = ecst::inner_parallelism::strategy;
  namespace ipc = ecst::inner_parallelism::composer;

  constexpr auto test_p = ips::none::v();

  constexpr auto ssig_velocity =   // .
      ss::make(st::velocity)       // .
      .parallelism(test_p)     // .
      .read(example::ct::velocity)      // .
      .write(example::ct::position);    // .

  //  .dependencies(st::curve) // .

  return sls::make(ssig_velocity);
}
} // namespace ecst_setup
} // namespace example
namespace c = example::component;
namespace s = example::system;

auto rndf = [](float, float max) {
  return max;
};

template <typename TContext>
class game_app {
 private:
  TContext& _ctx;

  void init_loops() {
    float dt = 1;

    while(true)
    {
      update_ctx(dt);

      dt = 0.5f;

      if(!_running) {
        break;
      }
    }
  }

  template <typename TProxy>
  void mk_entity(TProxy& proxy)
  {
    auto eid = proxy.create_entity(); 
    auto& cv = proxy.add_component(example::ct::velocity, eid);
    cv._x = rndf(-5, 5);
    cv._y = rndf(-5, 5);

    auto& cp = proxy.add_component(example::ct::position, eid);
    cp._x = rndf(0, 1024);
    cp._y = rndf(0, 768);
  }

  bool _running = true;
  void update_ctx(float dt) {
    namespace sea = ::ecst::system_execution_adapter;
#if 1
    _ctx.step([this, dt](auto& proxy) {
        proxy.execute_systems_from(example::st::acceleration)( // .
            sea::t(example::st::acceleration)
            .for_subtasks([dt](auto& s, auto& data) {
                s.process(dt, data);
                          })); 


        if(!_ctx.any_entity_in(example::st::acceleration)) // st::life
        {
          _running = false;
        }
            });
#endif
  }

  void init() {
    _ctx.step([this](auto& proxy)
              {
                for(ecst::sz_t i = 0; i < example::ecst_setup::entity_count; ++i)
                {
                  this->mk_entity(proxy);
                }
              });

    init_loops();
  }

 public:
    game_app(TContext& ctx) : _ctx{ctx}
    {
      init();
    }
  };

#include "SettingsGenerator.cpp"
int main() {
  using namespace example;
  using namespace example::ecst_setup;

  auto test_impl = [&](auto& ctx)
                   {
                     using ct = ECST_DECAY_DECLTYPE(ctx);
                     game_app<ct> a{ctx};
                     (void)a;
                   };

  test::run_tests(test_impl, entity_count, make_csl(), make_ssl());
  for(int i{0}; i<10;i++) {
  }

  return 0;
}
#else

int main() {
  return EXIT_SUCCESS; 
}
#endif
