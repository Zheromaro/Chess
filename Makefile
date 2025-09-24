.PHONY: help test_ctest test_direct test_dir clean build rebuild release run brun
.DEFAULT_GOAL := help

define BROWSER_PYSCRIPT
import os, webbrowser, sys

try:
	from urllib import pathname2url
except:
	from urllib.request import pathname2url

webbrowser.open("file://" + pathname2url(os.path.abspath(sys.argv[1])))
endef
export BROWSER_PYSCRIPT

define PRINT_HELP_PYSCRIPT
import re, sys

for line in sys.stdin:
	match = re.match(r'^([a-zA-Z_-]+):.*?## (.*)$$', line)
	if match:
		target, help = match.groups()
		print("%-20s %s" % (target, help))
endef
export PRINT_HELP_PYSCRIPT

BROWSER := python3 -c "$$BROWSER_PYSCRIPT"
INSTALL_LOCATION := ~/.local


help:
	@python3 -c "$$PRINT_HELP_PYSCRIPT" < $(MAKEFILE_LIST)

test_ctest: ## configure, rebuild, run with CTest
	cmake -B build -DCMAKE_INSTALL_PREFIX=$(INSTALL_LOCATION) -DCHESS_ENABLE_UNIT_TESTING=1
	cmake --build build --config Release
	cd build && ctest -C Release -VV

test_direct: ## configure, rebuild, run all test executables directly
	cmake -B build -DCMAKE_INSTALL_PREFIX=$(INSTALL_LOCATION) -DCHESS_ENABLE_UNIT_TESTING=1
	cmake --build build --config Release
	@for test in build/test/*_Test; do \
	  echo ">>> Running $$test"; \
	  $$test --gtest_color=yes || exit 1; \
	done

test_dir: ## run only one test executable: make test_dir DIR=dir/name
	@if [ -z "$(DIR)" ]; then \
	  echo "Usage: make test_dir DIR=foo"; \
	  exit 1; \
	fi
	@executable="build/test/$(DIR)_Test"; \
	if [ -x "$$executable" ]; then \
	  echo ">>> Running $$executable"; \
	  $$executable --gtest_color=yes || exit 1; \
	else \
	  echo "Error: test executable $$executable not found."; \
	  exit 1; \
	fi

clean: ## clean everything in build dir except vcpkg_installed
	@if [ -d build ]; then \
	  find build -mindepth 1 -maxdepth 1 ! -name vcpkg_installed -exec rm -rf {} +; \
	fi

clean_all: ## clean the entire build directory
	rm -rf build/

build: ## creat or complete the build directory
	cmake -Bbuild -DCMAKE_INSTALL_PREFIX=$(INSTALL_LOCATION) -DCHESS_ENABLE_UNIT_TESTING=0 
	cmake --build build --config Release

rebuild: clean build ## clean the build directory and rebuild it

release: clean_all ## clean the build directory and rebuild it for release
	cmake -Bbuild -DCMAKE_INSTALL_PREFIX=$(INSTALL_LOCATION) -DCHESS_ENABLE_UNIT_TESTING=0 -DCMAKE_BUILD_TYPE="Release"
	cmake --build build --config Release

run: ## run the project
	./build/bin/Release/CHESS || ./build/bin/Debug/CHESS

brun: build run ## build and run the project
