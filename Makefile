all:
	make -C src
	cp -r src/build ./
clean:
	rm -rf build
	rm -rf src/build
