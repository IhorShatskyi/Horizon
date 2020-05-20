#ifndef DEBUG_SEMPL_H
#define DEBUG_SEMPL_H

#include "iq.h"
#include "list.h"

extern list Sempls;

extern const struct debugSemplFunctions {

	void (* Show)(complex);

} DebugSempl;

#endif
