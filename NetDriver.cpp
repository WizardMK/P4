#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "CPUTimer.h"
#include "NetDriver.h"
#include "router.h"

using namespace std;

class Router;

using namespace std;

Checker::Checker(const char* filename, int mCount, int c, char sp): messageCount(mCount),
  connections(c), lastTime(-1), messageNum(1), lastCommand('M')
{
  int num;
  char comma;
  speed = sp == '0';
  messages = new Message[messageCount + 1];

  for(int j = 1; j <= connections; j++)
  {
    up[j] = true;
    connectionCounts[j] = 0;
  }

  ifstream inf(filename);

  while(messageNum <= messageCount)
  {
    inf >> messages[messageNum].command >> messages[messageNum].arrivalTime 
      >> comma >> num;

    if(messages[messageNum].command == 'M')
    {
      inf >> comma >> messages[messageNum].size; 
      messages[messageNum].connectionNum = -1; // not read yet
    }
    else // up or down
    {
      messages[messageNum].connectionNum = num;
      messages[messageNum].size = 0;
    }
    
    messages[messageNum].count = 0;
    messageNum++;
    inf.ignore(10, '\n');
  } // while
  
  inf.close();
  messageNum = 1;
} // Checker::Checker()


Checker::~Checker()
{
  delete [] messages;
}

void Checker::checkDownConnections()
{
  if(speed)
    return;

  for(int i = 1; i <= connections; i++)
    if(!up[i] && connectionCounts[i] > 0)
      cout << "Down connection " << i << " has " << connectionCounts[i]
        << " messages still assigned to it.\n";
} // checkDownConnections()

void Checker::finalCheck()
{
  double total = 0;
  int actualMessages = 0;
  
  if(speed)
    return;

  for(int i = 1; i <= messageCount; i++)
  {
    if(messages[i].count != messages[i].size  && messages[i].command == 'M')
      cout << "Still " << messages[i].size - messages[i].count
        << " packets in message #" << i << endl;

    if(messages[i].size > 0)
    {
      total += ((messages[i].endTime - messages[i].arrivalTime)
        / double(messages[i].size))
        * ((messages[i].endTime - messages[i].arrivalTime)
        / double(messages[i].size));
      
      actualMessages++;
    }
  }

  cout << "Final average delay: " << int (total / (1000 * actualMessages)) << endl;
} // finalCheck()


bool Checker::read(char *command, int currentTime, int *mNum, int *size)
{
  
  if(messageNum > messageCount)
    return false;

  if(currentTime < lastTime)
    cout << "Reading at time earlier than " << lastTime << endl;

  lastTime = currentTime;
  if(lastCommand == 'D')
    checkDownConnections();

  if(messages[messageNum].arrivalTime == currentTime)
  {
   // For debuggins, uncomment the next two lines to bypass connections going up and down.
   // while(messages[messageNum].command != 'M')
   //   ++messageNum;

    if(messages[messageNum].command == 'M')
    {
      lastCommand = *command = 'M';
      *mNum = messageNum;
      *size = messages[messageNum].size;
    } // if a Message command
    else // an Up or Down command
    {
      lastCommand = *command = messages[messageNum].command;
      lastConnection = *mNum = messages[messageNum].connectionNum;
      up[*mNum] = lastCommand == 'U';
    } // else an Up or Down command
    
   
    messageNum++;
  } // if unread messages at this time
  else
    *command = 'N'; // nothing left at this currentTime

  return true;
} // Checker::read()

void Checker::sent(int currentTime, int messageNum)
{
  static int lastSentTime = -1;

  if(speed)
    return;

  if(currentTime > lastSentTime)
  {
    for(int i = 0; i <= connections; i++)
      sentCounts[i] = 0;
    
    lastSentTime = currentTime;
  }  // if currentTime > lastSentTime

  if(currentTime < lastTime)
    cout << "Sent message #" << messageNum << " at earlier time than " << lastTime << endl;

  lastTime = currentTime;

  int connectionNum = messages[messageNum].connectionNum;

  if(++sentCounts[connectionNum] > 1)
    cout << "Using connection #" << connectionNum
      << " more than once during time " << currentTime << endl;

  if(++messages[messageNum].count == messages[messageNum].size)
  {
    connectionCounts[connectionNum]--;
    messages[messageNum].endTime = currentTime;
  } // if completed sending the message
  else
    if(messages[messageNum].count > messages[messageNum].size)
      cout << "Sending more packets than needed for message #" << messageNum << endl;
} // sent()


void Checker::setConnection(int messageNum, int connectionNum)
{
  if(speed)
    return;
  if(messages[messageNum].connectionNum == -1) // most common
  {
    if(lastTime != messages[messageNum].arrivalTime)
      cout << "Trying to set new connection for " << messageNum << " at time other than arrival time.\n";
    messages[messageNum].connectionNum = connectionNum;
    connectionCounts[connectionNum]++;
    return;
  } // just adding a new message

  if(lastCommand == 'M')
    cout << "Reassigning a connection when there is no change in connection status.\n";

  if(lastCommand == 'D' && up[(int) messages[messageNum].connectionNum])
    cout << "Reassigning a connection from a connection that did not go down.\n";

  if(lastCommand == 'U' && connectionNum != lastConnection)
    cout << "Reassigning a connection to a connection that did not come up.\n";

  connectionCounts[(int) messages[messageNum].connectionNum]--;
  messages[messageNum].connectionNum = connectionNum;
  connectionCounts[connectionNum]++;
}

int main(int argc, char* argv[])
{
  int messageCount, connections;
  CPUTimer ct;
  char filename[256], *ptr;
  strcpy(filename, argv[1]);
  ptr = strstr(filename,"Net");
  sscanf(ptr, "Net-%d-%d", &messageCount, &connections);
  Checker checker(argv[1], messageCount, connections, argv[2][0]);
  ct.reset();
  Router *router = new Router(&checker, connections);
  router->run();
  cout << "CPU time: " << ct.cur_CPUTime() << endl;
  checker.finalCheck();
  delete router;
  return 0;
} // main()

