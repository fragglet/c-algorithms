#!/bin/sh

mkdir -p autotools

aclocal
libtoolize
autoheader
automake -a 
autoconf
automake -a

./configure $@

