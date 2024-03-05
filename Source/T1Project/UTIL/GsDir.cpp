
#include "GsDir.h"

FGsDir* FGsDir::Instance = nullptr;

float FGsDir::_sinIndex[FULL_SHORT];
float FGsDir::_cosIndex[FULL_SHORT];