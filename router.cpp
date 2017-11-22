#include <iostream>
#include "router.h"
#include "NetDriver.h"

using namespace std;

Router::Router(Checker *check, int conns) : checker(check), connections(conns)
{
} // Router


Router::~Router()
{
  // Students should delete anything created with "new" here.
} // ~Router()


void Router::addMessage(int messageNum, int size)
{
  // tells checker which connection you initially choose for this message.
  // currentTime must match that when message was read().

  // checker->setConnection(messageNum, connection);
} // addMessage()


void Router::bringConnectionDown(int connectionNum)
{
  // reassigns messages currently assigned to down connection
  // currentTime must match that when connection is brought down.
  // Repeatedly calls setConnection() until down connection queue is empty

  // checker->setConnection(messageNum, newConnectionNum);
} // bringConnectionUp()


void Router::bringConnectionUp(int connectionNum)
{
  // reassigns messages to newly resurrected connection using
  // currentTime must match that when when connection is brought up.
  // Repeatedly calls setConnection to fill new connection.

  //checker->setConnection(messageNum, connectionNum);
} // bringConnectionUp()




void Router::run()
{
  char command;
  int num, size;
  currentTime = 1;

  // starting point for student code
  while(checker->read(&command, currentTime, &num, &size))
  {
    if(command == 'M') // num is the message number
      addMessage(num, size);
    else
      if(command == 'N')
        // done reading for this time so send a message on each up connection
        sendNow();
      else
        if(command == 'U') // num is the number of connection affected
          bringConnectionUp(num);
        else // command must be 'D'
          bringConnectionDown(num);

  }  // while more to read()

 // while(1) // while still messages unsent in queues
    sendNow();
} // run()


void Router::sendNow()
{
  // for each connection that is up call
  // checker->sent(currentTime, messageNum)
  // after all have been sent increment the currentTime.
  currentTime++;
} // sendNow()

