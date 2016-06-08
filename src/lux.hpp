#ifndef __lux__
#define __lux__

#include "lxalloc.hpp"
#include "lxstore.hpp"
#include "lxstack.hpp"
#include "lxtuple.hpp"
#include "lxthunk.hpp"
#include "lxchars.hpp"
#include "lxarray.hpp"
#include "lxclass.hpp"
#include "lxbuffs.hpp"
#include "lxdebug.hpp"
#include "lxerror.hpp"
#include "lxtools.hpp"

// We want to keep these names identical across modules

template <> const char *lux_Type<int*>::name = "int";
template <> const char *lux_Type<char*>::name = "char";
template <> const char *lux_Type<short*>::name = "short";
template <> const char *lux_Type<long*>::name = "long";
template <> const char *lux_Type<float*>::name = "float";
template <> const char *lux_Type<double*>::name = "double";

#endif // file
