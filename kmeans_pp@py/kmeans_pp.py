import os
import pandas as pd
import numpy as np
import sys
import mykmeanssp as mk

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

    argc = len(sys.argv)

    try:
        k = int(sys.argv[1])

        if argc > 5:
            iter = int(sys.argv[2])
            eps = float(sys.argv[3])
            file_name_1 = sys.argv[4]
            file_name_2 = sys.argv[5]
        else:
            iter = 300
            eps = float(sys.argv[2])
            file_name_1 = sys.argv[3]
            file_name_2 = sys.argv[4]

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
    data = data.set_index(0)  # Set the first column as index
    data.sort_index(inplace=True)  # Sort by index

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

def get_centroid_indices(data: pd.DataFrame, centroids_list: np.ndarray) -> list:
    """
    Finds the original indices of the centroids in the data.

    Parameters:
        data (pd.DataFrame): A pandas DataFrame containing the data.
        centroids_list (np.ndarray): An array containing the centroids.

    Returns:
        list: A list containing the original indices of the centroids.
    """
    centroid_indices = []
    for centroid in centroids_list:
        centroid_index = data.index[np.where((data.values == centroid).all(axis=1))][0]  # Get the index value
        centroid_index = int(centroid_index)  # Convert to integer
        centroid_indices.append(centroid_index)
    return centroid_indices


def kmeans():
    # to run in cmd use: main.py k n d "input++/input_1_db_1.txt" "input++/input_1_db_2.txt"
    k, iter, eps, file_name_1, file_name_2 = sys_arguments()

    # to run internally:
    # k, iter, eps, file_name_1, file_name_2 = 15, 750, 0, "/a/home/cc/students/cs/matant2/NEW/Software-project-HW2/inputs++/input_3_db_1.txt", "/a/home/cc/students/cs/matant2/NEW/Software-project-HW2/inputs++/input_3_db_2.txt"

    data = inner_join(file_name_1, file_name_2)
    vectors_list = data.values
    centroids_list = initialize_centroid(vectors_list=vectors_list, k=k)
    centroid_indices = get_centroid_indices(data, centroids_list)
    centroid_indices_str = ','.join(map(str, centroid_indices))

    vectors_list = vectors_list.tolist()
    centroids_list = centroids_list.tolist()

    n = len(vectors_list)
    d = len(vectors_list[0])

    if not 1 < k < n:
        print("Invalid number of clusters!")
        sys.exit(1)

    if not 1 < iter < 1000:
        print("Invalid maximum iteration!")
        sys.exit(1)

    print(centroid_indices_str)

    centroids_list = mk.fit(vectors_list, centroids_list, k, n, d, iter, eps)

    for centroid in centroids_list:
        for sublist in centroid:
            print(",".join("{:.4f}".format(num) for num in sublist))

if __name__ == "__main__":
    kmeans()