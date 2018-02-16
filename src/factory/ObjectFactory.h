/*
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
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership.
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
 */
#ifndef FACTORY_OBJECTFACTORY_H_
#define FACTORY_OBJECTFACTORY_H_

#include <cassert>
#include <cstdio>
#include <cstring>

#include <string>
#include <unordered_map>
#include <utility>

namespace factory {

template<class BaseClass, class ... Args>
class ObjectFactory {
 public:
  typedef BaseClass* (*Constructor)(Args...);

  // the destructor of this class is used to delete the constructor map
  ~ObjectFactory() {
    if (constructorMap_ != nullptr) {
      delete constructorMap_;
    }
  }

  // this function maps the derived class name to the RegisterClass.create
  //  function that is used to call the actual constructor
  static void registerClass(const std::string& _type,
                            Constructor _constructor) {
    // add the mapping between type and constructor to the map
    //  ensure this type doesn't already exist
    bool res = constructorMap_->insert(std::make_pair(
        _type, _constructor)).second;
    (void)res;  // unused if assert not enabled
    assert(res);
  }

  // this function uses the constructor map to call the RegisterClass.create
  //  function
  static BaseClass* create(const std::string& _type, Args ... _args) {
    // retrieve the creator function
    if (constructorMap_ != nullptr && constructorMap_->count(_type) > 0) {
      // use the creator function to call the constructor
      return constructorMap_->at(_type)(_args...);
    } else {
      // for missing type, return nullptr
      return nullptr;
    }
  }

  // this static function returns the corresponding factory reference
  static ObjectFactory& get() {
    // define the factory statically
    static ObjectFactory<BaseClass, Args...> factory;

    // create the constructor map if not already created
    if (constructorMap_ == nullptr) {
      constructorMap_ = new ConstructorMap();
    }

    // return a reference to the static factory object
    return factory;
  }

 private:
  ObjectFactory() = default;
  ObjectFactory(const ObjectFactory&) = delete;
  ObjectFactory& operator=(const ObjectFactory&) = delete;

  // this defines the constructor map
  typedef std::unordered_map<std::string, Constructor> ConstructorMap;

  // this is the static constructor map for this factory
  static ConstructorMap* constructorMap_;
};

// this initializes the static constructor map pointer for each template
//  type that gets used
template<class BaseClass, class ... Args> std::unordered_map<
  std::string, BaseClass* (*)(Args...)>* ObjectFactory<
  BaseClass, Args...>::constructorMap_ = nullptr;

// this class is used as a dummy object so that derived classes can use
//  a macro that looks like a function call to register themselves to their
//  corresponding factory
template<class BaseClass, class DerivedClass, class ... Args>
class RegisterClass {
 public:
  // this constructor simply registers the class with the factory
  explicit RegisterClass(const std::string& _type) {
    ObjectFactory<BaseClass, Args...>::get().registerClass(_type, create);
  }

  // this function calls the derived class's constructor
  static BaseClass* create(Args... _args) {
    return new DerivedClass(_args...);
  }
};

}  // namespace factory

// this macro is how derived classes register themselves with their
//  corresponding factory. this should be called in the .cc file of the class
#define registerWithObjectFactory(_type, ...)                 \
  static factory::RegisterClass<__VA_ARGS__> dummyObj(_type)

#endif  // FACTORY_OBJECTFACTORY_H_
