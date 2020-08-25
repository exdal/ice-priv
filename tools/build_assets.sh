#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

if [ -z "$1" ]
then
    echo "Usage: ./tools/build_assets.sh {Project Name}"
else
    cd $DIR && cd ../$1
    ../tools/AssetCompiler.linux ./Assets ./.build/
fi
