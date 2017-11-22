#ifndef routerH
#define routerH

#include "NetDriver.h"
class Checker;

class Router
{
  Checker *checker;
  int connections;
  int currentTime;
  void addMessage(int messageNum, int size);
  void bringConnectionDown(int connectionNum);
  void bringConnectionUp(int connectionNum);
  void sendNow();
public:
  Router(Checker *check, int conn);
  ~Router();
  void run();
}; // class Router


#endif
