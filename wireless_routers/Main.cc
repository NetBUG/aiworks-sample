#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <math.h>
#include <numeric>
#include <vector>

//+ @author Oleg Urzhumtcev
//+ @brief Solution for Wireless routers task for Works AP

using namespace std;

#ifdef DEBUG
bool bDebug = true;
#include <ctime>
#define DEBUG
#else
bool bDebug = false;
#endif

//+ Global variables is not the best way to handle data in real applications, 
//+ but there were no limitations in current task
int M = 0;	//+ router count
int N = 0;	//+ room count

struct connections {
  vector<int> neighbors;
} ;

vector<int> rooms;	//+ satisfaction for each room
vector<connections> coverages;	//+ satisfaction for room + neighbors

void append_neighbor(connections& conn, int room) {
  if (find(conn.neighbors.begin(), conn.neighbors.end(), room) == conn.neighbors.end())
    conn.neighbors[find(conn.neighbors.begin(), conn.neighbors.end(), -1) 
      - conn.neighbors.begin()] = room;
}

int load_data(string filename = "sample.in") 
{
    fstream fh(filename, ios_base::in);

    fh >> N;
    fh >> M;
    assert(M > 0 && M < 8);	//~ Wrong routers specified
    assert(N > 1 && N < 1001);	//~ Wrong room count specified

    rooms.resize(N);
    coverages.resize(N);
    int j = 0;
    for (vector<int>::iterator i = rooms.begin(); i < rooms.end(); ++i)
    {
        fh >> *i;
        coverages[j++].neighbors.resize(3, -1);
    }
    if (bDebug)
      for (int i = 0; i < rooms.size(); i++)
        printf("%d ", rooms[i]);

    int src, dst;	//~ source and destination room
    for (int i = 1; i < N; i++)
      {
        fh >> src;
        fh >> dst;
        if (bDebug) printf("\nConn %d: %d -> %d", i, src - 1, dst - 1);
        append_neighbor(coverages[src - 1], dst - 1);
        append_neighbor(coverages[dst - 1], src - 1);
      }
    return 0;
}	//+ load_data

int calculate_satisfaction(vector<int> orig_perm)
{
    int ret = 0;
    vector<int> permutation(orig_perm);
    for (int i = 0; i < permutation.size(); i++)
    {
        vector<int> neighbors = coverages[i].neighbors;
        if (orig_perm[i] == 1)
          for (int j = 0; j < neighbors.size(); j++)
            if (neighbors[j] > -1)
              permutation[neighbors[j]] = 1;
    }

    for (int i = 0; i < permutation.size(); i++) {
        ret += permutation[i] > 0 ? rooms[i] : 0;
        if (bDebug) cout << permutation[i];
    }
    return ret;
}	//+ calculate_satisfaction

int get_max_satisfaction() 
{
    if (M == N)	//~ shorthand, although does not save much time for single permutation
        return accumulate(rooms.begin(), rooms.end(), 0);

    int max_sat = -1;
    //+ Calculating
    vector<int> allocations(N, 0);
    for (int i = N - M; i < N; i++)
      allocations[i] = 1;
    do {
        max_sat = max(max_sat, calculate_satisfaction(allocations));
    } while(std::next_permutation(allocations.begin(), allocations.end()));
    return max_sat;
}	//+ calculate_max_satisfaction

int main(int argc, char *argv[])
{
#ifdef DEBUG
    clock_t begin = clock();
#endif
    try {
        load_data();
    } catch (const exception& e) {
        cout << "Error reading source file!" << endl;
    }

    int max_sat = get_max_satisfaction();
    if (bDebug) printf("\nSatisfaction: %d\n", max_sat);
    else cout << max_sat;
#ifdef DEBUG
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  printf ("\n  Elapsed time: %.6f seconds", elapsed_secs);
#endif
}	//+ main()
