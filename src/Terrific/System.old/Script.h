#pragma once

#include <Magnum/Resource.h>

namespace Terrific {
  namespace System {

    class Script : public Magnum::Resource<Script> {
    public:

      bool reload(){};
      void unload(){};
      void destroy(){}; 

      virtual int getFuncHandle(std::string const &asFunc)=0;
      virtual void addArg(std::string const &asFunc)=0; 

/**
 *  \brief Runs a function in the script, for example "execInit(0)"
 *  \param asFuncLine is the line of code
 *  \return true if no error, else false
 */
      virtual bool run(std::string const &asFuncLine)=0;
      virtual bool run(int const alHandle)=0; 
    };

  };
};
