import pandas as pd
import numpy as np
import sys


def sys_arguments():
    if len(sys.argv) < 5:
        print("not enough arguments")
        sys.exit(1)

    try:
        k = int(sys.argv[1])
        if len(sys.argv) > 2:
            iter = int(sys.argv[2])
        else:
            iter = 300
        if len(sys.argv) > 3:
            eps = float(sys.argv[3])
        else:
            eps = 1.1
        file_name_1 = sys.argv[4]
        file_name_2 = sys.argv[5]

    except ValueError:
        print("Invalid arguments")
        sys.exit(1)

    return k, iter, eps, file_name_1, file_name_2


def inner_join(file_1, file_2):
    data1 = pd.read_csv(file_1)
    data2 = pd.read_csv(file_2)

    result = pd.merge(data1, data2, on=data1.columns[0], how='inner')

    return result


if __name__ == "__main__":
    file_name_1 = "file1.csv"
    file_name_2 = "file2.csv"
    joined_data = inner_join(file_name_1, file_name_2)
    print(joined_data)




