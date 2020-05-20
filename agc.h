#ifndef AGC_H
#define AGC_H

extern const struct agcFunctions {
	void (*const Update)(float);
	void (*const Reset)(void);
	float (*const Gain)(void);
} Agc;

#endif
