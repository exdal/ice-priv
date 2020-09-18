import os
import sys


def Main():
    current = os.path.dirname(__file__)
    old_path = os.getcwd()

    platform = sys.argv[1]  # win, linux, osx
    target = sys.argv[2]

    executable = os.path.realpath("%s/%s/AssetCompiler" % (current, platform))

    os.chdir(target)
    os.system("%s %s ../../out/Assets" % (executable, "./Assets"))
    os.chdir(old_path)


if __name__ == "__main__":
    Main()
