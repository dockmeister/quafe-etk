Installation
-----------

### Getting the sourcecode
  
    git clone git@github.com/dockmeister/quafe-etk

### Installing dependencies

* cmake >= 2.8
* gtkmm >= 3.0
* boost >= 1.46
* curl >= 7.0

### Compiling 'quafe-etk'
    cmake .
    make
    su -c "make install"

Usage
-----
quafe-etk supports the following command line arguments:

    -h, --help
    --config-path [=~/.quafe-etk/]
    --plugin-path [=$install_prefix/share/quafe-etk/plugins]
    -m, --minimized
    -s, --silent

Contributing
------------
