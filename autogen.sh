#!/bin/sh

mkdir -p autotools
autoreconf -fi && ./configure "$@"
