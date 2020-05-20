#ifndef LOOP_FILTER_H
#define LOOP_FILTER_H

extern const struct loopFilterFunctions {
	float (*const Update)(float);
} LoopFilter;

#endif
