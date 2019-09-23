#!/usr/bin/env python3
from decimal import Decimal

FILE = 'tests/results/{size}-{threads}-{test}.txt'

def get_seconds(filename):
    minutes, seconds = (
        open(filename).read()
        .split()[2][:-7]  # get elapsed time
        .split(':'))

    seconds, centiseconds = seconds.split('.')
    seconds = Decimal(seconds) + Decimal(centiseconds)/100
    minutes = Decimal(minutes)

    return minutes*60 + seconds


def main():
    print('size', 'threads', 'test', 'seconds', sep='\t')

    for size in ['1500', '6000']:
        for i in (0, 8, 16):
            for j in range(20):
                filename = FILE.format(size=size, threads=i, test=j)
                seconds = get_seconds(filename)

                print(size, i, j, seconds, sep='\t')

if __name__ == '__main__':
    main()
