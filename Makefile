all:
	cd src && qmake
	$(MAKE) -C src all

clean:
	$(MAKE) -C src clean
	rm -f src/Makefile
	rm -f src/.qmake.stash
	rm -rf bin
	rm -rf build
