#!/bin/sh

mkdir -p autotools

aclocal
libtoolize
automake -a 
autoconf
automake -a

./configure $@

