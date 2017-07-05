#ifndef __MACROS_H__
#define __MACROS_H__

#include <iostream>

#define _debug_ {cout << __FILE__ << ":" << __LINE__ << endl;}
#define _safeDelete(var) { if (var) { delete var;} }
#define _safeDeleteArray(var) { if (var) { delete[] var; } }

#endif
