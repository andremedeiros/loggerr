# Structure

    bundle gem loggerr
    cd loggerr
    rspec --init
    mkdir -p ext/loggerr
   
# Dependencies

In `loggerr.gemspec`, after `spec.email`:

    spec.extensions    = ["ext/loggerr/extconf.rb"]

In `loggerr.gemspec`, before `end`:

    spec.add_development_dependency "rake-compiler"
    spec.add_development_dependency "rspec"

# Define the extension

In `ext/loggerr/extconf.rb`:

    require 'mkmf'
    create_makefile('loggerr')

Create `ext/loggerr/loggerr.c` and `ext/loggerr/loggerr.h`.

# Ruby initialization

In `Rakefile`:

    require "bundler/gem_tasks"
    require "rake/extensiontask"
    require "rspec/core/rake_task"
    
    Rake::ExtensionTask.new("loggerr") do |extension|
      extension.lib_dir = "lib/loggerr"
    end
    
    RSpec::Core::RakeTask.new('spec') do |spec|
      spec.verbose = true
    end
    
    task :default => [:clean, :compile, :spec]


In `ext/loggerr/loggerr.h`:

    #include <ruby.h>
    
    void Init_loggerr();
    
    static VALUE rb_parse(VALUE self, VALUE path);

In `ext/loggerr/loggerr.c`:

    #include "loggerr.h"
    
    VALUE LoggerrModule;
    
    void Init_loggerr() {
      LoggerrModule = rb_define_module("Loggerr");
      rb_define_singleton_method(LoggerrModule, "parse", RUBY_METHOD_FUNC(rb_parse), 1);
    }
    
    static VALUE rb_parse(VALUE self, VALUE path) {
      return Qnil;
    }

Run `rake` on the console to show the process running.

Explain that this is the basic structure to:

* Initialize a ruby extension
* Define a Module inside that extension
* Define a singleton method (class method)

# Tests

On the top of `spec/spec_helper.rb`, add:

    require 'loggerr'
    
Create `spec/loggerr_spec.rb` and add:

    require 'spec_helper'
    
    describe Loggerr do
      describe '.parse' do
        it 'should raise an exception when an invalid path is passed' do
          expect { Loggerr.parse('/this/does/not/exist') }.to raise_error
        end
    
        it 'should return a hash if a valid path is passed' do
          expect( Loggerr.parse('spec/fixtures/log.txt') ).to be_a Hash
        end
      end
    end


Show the console output. There's an error, which tells us our extension isn't being loaded.

Replace `lib/loggerr.rb` with:

    require 'loggerr/loggerr'

Finally, implement the parser:

`loggerr.h`:

    #include <ruby.h>
    #include <stdio.h>
    #include <float.h>
    
    #define MIN(a,b) (a < b) ? a : b;
    #define MAX(a,b) (a > b) ? a : b;
    
    void Init_loggerr();
    
    static VALUE rb_parse(VALUE self, VALUE path);


`loggerr.c` (inside `rb_parse`):

    Check_Type(path, T_STRING);
    
    char *_path = StringValueCStr(path);
    FILE *fp = fopen(_path, "r");
    
    if(!fp)
        rb_raise(rb_eException, "File not found.");
    
    char *uri = malloc(4096);
    double time;
    int response_code;
    
    double total_time = 0;
    double min_time   = DBL_MAX;
    double max_time   = 0;
    int line_count    = 0;
    int error_count   = 0;
    
    while(!feof(fp)) {
        if( fscanf(fp, "%s %lgms. %d", uri, &time, &response_code) < 3 )
            continue;/Users/andremedeiros/Documents/talk.pdf

        if(response_code >= 500)
            error_count++;

        total_time += time;
        min_time = MIN(min_time, time);
        max_time = MAX(max_time, time);

        line_count++;
    }

    fclose(fp);

    VALUE result = rb_hash_new();
    rb_hash_aset(result, rb_str_new2("average"), DBL2NUM(total_time / line_count));
    rb_hash_aset(result, rb_str_new2("count"),   INT2NUM(line_count));
    rb_hash_aset(result, rb_str_new2("errors"),  INT2NUM(error_count));
    rb_hash_aset(result, rb_str_new2("min"),     DBL2NUM(min_time));
    rb_hash_aset(result, rb_str_new2("max"),     DBL2NUM(max_time));

    return result;
    
Run `rake` to show tests passing. That's your first extension.