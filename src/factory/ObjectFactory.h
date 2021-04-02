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
#include <vector>

namespace factory {

template <class BaseClass, class... Args>
class ObjectFactory {
 public:
  // Defines the base class constructor.
  typedef BaseClass* (*Constructor)(Args...);
  // Defines the constructor map.
  typedef std::unordered_map<std::string, Constructor> ConstructorMap;

  // This function maps the derived class name to the RegisterClass.create
  // function that is used to call the actual constructor.
  static void registerClass(const std::string& _type,
                            Constructor _constructor) {
    ConstructorMap& constructor_map = get();
    // Adds the mapping between type and constructor to the map ensure this type
    // doesn't already exist.
    bool res =
        constructor_map.insert(std::make_pair(_type, _constructor)).second;
    assert(res);
  }

  // This function uses the constructor map to call the RegisterClass.create
  // function.
  static BaseClass* create(const std::string& _type, Args... _args) {
    ConstructorMap& constructor_map = get();
    // Retrieves the creator function.
    if (constructor_map.count(_type) > 0) {
      // Uses the creator function to call the constructor.
      return constructor_map.at(_type)(_args...);
    } else {
      // For missing type, return nullptr.
      return nullptr;
    }
  }

  // This function returns all registered class names.
  static std::vector<std::string> classes() {
    ConstructorMap& constructor_map = get();
    std::vector<std::string> names;
    for (auto it = constructor_map.cbegin(); it != constructor_map.cend();
         ++it) {
      names.push_back(it->first);
    }
    return names;
  }

 private:
  static ConstructorMap& get() {
    // This is the static constructor map for this factory.
    static ConstructorMap constructor_map;
    return constructor_map;
  }

  ObjectFactory() = delete;
  ~ObjectFactory() = delete;
  ObjectFactory(const ObjectFactory&) = delete;
  ObjectFactory& operator=(const ObjectFactory&) = delete;
};

// This class is used as a dummy object so that derived classes can use a macro
// that looks like a function call to register themselves to their corresponding
// factory.
template <class BaseClass, class DerivedClass, class... Args>
class RegisterClass {
 public:
  // This constructor registers the class with the factory.
  explicit RegisterClass(const std::string& _type) {
    ObjectFactory<BaseClass, Args...>::registerClass(_type, create);
  }

  // This function calls the derived class's constructor.
  static BaseClass* create(Args... _args) {
    return new DerivedClass(_args...);
  }
};

}  // namespace factory

// This macro is how derived classes register themselves with their
// corresponding factory. This should be called in the .cc file of the class.
#define registerWithObjectFactory(_type, ...) \
  static factory::RegisterClass<__VA_ARGS__> dummy##__LINE__(_type)

#endif  // FACTORY_OBJECTFACTORY_H_
