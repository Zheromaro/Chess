.PHONY: help clean clean_all build rebuild release run brun \
        test_ctest test_direct test_dir

.DEFAULT_GOAL := help

# ==========================================================
# 📦 Configuration
# ==========================================================
BROWSER := python3 -c "$$BROWSER_PYSCRIPT"
INSTALL_LOCATION := ~/.local
VCPKG_ROOT := /home/zahrawi/Projects/EnvironmentDependencies/vcpkg
VCPKG_TOOLCHAIN := $(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake

# necessary for vcpkg
export CC := /usr/bin/gcc
export CXX := /usr/bin/g++
export CFLAGS := -Wno-error=override-init

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
    match = re.match(r'^([a-zA-Z0-9_.-]+):.*?## (.*)$$', line)
    if match:
        target, help = match.groups()
        print("%-25s %s" % (target, help))
endef
export PRINT_HELP_PYSCRIPT


# ==========================================================
# 🧭 General Help
# ==========================================================
help:
	@python3 -c "$$PRINT_HELP_PYSCRIPT" < $(MAKEFILE_LIST)

# ==========================================================
# 🧹 Cleaning
# ==========================================================
clean: ## Clean everything in build dir except vcpkg_installed
	@if [ -d build ]; then \
	  find build -mindepth 1 -maxdepth 1 ! -name vcpkg_installed -exec rm -rf {} +; \
	fi

clean_all: ## Clean the entire build directory
	rm -rf build/

# ==========================================================
# 🏗️ Build Modes
# ==========================================================
build: ## Build the project (default, no tests)
	cmake -B build -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_TOOLCHAIN) \
	      -DCMAKE_C_COMPILER=/usr/bin/gcc \
	      -DCMAKE_INSTALL_PREFIX=$(INSTALL_LOCATION) \
	      -DCHESS_BUILD_EXECUTABLE=ON \
	      -DCHESS_BUILD_HEADERS_ONLY=OFF \
	      -DCHESS_ENABLE_UNIT_TESTING=OFF
	cmake --build build --config Release

rebuild: clean build ## Clean and rebuild the project

release: clean_all ## Clean and rebuild for release
	cmake -B build -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_TOOLCHAIN) \
	      -DCMAKE_C_COMPILER=/usr/bin/gcc \
		  -DCMAKE_INSTALL_PREFIX=$(INSTALL_LOCATION) \
	      -DCHESS_BUILD_EXECUTABLE=ON \
	      -DCHESS_BUILD_HEADERS_ONLY=OFF \
	      -DCHESS_ENABLE_UNIT_TESTING=OFF \
	      -DCMAKE_BUILD_TYPE=Release
	cmake --build build --config Release

# ==========================================================
# 🧪 Unit Testing
# ==========================================================
test_ctest: ## Configure, rebuild, and run CTest
	cmake -B build -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_TOOLCHAIN) \
	      -DCMAKE_INSTALL_PREFIX=$(INSTALL_LOCATION) \
	      -DCHESS_ENABLE_UNIT_TESTING=ON
	cmake --build build --config Release
	cd build && ctest -C Release -VV

test_direct: ## Rebuild and run all test executables directly
	cmake -B build -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_TOOLCHAIN) \
	      -DCMAKE_INSTALL_PREFIX=$(INSTALL_LOCATION) \
	      -DCHESS_ENABLE_UNIT_TESTING=ON
	cmake --build build --config Release
	@for test in build/test/*_Test; do \
	  echo ">>> Running $$test"; \
	  $$test --gtest_color=yes || exit 1; \
	done

test_dir: ## Run one test executable (use DIR=dir/name)
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

# ==========================================================
# 🚀 Run the project
# ==========================================================
run: ## Run the executable
	./build/bin/Release/CHESS || ./build/bin/Debug/CHESS

brun: build run ## Build and run
