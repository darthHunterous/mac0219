#!/usr/bin/env python3
import sys
import csv
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.lines as mlines

def get_data(threads, array_size):
    """ Return tuple of lists ([mean], [error])
    The list's value on the i-th position represent milliseconds of the
    execution using i IFs.
    """
    mean = {}
    error = {}

    with open('data.csv') as csvfile:
        # columns: ifs,threads,array_sz,mean,lower_ci,upper_ci
        reader = csv.reader(csvfile, delimiter=',')
        for row in reader:
            if row[1] == str(threads) and row[2] == str(array_size):
                h = float(row[3]) - float(row[4])
                _id = int(row[0])
                error[_id] = h
                mean[_id] = float(row[3])

    # create lists ordered by dict key
    mean_list = []
    error_list = []
    for item in sorted(mean.keys()):
        mean_list.append(mean[item])
        error_list.append(error[item])

    return (mean_list, error_list)

def plot_data(threads, array_size, include_zero_if=True,
              save_file=True):
    mean, error = get_data(threads, array_size)

    initial_tick = 0
    if not include_zero_if:
        initial_tick = 1
        mean = mean[1:]
        error = error[1:]

    n = len(mean)

    x_ticks = [str(x) for x in range(initial_tick, n + 1)]

    x_1 = np.arange(1, n+1)

    y_1 = np.array(mean)

    err_1 = np.array(error)

    plt.errorbar(x=x_1, y=y_1, yerr=err_1, color="blue", capsize=3,
                 linestyle="None",
                 marker="s", markersize=7, mfc="blue", mec="blue")

    plt.xticks(x_1, x_ticks, rotation=90)

    plt.xlabel('if\'s encadeados')
    plt.ylabel('tempo (ms)')


    plt.title('{} thread{}, vetor de tamanho {}'.format(
        threads, 's' if int(threads) > 1 else '', array_size))

    # plt.axhline(y=0.2, color='r', linestyle=':', linewidth=1)

    if save_file:
        plt.savefig('plots/{}-{}-{}.png'.format(
            int(include_zero_if), threads, array_size))
    else:
        plt.show()

def get_best_if():

    with open('data.csv') as csvfile:
        # columns: ifs,threads,array_sz,mean,lower_ci,upper_ci
        reader = csv.reader(csvfile, delimiter=',')
        min_upper = {}

        for row in reader:
            if not row[5][0].isdigit(): continue
            upper = float(row[5])
            _id = row[1] + '-' + row[2]

            if _id not in min_upper or upper < min_upper[_id][1]:
                min_upper[_id] = (row[0], float(upper))

    import pprint
    pprint.pprint(min_upper)


def main():
    # get_best_if()
    # return
    if len(sys.argv) not in [3, 4, 5]:
        print(
            'USAGE:     ./plot_data.py THREADS ARRAY_SIZE [ZERO_IF]'
            ' [SAVE_FILE]\n'
            '\n'
            '    ZERO_IF\t\tindicates if zero-if should be included on '
            'the graphic\n'
            '    (default=1)\n\n'
            '    SAVE_FILE\t\tindicates if should save graphic to file '
            'instead of previewing interactively\n'
            '    (default=0)\n'
        )
        return

    include_zero_if = True
    if len(sys.argv) >= 4:
        include_zero_if = sys.argv[3] == '1'


    save_file = False
    if len(sys.argv) == 5:
        save_file = sys.argv[4] == '1'

    plot_data(sys.argv[1], sys.argv[2], include_zero_if, save_file)

main()
