#ifndef COMMAND_H
#define COMMAND_H

#include "client.h"

typedef struct
{
  char                  *orders;
  void                  (*func)();
}                       Command;

#endif