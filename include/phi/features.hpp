#pragma once

// String view header
#if !defined(__has_include)
#define __has_include(_header) 0
#endif

#if __cpp_lib_string_view || __has_include(<string_view>)

#include <string_view>

using std_string_view = std::string_view;
using namespace std::string_view_literals;
#elif __cpp_lib_experimental_string_view || __has_include(<experimental/string_view>)

#include <experimental/string_view>

using std_string_view = std::experimental::string_view;
using namespace std::experimental::string_view_literals;
#endif

// Warning for unused return values
#if defined(__GNUC__) || defined(__clang__)
# define WARN_UNUSED(_type) __attribute__((warn_unused_result)) _type
#else
# define WARN_UNUSED(_type) _type
#endif
