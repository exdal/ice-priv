#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

git submodule update --init


if [ -z "$1" ]
then
    cd $DIR && cd ..
    ./tools/genie.linux --gcc=linux-gcc gmake
else
    cd $DIR && cd ..
    ./tools/genie.linux $1
fi
