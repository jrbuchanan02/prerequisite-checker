source_extension = .c++
extern_extension = .cpp

object_extension = .o

define extension = 
	$(if (,$(findstring yaml,$(1))),$(extern_extension),$(source_extension))
endef

source_directories = src/
extern_directories = yaml-cpp/src/

include_directories = $(source_directories) $(extern_directories) yaml-cpp/include

source_files = $(foreach dir, $(source_directories), $(wildcard $(dir)*$(source_extension)))
extern_files = $(foreach dir, $(extern_directories), $(wildcard $(dir)*$(extern_extension)))

executable_name_base = prerequisite-checker

mingw_extension = .exe
linux_extension = 

windows: version = mingw
linux: version = linux

windows: mkdir_arg = -Force -Name
linux: mkdir_arg = -p

executable_name = $(executable_name_base)$($(version)_extension)

bin_location = bin/

object_files = $(foreach file, $(source_files) $(extern_files), $(patsubst %$(call $(extension),$(file)),$(bin_location)%$(object_extension),$(file)))

include_flags = $(foreach include, $(include_directories), -I $(include))
warning_flags = -Wall -Wpedantic -Wextra

windows: compiler = g++
linux: compiler = g++-10
compiler_switches = --std=c++20 -O3 $(include_flags) $(warning_flags)

define source_from_bin_name = 
	$(patsubst $(bin_location)%,%,$(patsubst %$(object_extension),%$(call $(extension),@),$@))
endef

define compile_bin_file = 
	$(compiler) $(compiler_switches) -c $(source_from_bin_name) -o $@
endef

$(object_files) : $(source_files) $(extern_files)
	@echo Compiling file $@
	@$(compile_bin_file)

build: $(source_files) $(extern_files) $(object_files)
	@echo Linking...
	@$(compiler) $(compiler_switches) $(object_files) -o $(executable_name)
windows: $(object_files) build
	@echo Done.
linux: build