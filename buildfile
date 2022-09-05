# Package Library
./: lyrahgames/xstd/ manifest doc{README.md} legal{COPYING}

# Other Default Targets
./: tests/: include = $config.lyrahgames_xstd.test
tests/: install = false
