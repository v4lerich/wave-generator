BUILD_TYPE=Release

.PHONY: build
build:
	cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -B"$(shell pwd)/build/${BUILD_TYPE}" -H.
	cmake --build "$(shell pwd)/build/${BUILD_TYPE}"

.PHONY: run
run: build
	./build/${BUILD_TYPE}/app/wave-generator

.PHONY: build-release run-release
build-release: 
	$(MAKE) build BUILD_TYPE=Release
run-release: 
	$(MAKE) run BUILD_TYPE=Release

.PHONY: build-debug run-debug
build-debug: 
	$(MAKE) build BUILD_TYPE=Debug
run-debug: 
	$(MAKE) run BUILD_TYPE=Debug
