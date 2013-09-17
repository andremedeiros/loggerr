#include <ruby.h>
#include <stdio.h>
#include <float.h>

#define MIN(a, b) a < b ? a : b;
#define MAX(a, b) a > b ? a : b;

void Init_loggerr();
VALUE rb_parse(VALUE self, VALUE path);
