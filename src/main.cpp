#include <iostream>
#include <vector>
#include <ctime>
#include <math.h>
#include <tuple>
#include <iterator>
#include <algorithm>
#include <numeric>
// #include <list>

using std::vector;
using std::cout;
using std::endl;
using std::tuple;

// pass 2 array by reference and randomly generate symmetric adjacency matrix for Kn graph
void generateMatrix(float** &mat, int size){

    mat = new float*[size]; // allocate memory for first layer of matrix
    float high = 25.0f; // upper bound for number generation
    float low = 5.0f; // lower bound for number generation
    srand((unsigned int)time(NULL)); // seed pseudo-random number generation

    // allocate memory for second dimension of matrix
     for(int i = 0; i < size; i++){
        mat[i] = new float[size]; // matrix is size (n x n)
     }

    // generate edge weights between vertices
    for(int i = 0; i < size; i++){

        mat[i][i] = 0.0f; // length to itself always zero
        for(int j = i + 1; j < size; j++){ // traverse over main diagonal

            float random = low + static_cast <float> ( (nearbyintf(rand() / static_cast <float> (RAND_MAX/(high - low)) * 10.0f)) / 10.0f ); // generate edge weight and round to one decimal place
            mat[i][j] = random; // apply edge weight
            mat[j][i] = random; // ...symmetrically
        }
    }
}

//returns
vector<tuple<int, float>> nearestNeighbor(float** &mat, int first, int v, vector<int> remaining){

    //no other options left
    if(remaining.size() == 0){

        return {{first, mat[first][v]}};
    }

    //find nearest neighbor(s)
    vector<int> nearest = {}; //vector in case of ties
    float min = __FLT_MAX__; //keep track of min value separately

    for(int i = 0; i < remaining.size(); i++){

        float weight = mat[remaining[i]][v];

        if(weight < min){
            nearest = {remaining[i]};
            min = weight;
        } else if (weight == min){
            nearest.push_back(remaining[i]);
        }
    }

    //find lowest total weight of all ties. call function recursively
    vector<tuple<int, float>> shortestFromTies = {{-1, __FLT_MAX__}};

    for(int i = 0; i < nearest.size(); i++){

        vector<int> newRemaining = remaining;
        newRemaining.erase(std::remove(newRemaining.begin(), newRemaining.end(), nearest[i]), newRemaining.end());
        vector<tuple<int, float>> result = nearestNeighbor(mat, first, nearest[i], newRemaining);
        result.insert(result.begin(), {nearest[i], std::get<1>(result[0]) + min});

        if(std::get<1>(result[0]) < std::get<1>(shortestFromTies[0])){ //if tie still exists, choose path with lowest index vertex, which is the path calculated first, hence the less than
            shortestFromTies = result;
        }
    }

    return shortestFromTies;
}

int main(){

    cout << endl
        << "-----------------------------------------------------" << endl
        << "| Branching Nearest Neighbor for Traveling Salesman |" << endl
        << "-----------------------------------------------------" << endl
        << endl;

    // aribitrary size graph
    float** adjMat;
    int size = 5;
    generateMatrix(adjMat, size);

    // print generated matrix
    cout << "Adjacency Matrix:\n";
    for(int i = 0; i < size; i++){
        cout << "| ";
        for(int j = 0; j < size; j++){
            cout << adjMat[i][j] << ", ";
        }
        cout << endl << endl;
    }
    cout << endl;

    vector<tuple<int, float>> bestPath = {{-1, __FLT_MAX__}};

    //nearest neighbor starting at all vertices
    for(int i = 0; i < size; i++){

        vector<int> remaining(size); // initialize vector of remaining vertices
        std::iota (std::begin(remaining), std::end(remaining), 0); // initialize with values 0 - size
        remaining.erase(remaining.begin() + i); // remove start vertex

        // vector<tuple<int, float>> result = nearestNeighbor(adjMat, i, i, remaining);
        vector<tuple<int, float>> path =  nearestNeighbor(adjMat, i, i, remaining);

        if(std::get<1>(path[0]) < std::get<1>(bestPath[0])){
            bestPath = path;
        }
    }

    cout << "Results starting at vertex " << std::get<0>(bestPath[bestPath.size() - 1]) << ": " << endl;
    for(int j = 0; j < bestPath.size(); j++){
        cout << std::get<0>(bestPath[j]) << " " << std::get<1>(bestPath[j]) << endl;
    }
    // cout << "--------------" << endl;

    return 0;
}
