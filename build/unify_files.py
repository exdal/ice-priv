#!/usr/bin/env python
# Copyright 2017 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import argparse
import sys


def main(argv):
    parser = argparse.ArgumentParser()
    parser.add_argument('-o', '--output')
    parser.add_argument("-p", "--precompiled_header")
    parser.add_argument('files', nargs='+')
    args = parser.parse_args(argv)

    filenames = []
    for f in args.files:
        if f.startswith('@'):
            with open(f[1:]) as fp:
                for l in fp.readlines():
                    filenames.extend(l.split())
        else:
            filenames.append(f)

    out_fp = sys.stdout
    try:
        if args.output != '-':
            out_fp = open(args.output, 'w')

        if (args.precompiled_header):
            out_fp.write("#include \"" + args.precompiled_header + "\"\n")

        for f in filenames:
            out_fp.write("#include \"" + f + "\"\n")
    finally:
        if out_fp != sys.stdout:
            out_fp.close()


if __name__ == '__main__':
    main(sys.argv[1:])
