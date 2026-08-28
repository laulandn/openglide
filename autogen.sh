#!/bin/sh
aclocal
autoconf
autoheader
glibtoolize
automake --add-missing
