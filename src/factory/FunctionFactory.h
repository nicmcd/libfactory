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
#ifndef FACTORY_FUNCTIONFACTORY_H_
#define FACTORY_FUNCTIONFACTORY_H_

#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace factory {

template <typename Return, typename... Args>
class FunctionFactory {
 public:
  // This defines the function pointer.
  typedef Return (*FunctionPtr)(Args...);
  // This defines the function map.
  typedef std::unordered_map<std::string, FunctionPtr> FunctionMap;

  // This function maps the function name to the RegisterClass.create function
  // that is used to call the actual constructor.
  static void registerFunction(const std::string& _type,
                               FunctionPtr _function) {
    FunctionMap& function_map = get();
    // Adds the mapping between type and function pointer to the map ensure this
    // type doesn't already exist.
    bool res = function_map.insert(std::make_pair(_type, _function)).second;
    assert(res);
  }

  // This function uses the function map to retrieve the
  // RegisterFunction.retrieve function.
  static FunctionPtr retrieve(const std::string& _type) {
    FunctionMap& function_map = get();
    // Retrieves the function.
    if (function_map.count(_type) > 0) {
      return function_map.at(_type);
    } else {
      // For missing type, return nullptr.
      return nullptr;
    }
  }

  // This function returns all registered function names
  static std::vector<std::string> functions() {
    FunctionMap& function_map = get();
    std::vector<std::string> names;
    for (auto it = function_map.cbegin(); it != function_map.cend(); ++it) {
      names.push_back(it->first);
    }
    return names;
  }

 private:
  static FunctionMap& get() {
    // This is the static function map for this factory
    static FunctionMap function_map;
    return function_map;
  }

  FunctionFactory() = delete;
  ~FunctionFactory() = delete;
  FunctionFactory(const FunctionFactory&) = delete;
  FunctionFactory& operator=(const FunctionFactory&) = delete;
};

// This class is used as a dummy object so that function implementations can use
// a macro that looks like a function call to register themselves to their
// corresponding factory
template <typename Return, typename... Args>
class RegisterFunction {
 public:
  typedef Return (*FunctionPtr)(Args...);

  // This constructor simply registers the function with the factory
  RegisterFunction(const std::string& _type, FunctionPtr _func) {
    FunctionFactory<Return, Args...>::registerFunction(_type, _func);
  }
};

}  // namespace factory

// This macro is how derived classes register themselves with their
// corresponding factory. This should be called in the .cc file of the class.
#define registerWithFunctionFactory(_type, _func, ...) \
  static factory::RegisterFunction<__VA_ARGS__> dummy##__LINE__(_type, _func)

#endif  // FACTORY_FUNCTIONFACTORY_H_
