.DEFAULT_GOAL := all

%::
	$(MAKE) -j -C src/ $@
