# Package Library
./: manifest doc{README.md} legal{COPYING}
./: lib{lyrahgames-xstd}: lyrahgames/xstd/hxx{**}
{
  cxx.export.poptions = "-I$out_root" "-I$src_root"
}
hxx{**}: install.subdirs = true

# Other Default Targets
./: tests/: include = $config.lyrahgames_xstd.test
tests/: install = false
