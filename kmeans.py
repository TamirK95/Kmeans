import sys


def counting_coordinates(string):
    """Returns number of coordinates in string."""
    num_of_commas = string.count(',')
    num_of_cords = num_of_commas + 1
    return num_of_cords


def copy_arr1_to_arr2(arr1, arr2, k, num_of_cords):
    """Copy contents of arr1 to arr2."""
    for i in range(k):
        for j in range(num_of_cords):
            arr2[i][j] = arr1[i][j]


def make_all_zero(curr_clusters, k, num_of_cords):
    """Turn all slots of currClusters to zero."""
    for i in range(k):
        for j in range(num_of_cords):
            curr_clusters[i][j] = 0


def is_converged(clusters1, clusters2, k, num_of_cords):
    """Check if clusters1 equals to clusters2, return True if so, or False if not."""
    for i in range(k):
        for j in range(num_of_cords):
            if clusters1[i][j] != clusters2[i][j]:
                return False
    return True


def call_error():
    """Print error message and exit."""
    print("Error in input format.")
    sys.exit()


def print_final_res(res, k, num_of_cords):
    """Print the coordinates as needed for the exercise."""
    for t in range(k):
        for p in range(num_of_cords):
            counter = 1
            num = str(float("{:.4f}".format(res[t][p]))).split(".")
            integ = list(map(str, num[0]))
            frac = list(map(int, num[1]))
            for digit in integ:
                if digit == "-":
                    print(digit, end="")
                elif counter <= 5:
                    print(int(digit), end="")
                    counter += 1
            if counter <= 5:
                print(".", end="")
            for digit in frac:
                if counter <= 5:
                    print(digit, end="")
                    counter += 1
            while counter <= 5:
                print(0, end="")
                counter += 1
            if p == num_of_cords-1:
                print("")
            else:
                print(",", end="")


length = len(sys.argv)
total = 0
maxIter = 200
if (length != 2) and (length != 3):
    call_error()
elif length == 3:  # maxIter is given
    if not sys.argv[2].isdigit():
        call_error()
    maxIter = int(sys.argv[2])
if not sys.argv[1].isdigit():
    call_error()
k = int(sys.argv[1])
if k <= 0 or maxIter <= 0:
    call_error()
firstDP = input()
numOfCords = counting_coordinates(firstDP)
size = 500
DPs = [[0 for y in range(numOfCords)] for x in range(500)]
# taking care of first DP:
firstDPlist = firstDP.split(",")
i = 0
for cord in firstDPlist:
    DPs[0][i] = cord
    i += 1
    total += 1
i = 1
# now for all the rest DPs:
while True:
    try:
        DP = input()
        DPlist = DP.split(",")
        if i + 1 == size:  # need bigger DPs array
            size = size * 2
            tempDPs = [[0 for y in range(numOfCords)] for x in range(size)]  # made new 2d array
            for a in range(int((size/2))):
                for b in range(numOfCords):
                    tempDPs[a][b] = DPs[a][b]
            DPs = tempDPs
        j = 0
        for cord in DPlist:
            DPs[i][j] = cord
            j += 1
            total += 1
        i += 1
    except EOFError:
        break
if k >= i:
    call_error()
# now to delete not-needed slots in DPs
tempDPs = [[0 for y in range(numOfCords)] for x in range(i)]  # made new 2d array
# now to copy all members of DPs:
for a in range(i):
    for b in range(numOfCords):
        tempDPs[a][b] = DPs[a][b]
DPs = tempDPs  # made the conversion, array is in the right size now.
# now DPs contains all DPs, creating array for clusters.
currClusters = [[0 for y in range(numOfCords)] for x in range(k)]
oldClusters = [[0 for y in range(numOfCords)] for x in range(k)]
copy_arr1_to_arr2(DPs, currClusters, k, numOfCords)
make_all_zero(oldClusters, k, numOfCords)
# now to compute the exact clusters, by the algorithm
iterCounter=0
while (not is_converged(currClusters, oldClusters, k, numOfCords)) and (iterCounter < maxIter):
    copy_arr1_to_arr2(currClusters, oldClusters, k, numOfCords)
    iterCounter +=1
    countersArr = [0 for y in range(k)]
    for t in range(k):
        countersArr[t]=0
    make_all_zero(currClusters, k, numOfCords)
    for a in range(k):
        for b in range(numOfCords):
            currClusters[a][b] = 0
    for t in range(i):
        DPoint = DPs[t]  # the point ill search min for
        minDistance = float('inf')
        closestClusterIndx = 0
        for a in range(k):
            cluster = oldClusters[a]
            minCandidate = 0
            for b in range(numOfCords):
                x = float(DPoint[b])-float(cluster[b])
                minCandidate += x*x
            if minCandidate < minDistance:
                closestClusterIndx = a
                minDistance = minCandidate
        countersArr[closestClusterIndx] += 1
        for a in range(numOfCords):
            currClusters[closestClusterIndx][a] += float(DPs[t][a])
    # now to divide by | s_i |
    for t in range(k):
        for a in range(numOfCords):
            currClusters[t][a] = (float(currClusters[t][a]) / int(countersArr[t]))
print_final_res(currClusters, k, numOfCords)
