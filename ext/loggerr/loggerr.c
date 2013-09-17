#include "loggerr.h"

VALUE LoggerrModule;

void Init_loggerr() {
  LoggerrModule = rb_define_module("Loggerr");
  rb_define_singleton_method(LoggerrModule, "parse", RUBY_METHOD_FUNC(rb_parse), 1);
}

VALUE rb_parse(VALUE self, VALUE path) {
  Check_Type(path, T_STRING); // :-(

  char *_path = StringValueCStr(path);
  FILE *fp = fopen(_path, "r");

  if(!fp)
    rb_raise(rb_eException, "File does not exist or could not be opened.");

  char *uri = malloc(4096);
  double time;
  int response_code;

  double total_time = 0;
  double min_time   = DBL_MAX;
  double max_time   = 0;
  int line_count    = 0;
  int error_count   = 0;

  while(!feof(fp)) {
    if( fscanf(fp, "%s %lfms. %d", uri, &time, &response_code) < 3 )
      continue;

    if(response_code >= 500)
      error_count++;

    total_time += time;
    min_time = MIN(min_time, time);
    max_time = MAX(max_time, time);

    line_count++;
  }

  fclose(fp);

  VALUE result = rb_hash_new();
  rb_hash_aset(result, rb_str_new2("average"),  DBL2NUM(total_time / line_count));
  rb_hash_aset(result, rb_str_new2("count"),    INT2NUM(line_count));
  rb_hash_aset(result, rb_str_new2("errors"),   INT2NUM(error_count));
  rb_hash_aset(result, rb_str_new2("min"),      DBL2NUM(min_time));
  rb_hash_aset(result, rb_str_new2("max"),      DBL2NUM(max_time));

  return result;
}

