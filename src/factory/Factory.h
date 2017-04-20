/*
 * Copyright (c) 2017, Nic McDonald, Doug Gibson
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * - Neither the name of prim nor the names of its contributors may be used to
 * endorse or promote products derived from this software without specific prior
 * written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * A significant portion of this work was modified from:
 * http://stackoverflow.com/questions/2906638
 *
 */
#ifndef FACTORY_FACTORY_H_
#define FACTORY_FACTORY_H_

#include <cassert>
#include <cstring>

#include <string>
#include <unordered_map>
#include <utility>

namespace factory {

template<class BaseClass, class ... Args>
class Factory {
 public:
  typedef BaseClass* (*Constructor)(Args...);

  // this function maps the derived class name to the RegisterClass.create
  //  function that is used to call the actual constructor
  static void registerClass(const char* _name, Constructor _constructor) {
    // add the mapping between name and constructor to the map
    //  ensure this name doesn't already exist
    bool res = constructorMap.insert(std::make_pair(
        _name, _constructor)).second;
    (void)res;  // unused if assert not enabled
    assert(res);
  }

  // this function uses the constructor map to call the RegisterClass.create
  //  function
  static BaseClass* create(const char* _name, Args ... _args) {
    // retrieve the creator function, use it to call the constructor
    if (constructorMap.count(_name) > 0) {
      return constructorMap.at(_name)(_args...);
    } else {
      // for missing name, return nullptr
      return nullptr;
    }
  }

  // this static function returns the corresponding factory reference
  static Factory& get() {
    // define the factory statically
    static Factory<BaseClass, Args...> factory;

    // return a reference to the static factory object
    return factory;
  }

 private:
  Factory() = default;
  Factory(const Factory&) = delete;
  Factory& operator=(const Factory&) = delete;

  // this defines the constructor map
  typedef std::unordered_map<const char*, Constructor> ConstructorMap;

  // this is the static constructor map for this factory
  static ConstructorMap constructorMap;
};

// this initializes the static constructor map pointer for each template
//  type that gets used
template<class BaseClass, class ... Args> std::unordered_map<
  const char*, BaseClass* (*)(Args...)> Factory<
  BaseClass, Args...>::constructorMap;

// this class is used as a dummy object so that derived classes can use
//  a macro that looks like a function call to register themselves to their
//  corresponding factory
template<class BaseClass, class DerivedClass, class ... Args>
class RegisterClass {
 public:
  // this constructor simply registers the class with the factory
  explicit RegisterClass(const char* _name) {
    Factory<BaseClass, Args...>::get().registerClass(_name, create);
  }

  // this function calls the derived class's constructor
  static BaseClass* create(Args ... _args) {
    return new DerivedClass(_args...);
  }
};

}  // namespace factory

// this macro is how derived classes register themselves with their
//  corresponding factory. this should be called in the .cc file of the class
#define registerWithFactory(name, ...) \
  static factory::RegisterClass<__VA_ARGS__> dummyObj(name)

#endif  // FACTORY_FACTORY_H_
