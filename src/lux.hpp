#ifndef __lux__
#define __lux__

#include "lxalloc.hpp"
#include "lxstore.hpp"
#include "lxstack.hpp"
#include "lxthunk.hpp"
#include "lxchars.hpp"
#include "lxarray.hpp"
#include "lxclass.hpp"
#include "lxbuffs.hpp"
#include "lxdebug.hpp"
#include "lxerror.hpp"
#include "lxtools.hpp"

// We want to keep these names identical across modules

template <> const char *lux_Name<int*>::name = "int";
template <> const char *lux_Name<char*>::name = "char";
template <> const char *lux_Name<short*>::name = "short";
template <> const char *lux_Name<long*>::name = "long";
template <> const char *lux_Name<float*>::name = "float";
template <> const char *lux_Name<double*>::name = "double";

#endif // file
