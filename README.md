# libfactory
Libfactory is a library for generating abstract object factories in C++.

Object factories are a popular way to create objects based from string identifiers. For example, a user may specify they want a "poodle" and the code needs to turn that into a Poodle object.

The archaic way of making object factories looks like this:
```c++
// base class
#include "Puppy.h"

// derived classes
#include "Chihuahua.h"
#include "Labrador.h"
#include "Poodle.h"
#include "Rottweiler.h"

class PuppyFactory {
 public:
  Puppy* create(std::string breed, std::string name) {
    if (breed == "chihuahua") {
      return new Chihuahua(name);
    } else if (breed == "labrador") {
      return new Labrador(name);
    } else if (breed == "poodle") {
      return new Poodle(name);
    } else if (breed == "rottweiler") {
      return new RottWeiler(name);
    } else {
      throw std::runtime_error("bad puppy breed");
    }
  }
};
```
This requires every new derived type to not only implement its own functionality but also modify the factory class as well. Sometimes this is annoying other times this doesn't support adding proprietary code to an open source codebase. Ideally, proprietary code modules can be dropped into the open source code base and just compile.

Libfactory turns the above code into the following code:
```c++
#include <factory/ObjectFactory.h>

class Puppy {
 public:
  virtual void speak() = 0;  // must be overidden by derived classes

  Puppy* create(std::string breed, std::string name) {
    return factory::ObjectFactory<Puppy, std::string>::create(breed, name);
  }
}
```

To do this, each derived class conforms to a fixed constructor format. In the above example each Puppy takes one argument, a std::string name. At the end of each derived class source file, the code uses a macro to register the derived class with the base class factory, as follows:

```c++
#include <factory/ObjectFactory.h>
#include "Puppy.h"

class Rottweiler : public Puppy {
 public:
  Rottweiler(std::string name) : name_(name) {}

  void speak() override {
    printf("My name is %s and I will eat you!\n", name_.c_str());
  }

 private:
  std::string name_
};

registerWithObjectFactory("rottweiler", Puppy, Rottweiler, std::string);
```
