#!/bin/bash
# Setup script for IDEs

import subprocess
import platform
import argparse
import shutil
import os

# Supported IDEs
IDEs = [
    "vs2017",
    "vs2019",

    "vscode",
]

# Target platforms
Platforms = [
    "android"
]

parser = argparse.ArgumentParser(
    description='IDE config helper script.')

parser.add_argument('--ide', choices=IDEs, type=str, required=True)
parser.add_argument('--target', choices=Platforms, type=str, required=False)

args = parser.parse_args()


def GenerateARGS(platform, target):
    if not os.path.isdir("./out"):
        os.mkdir("./out")

    shutil.copyfile(f"./.ide/GN/{platform}/{target}/args.gn",
                    "./out/args.gn")

    subprocess.call([f"./tools/{platform}/gn",
                     "gen",
                     "out"])


def GenerateVSCode(platform, target):
    GenerateARGS(platform, target)
    if os.path.isdir("./.vscode"):
        shutil.rmtree("./.vscode")

    shutil.copytree(f"./.ide/vscode/{platform}/{target}", "./.vscode")


def GenerateVisualStudio(platform, target, version):
    GenerateARGS(platform, target)
    subprocess.call([f"./tools/{platform}/gn",
                     "gen",
                     "out", f"--ide=vs{version}"])


def main():
    is_linux = platform.system() == "Linux"
    is_win = platform.system() == "Windows"

    target_platform = args.target
    host_platform = ""

    if is_linux:
        host_platform = "linux"
    elif is_win:
        host_platform = "win"
    else:
        print("Unsupported platform!")
        return 1

    if target_platform == None:
        target_platform = "pc"

    if args.ide == "vscode":
        GenerateVSCode(host_platform, target_platform)
    elif args.ide.startswith("vs20"):
        GenerateVisualStudio(host_platform, target_platform,
                             args.ide.split("vs")[1])


if __name__ == "__main__":
    exit(main())
