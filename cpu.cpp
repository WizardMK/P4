// Author: Sean Davis
#include "cpu.h"

CPU::CPU()
{
} // CPU()


void CPU::operation(OpCode &opCode, Instruction &instruction, int &buffer)
{
  switch (opCode)
  {
    case ADD  : // *addr3 = *addr1 + *addr2
    case LOAD :  // buffer contains int requested from RAM
    case STORE:  // Sent by RAM after a STORE
    case DONE : //  All ADDs in file have been sent.  Time to STORE dirty cache.
  } // switch
} // operation()



