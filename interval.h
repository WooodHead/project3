#ifndef INTERVAL_H
#define INTERVAL_H
#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
using namespace Gecode;
void interval(Home home, const IntVarArgs& x, const IntArgs& w, double p);

#endif // INTERVAL_H
