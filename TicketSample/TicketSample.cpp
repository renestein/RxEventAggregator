// TicketSample.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AppBoostrapper.h"


int main()
{
  AppBoostrapper bootStrapper;
  bootStrapper.Start();
  std::cin.get();
  bootStrapper.Stop();
  return 0;
}
