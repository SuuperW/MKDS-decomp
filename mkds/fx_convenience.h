#pragma once

#ifndef FX_CONVENIENCE
#define FX_CONVENIENCE

#include "nitro/fx/fx.h"

extern fx32 FX_MulFunc(fx32 v1, fx32 v2); // VS Code is being shitty with overloads
extern fx32 FX_MulFunc3(fx32 v1, fx32 v2, fx32 v3);
extern fx32 FX_MulFunc4(fx32 v1, fx32 v2, fx32 v3, fx32 v4);

extern void VEC_Mult(fx32 scalar, VecFx32* vector, VecFx32* out);

#endif
