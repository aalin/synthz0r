default: synthz0r

synthz0r: FORCE
	$(MAKE) -j -C src/

clean:
	$(MAKE) -C src/ clean

FORCE:
