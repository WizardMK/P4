#ifndef NETDRIVER_H
  #define NETDRIVER_H



typedef struct
{
  char command;
  int arrivalTime;
  int endTime;
  short size;
  short count;
  char connectionNum;
} Message;


class Checker
{
  Message *messages;
  int messageCount;
  int connections;
  int lastTime;
  int messageNum;
  char lastCommand;
  char lastConnection;
  bool up[11];
  int connectionCounts[11];
  int sentCounts[11];
  bool speed;
  void checkDownConnections();
public:
  Checker(const char* filename, int messageCt, int conns, char sp);
  ~Checker();
  bool read(char *command, int arrivalTime, int *messageNum, int *size);
  void sent(int currentTime, int messageNum);
  void setConnection(int messageNum, int connectionNum);
  void finalCheck();
};
#endif

