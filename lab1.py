from lab1_utilities import *


def get_installations_from_file(file_name):
    content = open(file_name)
    content = content.readlines()
    content.pop(0)
    for i in range(len(content)):
        content[i] = content[i].split('\t')

    Art_installations = []

    for i in content: #Each line
        if i[15] == 'INDOORS\n':
            temp = True
        else:
            temp = False

        current = Installation(i[0], int(i[2]), (float(i[7]),float(i[8])) , temp)
        Art_installations.append(current)
    
    return Art_installations


def euclidean_distance(position1, position2):
    return (((position1[0] - position2[0]) ** 2 + (position1[1] - position2[1]) ** 2) ** 0.5)


def get_adjacency_mtx(installations):
    matrix = []
    counter = -1
    for i in installations:
        counter += 1
        matrix.append([])
        for d in installations:
            if i == d:
                matrix[counter].append(0)
            else:
                if(abs(i.ward - d.ward) < 2):
                    dis = euclidean_distance(i.position, d.position)
                    if(i.indoor or d.indoor):
                        dis *= 1.5
                    matrix[counter].append(dis)
                else:
                    matrix[counter].append(0)
    return matrix


def make_graph(installations):
    names = []
    matrix = get_adjacency_mtx(installations)
    for i in installations:
        names.append(i.name)
    graph = Graph(names, matrix)
    indexes = {}
    for i in range(len(names)):
        indexes[names[i]] = []
        for d in range(len(matrix[i])):
            if matrix[i][d]:
                indexes[names[i]].append(names[d])


    return graph


def find_shortest_path(installation_A, installation_B, graph):
    q = []
    lengths = []
    path = []
    total_paths = {}
    graph = graph
    for i in graph.artwork_to_index:
        if(i == installation_A):
            lengths.append(0)
        else:
            lengths.append(float('inf')) 


    q.append(installation_A)

    while(len(q) > 0): 

        temp_list_lengths = []
        temp_list_names = []

        #Update All Adjacent Lengths
        for i in range(len(lengths)): #For all nodes
            if(graph.adjacency_mtx[graph.artwork_to_index[q[0]]][i]): #If connected to current node
                new_length = lengths[graph.artwork_to_index[q[0]]] \
                             + graph.adjacency_mtx[graph.artwork_to_index[q[0]]][i]
                if(new_length < lengths[i]):
                    lengths[i] = new_length
                    temp_list_names.append(graph.installations[i])
                    temp_list_lengths.append(new_length)

        #Order all the adjacent nodes from least to most lengths
        for i in range(len(temp_list_lengths) - 1):
            for j in range(len(temp_list_lengths) - i - 1):
                if(temp_list_lengths[j] > temp_list_lengths[j + 1]):
                    temp_list_lengths[j],temp_list_lengths[j + 1] = temp_list_lengths[j + 1],temp_list_lengths[j]
                    temp_list_names[j],temp_list_names[j + 1] = temp_list_names[j + 1],temp_list_names[j]
        
        #Queue all the adjacent nodes from least to most lengths
        q.pop(0)
        q.extend(temp_list_names)

    path = []
    current_node = installation_B
    while(current_node != installation_A):
        all_adjacent_indexes = []
        for i in range(len(lengths)):
            if(graph.adjacency_mtx[graph.artwork_to_index[current_node]][i]):
                all_adjacent_indexes.append(i)
        all_differences = []
        for i in all_adjacent_indexes:
            all_differences.append(lengths[graph.artwork_to_index[current_node]] - graph.adjacency_mtx[i][graph.artwork_to_index[current_node]] - lengths[i])
        

        for i in range(len(all_differences)):
            all_differences[i] = abs(all_differences[i])

        temp = all_adjacent_indexes[all_differences.index(min(all_differences))]
        current_node = graph.installations[temp]
        path.append(current_node)
        current_node = path[-1]

    path.reverse()
    path.append(installation_B)
    

    if(lengths[graph.artwork_to_index[installation_B]]):
        return (lengths[graph.artwork_to_index[installation_B]], path)
    else:
        return (None, [])

