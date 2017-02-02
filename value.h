#pragma once

typedef enum {
  integer_t, float_t, string_t, char_t, void_t
} value_type_t;

struct _value {
  value_type_t type;
  union {
    int as_i;
    float as_f;
    char* as_s;
    char as_c;
  } data;
};
typedef struct _value value_t ;
extern const value_t val_null;
