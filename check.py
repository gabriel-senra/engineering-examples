"""Run host checks with finite timeouts; requires g++ and Node 24+."""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parent
with tempfile.TemporaryDirectory() as scratch:
    binary = str(Path(scratch) / 'engineering-tests')
    subprocess.run(['g++', '-std=c++17', '-pthread', '-Wall', '-Wextra',
                    '-Wpedantic', '-Werror', '-O2', '-Iinclude',
                    'tests/test_samples.cpp', '-o', binary],
                   cwd=root, check=True, timeout=60)
    subprocess.run([binary], cwd=root, check=True, timeout=30)
    subprocess.run(['node', '--test', 'diagram/render.test.mjs'],
                   cwd=root, check=True, timeout=30)
