#!/bin/sh -e

echo "Running autoheader..."
autoheader
echo "Running autoconf..."
autoconf
echo "Done."
