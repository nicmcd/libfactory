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

namespace factory {

template<typename Return, typename ... Args>
class FunctionFactory {
 public:
  typedef Return (*FunctionPtr)(Args...);

  // the destructor of this class is used to delete the function map
  ~FunctionFactory() {
    if (functionMap_ != nullptr) {
      delete functionMap_;
    }
  }

  // this function maps the function name to the RegisterClass.create
  //  function that is used to call the actual constructor
  static void registerFunction(const std::string& _type,
                               FunctionPtr _function) {
    // add the mapping between type and function pointer to the map
    //  ensure this type doesn't already exist
    bool res = functionMap_->insert(std::make_pair(
        _type, _function)).second;
    (void)res;  // unused if assert not enabled
    assert(res);
  }

  // this function uses the function map to retrieve the
  //  RegisterFunction.retrieve function
  static FunctionPtr retrieve(const std::string& _type) {
    // retrieve the function
    if (functionMap_ != nullptr && functionMap_->count(_type) > 0) {
      return functionMap_->at(_type);
    } else {
      // for missing type, return nullptr
      return nullptr;
    }
  }

  // this static function returns the corresponding factory reference
  static FunctionFactory& get() {
    // define the factory statically
    static FunctionFactory<Return, Args...> factory;

    // create the constructor map if not already created
    if (functionMap_ == nullptr) {
      functionMap_ = new FunctionMap();
    }

    // return a reference to the static factory object
    return factory;
  }

 private:
  FunctionFactory() = default;
  FunctionFactory(const FunctionFactory&) = delete;
  FunctionFactory& operator=(const FunctionFactory&) = delete;

  // this defines the function map
  typedef std::unordered_map<std::string, FunctionPtr> FunctionMap;

  // this is the static function map for this factory
  static FunctionMap* functionMap_;
};

// this initializes the static function map pointer for each template
//  type that gets used
template<typename Return, typename ... Args> std::unordered_map<
  std::string, Return (*)(Args...)>* FunctionFactory<
  Return, Args...>::functionMap_ = nullptr;

// this class is used as a dummy object so that function implementations can use
//  a macro that looks like a function call to register themselves to their
//  corresponding factory
template<typename Return, typename ... Args>
class RegisterFunction {
 public:
  typedef Return (*FunctionPtr)(Args...);

  // this constructor simply registers the function with the factory
  RegisterFunction(const std::string& _type,
                   FunctionPtr _func) {
    FunctionFactory<Return, Args...>::get().registerFunction(_type, _func);
  }
};

}  // namespace factory

// this macro is how derived classes register themselves with their
//  corresponding factory. this should be called in the .cc file of the class
#define registerWithFunctionFactory(_type, _func, ...)                  \
  static factory::RegisterFunction<__VA_ARGS__> dummyObj(_type, _func)

#endif  // FACTORY_FUNCTIONFACTORY_H_
