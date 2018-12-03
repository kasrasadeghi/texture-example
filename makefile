SHELL := /bin/bash

default:
.PHONY: default
default: build\:false
	cd build; ./fortress-commander

.PHONY: test
test: build\:true
	cd build; ./fortress-commander_TEST

.PHONY: build
build\:%: cmake\:%
	cd build; make -j2

# usage cmake:true for testing, cmake:false otherwise
.PHONY: cmake\:%
cmake\:%: buildfolder
	cd build; cmake -DTEST=$* ..

.PHONY: buildfolder
buildfolder:
	[[ -d build ]] || mkdir build

.PHONY: clean
clean:
	rm -rf build

graphics:
	cd build; rm -rf Graphics
	cp -r ../fortress-commander/src/Graphics src