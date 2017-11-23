// Author: Sean Davis
#ifndef cpuH
#define cpuH

#include "CPURunner.h"

class CPU
{
public:
  CPU();
  void operation(OpCode &opCode, Instruction &instruction, int &buffer);
};
#endif
