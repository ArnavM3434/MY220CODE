#!/usr/bin/env python3
# test.py - run MP2 tests or run the calculator interactively
# Usage:
# python test.py
#   Runs the user's program interactively by inputting a line and then sending
#   that line to the simulator. Eliminates extra unnecessary messages generated
#   by the simulator and allows the user to re-run the program with new inputs
#   by simply entering a new calculation.
#
# python test.py test
#   Run all of the test cases provided in the assignment page.

# Copyright (c) 2022 Aidan Yaklin

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import subprocess
import time
import sys

def run_sim(msg):
    proc = subprocess.Popen(['lc3sim', '-s', 'test.sim'], stdout=subprocess.PIPE, stdin=subprocess.PIPE)
    # skip through the header
    for i in range(18):
        proc.stdout.readline()

    proc.stdin.write(msg[0:1])
    proc.stdin.flush()

    for i in range(4):
        proc.stdout.readline()
    
    for i in range(1, len(msg)):
        proc.stdin.write(msg[i:i+1])
        try:
            proc.stdin.flush()
        except BrokenPipeError:
            break
        print('.', end='', flush=True)
        time.sleep(0.1)

    result = proc.stdout.readline()

    try:
        proc.communicate(timeout=1) # read everything else it has to say
    except subprocess.TimeoutExpired:
        proc.terminate()

    print()
    return result.decode('ascii').strip()

def run_asm():
    subprocess.run(['lc3as', 'prog2.asm'])

def interactive():
    try:
        while True:
            print("> ", end='', flush=True)
            msg = input() + '='
            msg = msg.encode('ascii')
            print(run_sim(msg))
    except (KeyboardInterrupt, EOFError):
        print()

def run_tests():
    test_inputs = [
        '3',
        '',
        '1 2 4 + 5 + +',
        '1 2 3 4 +',
        '1 2 + + +',
        '2 1 -',
        '2 5 -',
        '7 4 *',
        '4 2 - 1 3 - *',
        '1 4 - 3 2 - *',
        '1 3 - 2 5 - *',
        '4 2 /',
        '1 2 /',
        '5 3 + 4 / 3 ^ 1 *',
        '5 1 2 + 4 * + 3 -',
        '2 2 ^ 2 ^'
    ]
    test_expected = [
        '3=0003',
        '=Invalid Expression',
        '1 2 4 + 5 + +=000C',
        '1 2 3 4 +=Invalid Expression',
        '1 2 + +Invalid Expression',
        '2 1 -=0001',
        '2 5 -=FFFD',
        '7 4 *=001C',
        '4 2 - 1 3 - *=FFFC',
        '1 4 - 3 2 - *=FFFD',
        '1 3 - 2 5 - *=0006',
        '4 2 /=0002',
        '1 2 /=0000',
        '5 3 + 4 / 3 ^ 1 *=0008',
        '5 1 2 + 4 * + 3 -=000E',
        '2 2 ^ 2 ^=0010'
    ]

    fails = 0
    for i in range(len(test_inputs)):
        print("test %d" % i)
        msg = test_inputs[i] + '='
        result = run_sim(msg.encode('ascii'))
        if result != test_expected[i]:
            print("FAIL")
            print("-> Input was: %s" % msg)
            print("-> Expected : %s" % test_expected[i])
            print("-> Your code: %s" % result)
            fails += 1
        else:
            print("OK")
    print("%d / %d tests failed" % (fails, len(test_inputs)))


def main():
    with open('test.sim', 'w') as f:
        f.write('f prog2\nc\n')
    run_asm()

    if len(sys.argv) > 1:
        if sys.argv[1] == 'test':
            run_tests()
        else:
            print("Usage: %s [test]" % sys.argv[0])
            print("Options:")
            print("    test: run tests")
            print()
            print("Otherwise, run the calculator interactively")
    else:
        interactive()


if __name__ == "__main__":
    main()