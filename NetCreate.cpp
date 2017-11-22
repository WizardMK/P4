#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>

using namespace std;

void createFile(const char *filename, int messages, int connections)
{
  ofstream outf(filename);
  int totalBytes = 0, time = 1, count = 0, size, downCount = 0;
  bool up[11];
  double load;

  for(int i = 0; i <= connections; i++)
    up[i] = true;

  while(count < messages)
  {
    size = 0x400 / ((rand() & 0xFF) + 1);  // range 4 - 1024;
    outf << 'M' << time << ',' << ++count << ',' << size << endl;
    totalBytes += size;
    load = totalBytes - connections * time;
    if(load / connections > 20000) // too many waiting so extend time accordingly
      time += (int) (load / connections - 20000);

    if((rand() & 0xFF) == 0 && downCount < connections - 1 && messages - count > 20) // Every 256
    {
      int goner = rand() % connections + 1;
      while(!up[goner])
        goner = rand() % connections + 1;

      outf << 'D' << time << ',' << goner << endl;
      up[goner] = false;
      downCount++;
      count++;
    }

    if(downCount > 0 && (messages - count < 20 || (rand() & 0xFF) == 0)) // Every 256
    {
      int resurrect;

      for(resurrect = 1; up[resurrect]; resurrect++);

      outf << 'U' << time << ',' << resurrect << endl;
      up[resurrect] = true;
      downCount--;
      count++;
    }
  }

  outf.close();
} // createFile()

int main()
{
  int seed, messages, connections;
  char filename[80];
  cout << "Messages >> ";
  cin >> messages;
  cout << "connections >> ";
  cin >> connections;
  cout << "Seed >> ";
  cin >> seed;
  srand(seed);
  sprintf(filename, "Net-%d-%d-%d.csv", messages, connections, seed);
  createFile(filename, messages, connections);

  return 0;
}

