#!/usr/bin/env python3
"""assertを有効にしてローカル検証をコンパイル・実行する。"""

import argparse
import os
from pathlib import Path
import shlex
import subprocess
import tempfile


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--acl', type=Path, help='任意のACL依存テストも実行する場合のincludeパス')
    args = parser.parse_args()
    test_dir = Path(__file__).resolve().parent / 'local'
    compiler = shlex.split(os.environ.get('CXX', 'g++'))
    flags = ['-std=c++23', '-O2', '-Wall', '-Wextra', '-UNDEBUG']
    with tempfile.TemporaryDirectory(prefix='sora-local-tests-') as directory:
        for source in sorted(test_dir.glob('*.local.cpp')):
            includes = []
            if source.name == 'AtCoderCombination.local.cpp':
                if args.acl is None:
                    print(f'SKIP {source.name} (実行には --acl が必要)', flush=True)
                    continue
                includes = ['-I', str(args.acl.resolve())]
            executable = Path(directory) / source.stem
            subprocess.run(compiler + flags + includes + [str(source), '-o', str(executable)], check=True)
            subprocess.run([str(executable)], check=True)
            print(f'PASS {source.name}', flush=True)


if __name__ == '__main__':
    main()
