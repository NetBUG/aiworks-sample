#include <algorithm>
#include <cassert>
#include <fstream>
#include <iterator>
#include <math.h>
#include <numeric>
#include <iostream>
#include <vector>

//+ @author Oleg Urzhumtcev
//+ @brief Solution for Magic Cube application for WorksAP competition

using namespace std;

#ifndef DEBUG
bool bDebug = false;
#else
#include <ctime>
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
    for (int i = 0; i < big_cube.size(); i++)
        fh >> big_cube[i];
    if (bDebug)
      for (int i = 0; i < big_cube.size(); i++)
        printf("%d ", big_cube[i]);

    int cs;		//+ cube size
    for (int j = 0; j < N; j++)
      {
        fh >> cs;
        vector<int> cube(pow(cs, 3));
        for (vector<int>::iterator it = cube.begin(); it < cube.end(); ++it)
            fh >> *it;
        cubes.push_back(cube);
      }
    return 0;
}	//+ load_data

bool find_broken_seq(vector<int> seq) {
  int cur_val = 0;
  for (vector<int>::iterator it = seq.begin(); it < seq.end(); ++it)
    if (*it > 0)
    {
      if (*it > cur_val)
        cur_val = *it;
      else
        return true;
    }
  return false;
}

coord check_validity(vector<int> allocX, vector<int> allocY, vector<int> allocZ)
{
  vector <int> new_cube(big_cube);
  coord position;
  if (find_broken_seq(allocX) || find_broken_seq(allocX) || find_broken_seq(allocZ)) 
    return position;  //+ "Thrown away on wrong cube relative position"

  for (int i = 0; i < cubes.size(); i++)  //~ applying small cubes
  {
    position.x = distance(allocX.begin(), find(allocX.begin(),allocX.end(), i));
    position.y = distance(allocY.begin(), find(allocY.begin(),allocY.end(), i));
    position.z = distance(allocZ.begin(), find(allocZ.begin(),allocZ.end(), i));
    
    for (int x = position.x; x < position.x + cubes[i].size(); x++)
      for (int y = position.y; y < position.y + cubes[i].size(); y++)
        for (int z = position.z; z < position.z + cubes[i].size(); z++) {
          new_cube[x + y*M + z*pow(M, 2)] = (new_cube[x + y*M + z*pow(M, 2)] 
            + cubes[i][0]) % P;
          continue;
        }
  }
  return accumulate(new_cube.begin(), new_cube.end(), 0) == 0 ? position : *(new coord());
}

void calculate_cubes() 
{
    vector<coord> res(0);

    //+ Calculating
    int sum_cube_size = 0;
    for (vector<vector<int> >::iterator it = cubes.begin(); it < cubes.end(); ++it)
      sum_cube_size += it->size();
    vector<int> allocX(M - sum_cube_size + 1, 0);
    vector<int> allocY(M - sum_cube_size + 1, 0);
    vector<int> allocZ(M - sum_cube_size + 1, 0);
    for (int i = 1; i <= cubes.size(); i++)
      allocX[i] = i, allocY[i] = i, allocZ[i] = i;
    do {
      do {
        do {
          coord position = check_validity(allocX, allocY, allocZ);
          if (position.x > -1)   //+ if 
            res.push_back(position);
          } while(std::next_permutation(allocZ.begin(), allocZ.end()));
      } while(std::next_permutation(allocY.begin(), allocY.end()));
    } while(std::next_permutation(allocX.begin(), allocX.end()));
    if (bDebug) {
      cout << endl << res.size() << " combinations: " << endl;
      for (vector<coord>::iterator i = res.begin(); i < res.end(); ++i)
        printf("[%d %d %d]\n", i -> x, i -> y, i -> x);
    } else {
      for (vector<coord>::iterator i = res.begin(); i < res.end(); ++i)
        printf("%d %d %d", i -> x, i -> y, i -> x);
    }
}	//+ calculate_cubes

int main(int argc, char *argv[])
{
#ifdef DEBUG
    clock_t begin = clock();
#endif
    try {
        load_data();
    } catch (const exception& e) {
        cout << "Error reading source file!" << endl;
        return -1;
    }

    if (bDebug) for (int i = 0; i < cubes.size(); i++) {
      cout << endl << cubes.size() << " cubes" << endl;
      for (int j = 0; j < cubes[i].size(); j++)
        cout << cubes[i][j];
      cout << endl;
    }

    calculate_cubes();
#ifdef DEBUG
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  printf ("\n  Elapsed time: %.6f seconds", elapsed_secs);
#endif
}	//+ main()
