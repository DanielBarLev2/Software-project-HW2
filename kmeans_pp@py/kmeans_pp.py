import pandas as pd
import numpy as np
import sys


def sys_arguments():
    """
    Parses command-line arguments for K-means clustering.

    Expects at least five arguments:
    1. K (int): Number of clusters.
    2. Iter (int): Maximum number of iterations (optional, default is 300).
    3. Eps (float): Threshold to declare convergence (optional, default is 1.1).
    4. File_name_1 (str): Path to the first CSV file.
    5. File_name_2 (str): Path to the second CSV file.

    Returns:
    - k (int): Number of clusters.
    - iter (int): Maximum number of iterations.
    - eps (float): Threshold to declare convergence.
    - file_name_1 (str): Path to the first CSV file.
    - File_name_2 (str): Path to the second CSV file.
    """
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


def inner_join(file_name_1: str, file_name_2: str) -> pd.DataFrame:
    """
    Perform an inner join operation on two CSV files.
    Then, sort the data and trim the first column.

    Parameters:
    - file_name_1 (str): The path to the first CSV file.
    - file_name_2 (str): The path to the second CSV file.

    Returns:
    - pd.DataFrame: The result of the inner join operation.
    """

    data1 = pd.read_csv(file_name_1, header=None)
    data2 = pd.read_csv(file_name_2, header=None)

    data = pd.merge(data1, data2, on=data1.columns[0], how='inner')
    data.sort_values(by=0)
    data = data[data.columns[1:]]

    return data


def initialize_centroid(vectors_list, k: int) -> np.ndarray:
    """
    Initializes centroids for k-means clustering.

    Parameters:
        vectors_list (np.ndarray): A numpy array containing the vectors.
        k (int): The number of centroids to initialize.

    Returns:
        np.ndarray: An array containing the initialized centroids.
    """

    np.random.seed(0)

    # initialize centroids_list with a random vector
    centroids_list = [vectors_list[np.random.choice(len(vectors_list))]]

    while len(centroids_list) < k:
        # initialize an array to store distances from each vector to the nearest centroid
        distances = np.zeros(len(vectors_list))

        # calculate the minimum distance to any centroid in centroids_list for each vector
        for i, x in enumerate(vectors_list):
            min_distance = np.inf

            # iterate over existing centroids to find the minimum distance
            for c in centroids_list:
                distance = np.linalg.norm(x - c)

                if distance < min_distance:
                    min_distance = distance

            distances[i] = min_distance

        # calculate probabilities for selecting new centroids based on distances
        probabilities = distances / np.sum(distances)

        # choose a new centroid index using the calculated probabilities
        new_centroid_index = np.random.choice(len(vectors_list), p=probabilities)
        new_centroid = vectors_list[new_centroid_index]
        centroids_list.append(new_centroid)

    return np.array(centroids_list)


def kmeans():
    # to run in cmd use: main.py k n d "input++/input_1_db_1.txt" "input++/input_1_db_2.txt"
    # k, iter, eps, file_name_1, file_name_2 = sys_arguments()

    # to run internally:
    k, iter, eps, file_name_1, file_name_2 = 3, 0, 0, "input++/input_1_db_1.txt", "input++/input_1_db_2.txt"

    data = inner_join(file_name_1, file_name_2)

    vectors_list = data.values

    centroids_list = initialize_centroid(vectors_list=vectors_list, k=k)

    print(centroids_list)


if __name__ == "__main__":
    kmeans()
