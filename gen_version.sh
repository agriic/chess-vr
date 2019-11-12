#!/bin/sh

# check for git short hash
version=$(git describe --always | tr '-' '.' 2> /dev/null)


NEW_REVISION="#define VERSION \"$version\""
OLD_REVISION=$(cat "src/Version.hpp" 2> /dev/null)

# Update version.h only on revision changes to avoid spurious rebuilds
if test "$NEW_REVISION" != "$OLD_REVISION"; then
    echo "$NEW_REVISION" > "src/Version.hpp"
fi
