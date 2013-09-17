require "bundler/gem_tasks"
require "rake/extensiontask"
require "rspec/core/rake_task"

Rake::ExtensionTask.new("loggerr") do |ext|
  ext.lib_dir = "lib/loggerr"
end

RSpec::Core::RakeTask.new("spec") do |spec|
  spec.verbose = true
end

task :default => [:clean, :compile, :spec]
