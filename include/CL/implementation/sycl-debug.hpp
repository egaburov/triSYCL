/** \file This is a small class to track constructor/destructor invocations

    Define the TRISYCL_DEBUG CPP flag to have an output.

    To use it in some class C, make C inherit from debug<C>.

    Ronan.Keryell at AMD dot com

    Copyright 2014 Advanced Micro Devices, Inc.

    This file is distributed under the University of Illinois Open Source
    License. See LICENSE.TXT for details.
*/

#ifdef TRISYCL_DEBUG
#include <boost/log/trivial.hpp>
#include <iostream>
#include <typeinfo>
#include <sstream>
#include <string>
// To be able to construct string literals like "blah"s
using namespace std::string_literals;

/** Dump a debug message in a formatted way.

    Use an intermediate ostringstream because there are issues with
    BOOST_LOG_TRIVIAL to display C strings
*/
#define TRISYCL_DUMP(expression) do {                \
    std::ostringstream s;                            \
    s << expression;                                 \
    BOOST_LOG_TRIVIAL(debug) << s.str();             \
  } while(0)
/// Same as TRISYCL_DUMP() but with thread id first
#define TRISYCL_DUMP_T(expression)                                      \
  TRISYCL_DUMP("Thread " << std::ios_base::hex                          \
               << std::this_thread::get_id() << ": " << expression)
#else
#define TRISYCL_DUMP(expression) do { } while(0)
#define TRISYCL_DUMP_T(expression) do { } while(0)
#endif

namespace cl {
namespace sycl {
namespace trisycl {

/** \addtogroup debug_trace Debugging and tracing support
    @{
*/

/** Class used to trace the construction, copy-construction,
    move-construction and destruction of classes that inherit from it

    \param T is the real type name to be used in the debug output.
 */
template <typename T>
struct Debug {
#ifdef TRISYCL_DEBUG
  /// Trace the construction with the compiler-dependent mangled named
  Debug() {
    TRISYCL_DUMP("Constructor of " << typeid(*this).name()
                 << " " << (void*) this);
  }


  /// Trace the copy construction with the compiler-dependent mangled
  /// named
  Debug(Debug const &) {
    TRISYCL_DUMP("Copy of " << typeid(*this).name() << " " << (void*) this);
  }


  /// Trace the move construction with the compiler-dependent mangled
  /// named
  Debug(Debug &&) {
    TRISYCL_DUMP("Move of " << typeid(*this).name() << " " << (void*) this);
  }


  /// Trace the destruction with the compiler-dependent mangled named
  ~Debug() {
    TRISYCL_DUMP("~ Destructor of " << typeid(*this).name()
                 << " " << (void*) this);
  }
#endif
};


/** Class used to display a vector-like type of classes that inherit from
    it

    \param T is the real type name to be used in the debug output.

    Calling the display() method dump the values on std::cout
 */
template <typename T>
struct DisplayVector {
  /// To debug and test
  void display() const {
    std::cout << typeid(T).name() << ":";
    // Get a pointer to the real object
    for (auto e : *static_cast<const T *>(this))
      std::cout << " " << e;
    std::cout << std::endl;
  }
};

/// @} End the debug_trace Doxygen group

}
}
}

/*
    # Some Emacs stuff:
    ### Local Variables:
    ### ispell-local-dictionary: "american"
    ### eval: (flyspell-prog-mode)
    ### End:
*/
