default: synthz0r

synthz0r: src
	$(MAKE) -j -C src/

clean:
	$(MAKE) -C src/ clean
