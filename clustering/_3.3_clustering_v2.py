"""
Question 1 Description:
In this programming problem and the next you'll code up the clustering algorithm
 from lecture for computing a max-spacing k-clustering. Download the text file 
 here. This file describes a distance function (equivalently, a complete graph 
 with edge costs). It has the following format:
[number_of_nodes]
[edge 1 node 1] [edge 1 node 2] [edge 1 cost]
[edge 2 node 1] [edge 2 node 2] [edge 2 cost]

There is one edge (i,j) for each choice of 1<=i<j<=n, where n is the number of 
nodes. For example, the third line of the file is "1 3 5250", indicating that
 the distance between nodes 1 and 3 (equivalently, the cost of the edge (1,3))
 is 5250. You can assume that distances are positive, but you should NOT assume
 that they are distinct.

Your task in this problem is to run the clustering algorithm from lecture on 
this data set, where the target number k of clusters is set to 4. What is the 
maximum spacing of a 4-clustering?
 

Question 2 Description:
In this question your task is again to run the clustering algorithm from 
lecture, but on a MUCH bigger graph. So big, in fact, that the distances (i.e.,
 edge costs) are only defined implicitly, rather than being provided as an 
 explicit list.
The data set is here. The format is:
[# of nodes] [# of bits for each node's label]
[first bit of node 1] ... [last bit of node 1]
[first bit of node 2] ... [last bit of node 2]

For example, the third line of the file "0 1 1 0 0 1 1 0 0 1 0 1 1 1 1 1 1 0 1 0 1 1 0 1" 
denotes the 24 bits associated with node #2.

The distance between two nodes u and v in this problem is defined as the Hamming
 distance--- the number of differing bits --- between the two nodes' labels. For
 example, the Hamming distance between the 24-bit label of node #2 above and the
 label "0 1 0 0 0 1 0 0 0 1 0 1 1 1 1 1 1 0 1 0 0 1 0 1" is 3 (since they differ
 in the 3rd, 7th, and 21st bits).

The question is: what is the largest value of k such that there is a k-clustering
 with spacing at least 3? That is, how many clusters are needed to ensure that no
 pair of nodes with all but 2 bits in common get split into different clusters?
"""
import itertools
import functools
import numpy as np
from typing import List

class Unionfind:
    def __init__(self, num_nodes: int):
        self.n = num_nodes
        self.unions = [0] * (1 + self.n)

    def find(self, node: int):
        '''
        if node doesn't belong to any union, return 0;
        otherwisem, return the index of the union it belongs to.
        '''
        if not self.unions[node]:
            return 0

        if self.unions[node] != node:
            self.unions[node] = self.find(self.unions[node])

        return self.unions[node]

    def union(self, node1: int, node2: int):
        '''
        if two nodes don't belong to any union, assign a union index to them;
        if either belongs to a union and the other doesn't belong to any union, attach the latter one to the same union;
        if two nodes belong to different union, merge them into one.

        node1: integer
        node2: integer
        '''
        if not self.unions[node1]:
            if self.unions[node2]:
                self.unions[node1] = self.find(node2)
            else:
                self.unions[node1] = node1
                self.unions[node2] = node1

            self.n -= 1
        else:
            if not self.unions[node2]:
                self.unions[node2] = self.find(node1)
                self.n -= 1
            else:
                union_1 = self.find(node1)
                union_2 = self.find(node2)

                if union_1 != union_2:
                    self.unions[union_2] = union_1
                    self.n -= 1


def explicit_distance(filename: str):
    '''
    given a file with explicit distances between nodes, return a list with all the distances.
    '''
    f = open(filename)
    line = f.readline()
    n = int(line)  #number of nodes
    distances = []

    line = f.readline()
    while line:
        distances.append(tuple(map(lambda x: int(x), line.split())))
        line = f.readline()

    f.close()
    return n, distances


def k_clustering(distance: List[tuple], num_nodes: int, k: int):
    '''
    given the distances between nodes and the number of final clusters, return the space
    '''
    uf = Unionfind(num_nodes)
    i = 0

    while uf.n > k:
        node1, node2, d = distances[i]
        i += 1

        union1 = uf.find(node1)
        union2 = uf.find(node2)

        if not union1 or not union2 or union1 != union2:
            uf.union(node1, node2)

    while i < len(distances):
        node1, node2, d = distances[i]
        union1 = uf.find(node1)
        union2 = uf.find(node2)

        if not union1 or not union2 or union1 != union2:
            return distances[i][2]

        i += 1



def implicit_distance_sorted(filename: str):
    '''
    given a file with implicit distances between nodes, return a list with sorted unique nodes
    '''
    f = open(filename)
    line = f.readline()
    _, bit_size = [int(x) for x in line.split()]  #number of nodes, number of bits for each node

    # convert text into integers and assign them to nodes
    nodes = []
    line = f.readline()
    while line:
        num = line.replace(' ', '')
        nodes.append(int(num, 2))
        line = f.readline()

    f.close()
    nodes= list(set(nodes))
    nodes.sort()
    return bit_size, nodes


def cluster_dfs(nodes: List[int], bit_size: int, space: int):
    '''
    given a sorted nodes list, bit size and the minimum max-space, return the number of clusters
    '''
    # operators with "1" bit no more than the number of space
    bitset_operator = np.array([i for i in generate_mask(bit_size, space)])

    # use the least significant bit to encode visited, that is,
    # the node's value is calculated by n >> 1
    # while the visited flag is calculated by n & 1
    # here, visited means the node belongs to a certain cluster
    node_n_visited = np.array([n << 1 for n in nodes])

    # make sure binary search on node_n_visited always return an index
    # that is in range of node_n_visited by append a big node to its end
    node_n_visited = np.append(node_n_visited, (1 << (bit_size + 1)) + 1)

    amount = 0
    stack = []

    for idx, node in enumerate(nodes):

        # check if the node had been visited
        if node_n_visited[idx] & 1 :
            continue

        node_n_visited[idx] |= 1
        stack.append(node_n_visited[idx])

        # depth first search begins, search any elements the start point could reach in less than "space" distance.
        # the above elements form a frontier, again search any elements this frontier could reach in less than "space" distance.
        while stack:  
            cur_node = stack.pop() >> 1

            dsts = (cur_node ^ bitset_operator) << 1   # find all elements in nodes which XOR cur_node will get a number
                                                       # that belongs to bitset_operator; left shift by 1 on these elements
            idxs = node_n_visited.searchsorted(dsts)      # find the indices of dsts in node_n_visited
            founds = node_n_visited[idxs]
            idxs = idxs[dsts == founds]
            node_n_visited[idxs] |= 1
            stack.extend(node_n_visited[idxs])

        amount = amount + 1

    return amount


def generate_mask_i(n: int, i: int): 
    '''
    generate n-bit binary data with i bits of "1"
    '''
    # enumerate C(i,n)
    for ones in itertools.combinations([1 << b for b in range(n)], i):
        yield functools.reduce(lambda x, y : x | y, ones, 0)


def generate_mask(n: int, m: int):
    '''
    generate n-bit binary data with no more than m bits of "1"
    '''
    for i in range(1, m): 
        for r in generate_mask_i(n, i):
            yield r





filename=["clustering1.txt","clustering_big.txt"]

i = 0
num_nodes, distances = explicit_distance(filename[i])
distances.sort(key = lambda x: x[2])
k = 4
print(k, 'clustering maximum space:', k_clustering(distances, num_nodes, k))


i = 1
space = 3
bit_size, nodes = implicit_distance_sorted(filename[i])
print('the number of clustering with maximum space at least', space, ':', cluster_dfs(nodes, bit_size, space))