#pragma once

#define STRING_DEFAULT_CAPACITY     10

#define string                      char*

#define string_reserve(cap)         _string_reserve(char, cap)
#define string_create()             _string_create(char)

#define string_push_char(s, cstr)   _string_push_(s, cstr)
#define string_push_string(s, cstr) _string_push_string(s, cstr)