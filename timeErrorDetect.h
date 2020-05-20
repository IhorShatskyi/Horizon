#ifndef TIME_ERROR_DETECT_H
#define TIME_ERROR_DETECT_H

extern const struct timeErrorDetectFunctions {
	void (*const Reset)(void);
	void (*const Add)(float);
	float (*const Error)(void);
} TimeErrorDetect;

#endif
