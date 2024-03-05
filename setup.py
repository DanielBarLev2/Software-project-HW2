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
