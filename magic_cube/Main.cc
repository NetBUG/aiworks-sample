#include <algorithm>
#include <cassert>
#include <fstream>
#include <iterator>
#include <math.h>
#include <numeric>
#include <iostream>
#include <vector>

using namespace std;

#ifndef DEBUG
bool bDebug = true;
#include <ctime>
#define DEBUG
#else
bool bDebug = true;
#endif

//+ Global variables is not the best way to handle data in real applications, 
//+ but there were no limitations in current task
int M = 0;	//+ big cube size
int N = 0;	//+ number of small cubes
int P = 0;	//+ max value in a cell
vector<int> big_cube;	//+ Intentionally uninitialized to avoid double allocation
vector<vector<int> > cubes;

struct coord {
  int x = -1;
  int y = -1;
  int z = -1;
} ;

int load_data(string filename = "sample.in") 
{
    fstream fh(filename, ios_base::in);

    fh >> M;
    fh >> N;
    fh >> P;
    assert(M > 1 && M < 8);	//~ Wrong bug cube size
    assert(N > 0 && N < 13);	//~ Wrong cube count
    assert(P > 2 && P < 6);	//~ Wrong max cell value

    big_cube.resize(pow(M, 3));
    for (vector<int>::iterator i = big_cube.begin(); i < big_cube.end(); ++i)
        fh >> *i;
    if (bDebug)
      for (int i = 0; i < big_cube.size(); i++)
        printf("%d ", big_cube[i]);

    cubes.resize(N);
    int cs;		//+ cube size
    for (int i = 0; i < N; i++)
      {
        fh >> cs;
        vector<int> cube(pow(cs, 3));
        for (vector<int>::iterator it = cube.begin(); it < cube.end(); ++it)
            fh >> *it;
        cubes.push_back(cube);
      }

    return 0;
}	//+ load_data

bool check_validity(vector<int> allocX, vector<int> allocY, vector<int> allocZ) {
  vector <int> new_cube(big_cube);
  for (int i = 0; i < cubes.size(); i++)  //~ applying small cubes
  {
    int cu_pos_X = distance(allocX.begin(), find(allocX.begin(),allocX.end(), i));
    int cu_pos_Y = distance(allocY.begin(), find(allocY.begin(),allocY.end(), i));
    int cu_pos_Z = distance(allocZ.begin(), find(allocZ.begin(),allocZ.end(), i));
    
    if (cu_pos_X > cu_pos_Y || cu_pos_Y > cu_pos_Z) return false; //~ TODO: break on wrong cube pos
    
    for (int x = cu_pos_X; i < cu_pos_X + cubes[i].size(); x++)
      for (int y = cu_pos_Y; i < cu_pos_Y + cubes[i].size(); y++)
        for (int z = cu_pos_Z; i < cu_pos_Z + cubes[i].size(); z++) {
          new_cube[x] = (new_cube[x] + cubes[i][0]) % P;
          new_cube[y + M] = (cubes[i][1] + new_cube[y + M]) % P;
          new_cube[z + 2 * M] += (cubes[i][2] + new_cube[z + 2 * M]) % P;
        }
  }
  return accumulate(new_cube.begin(), new_cube.end(), 0) == 0;
}

void calculate_cubes() 
{
    vector<coord> res(N);

    //+ Calculating
    int sum_cube_size = 0;
    cout << "Iterating... ";
    for (vector<vector<int> >::iterator it = cubes.begin(); it < cubes.end(); ++it)
      sum_cube_size += it->size();
    cout << "Total variation: " << sum_cube_size << ", allocating " << cubes.size();
    vector<int> allocX(M - sum_cube_size, 0);
    vector<int> allocY(M - sum_cube_size, 0);
    vector<int> allocZ(M - sum_cube_size, 0);
    for (int i = 0; i < cubes.size(); i++)
      allocX[i] = i + 1, allocY[i] = i + 1, allocZ[i] = i + 1;
    do {
      do {
        do {
        if (check_validity(allocX, allocY, allocZ))   //+ if 
        {
          coord position;
          position.x = 0;
          position.y = 1;
          position.z = 2;
          res.push_back(position);
        }
      } while(std::next_permutation(allocZ.begin(), allocZ.end()));
    } while(std::next_permutation(allocY.begin(), allocY.end()));
  } while(std::next_permutation(allocX.begin(), allocX.end()));

    for (vector<coord>::iterator i = res.begin(); i < res.end(); ++i)
        printf("[%d %d %d]\n", i -> x, i -> y, i -> x);
}	//+ calculate_cubes

int main(int argc, char *argv[])
{
#ifdef DEBUG
    clock_t begin = clock();
#endif
    cout << "Hello, world!\n";
    try {
        load_data();
    } catch (const exception& e) {
        cout << "Error reading source file!" << endl;
    }

    calculate_cubes();
#ifdef DEBUG
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  printf ("\n  Elapsed time: %.6f seconds", elapsed_secs);
#endif
}	//+ main()
