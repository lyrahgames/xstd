# Package Library
./: lib{lyrahgames-xstd}: lyrahgames/xstd/hxx{**}
{
  cxx.export.poptions = "-I$out_root" "-I$src_root"
}
hxx{**}: install.subdirs = true

# Other Default Targets
./: tests/ manifest doc{README.md} legal{COPYING}

tests/: install = false
