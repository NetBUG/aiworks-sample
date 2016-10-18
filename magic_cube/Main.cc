#include <cassert>
#include <fstream>
#include <math.h>
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
vector<vector<int>> cubes;

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
    for (vector<vector<int>>::iterator cube = cubes.begin(); 
      cube < cubes.end(); ++cube)
      {
        fh >> cs;
        cube -> resize(cs);
        if (bDebug)
          cout << endl << cs << "; ";
        for (vector<int>::iterator i = cube -> begin(); i < cube -> end(); ++i)
            fh >> *i;
      }

    return 0;
}	//+ load_data

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
    
#ifdef DEBUG
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  printf ("Elapsed time: %.6f seconds", elapsed_secs);
#endif
}	//+ main()
