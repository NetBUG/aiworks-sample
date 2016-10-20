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
int M = 0;	//+ router count
int N = 0;	//+ room count

struct connections {
  int x = -1;    //+ first possible neighbor
  int y = -1;
  int z = -1;
  int sat = -1;  //+ satisfaction
} ;

vector<int> rooms;	//+ satisfaction for each room
vector<connections> coverages;	//+ satisfaction for room + neighbors

int get_neighbor_count(connections conn) {
  if (conn.z > 0) return 3;
  if (conn.y > 0) return 2;
  if (conn.x > 0) return 1;
  return 0;
}

void append_neighbor(connections& conn, int room) {
  int nc = get_neighbor_count(conn);
  assert(nc < 3);	//+ room connections are full
  if (nc > 1) conn.z = room;
  else if (nc > 0) conn.y = room;
  else conn.x = room;
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
    for (vector<int>::iterator i = rooms.begin(); i < rooms.end(); ++i)
        fh >> *i;
    if (bDebug)
      for (int i = 0; i < rooms.size(); i++)
        printf("%d ", rooms[i]);

    int src, dst;	//~ source and destination room
    for (int i = 1; i < N; i++)
      {
        fh >> src;
        fh >> dst;
        if (bDebug) printf("Conn %d: %d -> %d\n", i, src, dst);
        append_neighbor(coverages[src], dst);
        append_neighbor(coverages[dst], src);
      }
    return 0;
}	//+ load_data

void calculate_satisfaction() 
{
    int max_sat = -1;

    //+ Calculating
    

    printf("Satisfaction: %d\n", max_sat);
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

    calculate_satisfaction();
#ifdef DEBUG
  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  printf ("\n  Elapsed time: %.6f seconds", elapsed_secs);
#endif
}	//+ main()
