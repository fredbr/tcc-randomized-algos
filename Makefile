clean:
	rm -rf build benchmark/build

install: clean
	mkdir -p build/include/ build/lib/
	cp -r pcg-cpp/include build/include/pcg_cpp/

	cd benchmark && cmake -E make_directory "build" && \
	cmake -E chdir "build" cmake -DBENCHMARK_DOWNLOAD_DEPENDENCIES=on -DCMAKE_BUILD_TYPE=Release ../ && \
	cmake --build "build" --config Release

	cp benchmark/build/src/libbenchmark.a build/lib/
	cp benchmark/build/src/libbenchmark_main.a build/lib/

	cp -r benchmark/include/benchmark build/include/
