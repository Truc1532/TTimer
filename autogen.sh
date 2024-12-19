#!/bin/bash

set -e

echo "Generating configure and makefile..."
autoreconf --install

echo "You can now run './configure' and 'make'"
