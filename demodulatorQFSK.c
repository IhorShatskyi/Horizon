#include "demodulatorQFSK.h"
#include "debugSempl.h"
#include "vco.h"
#include "math.h"
#include "storona.h"
#include "loopFilter.h"
#include "print.h"
#include "storonaDriver.h"
#include "agc.h"

typedef struct {
	float f1, f2, f3, f4;
} levelFrequency;

typedef union {
		uint8_t Symbol;
		struct {
			uint8_t b0:1;
			uint8_t b1:1;
			uint8_t unused:6;
		} Bit;
} SymbolToBits;


// Master functions
static void RelativeBaudRateSet(float);
static float RelativeBaudRateGet(void);
static void RelativeFrequencyShiftSet(float);
static float RelativeFrequencyShiftGet(void);
static void AddSempl(complex);
static void CombinationSet(uint8_t);
static void InversionSet(uint8_t);
static void Reset(void);

// Slave functions
static void InitParameters(void);
static float max(levelFrequency);
static uint8_t symbol(levelFrequency);
static float absolute(float);
static void errorDebug(levelFrequency);
static complex Invers(complex);

// Master struct
const struct demodulatorQfskFunctions DemodulatorQFSK = {
	
	{
		RelativeBaudRateSet,
		RelativeBaudRateGet
	},

	{
		RelativeFrequencyShiftSet,
		RelativeFrequencyShiftGet,
	},
	
	AddSempl,
	
	{
		CombinationSet
	},
	
	{
		InversionSet
	},
	
	Reset
};

// Parameters
static float RelativeBaudRate;
static complex VcoF1, VcoF2, VcoF3, VcoF4;
static complex ShiftF1, ShiftF2, ShiftF3, ShiftF4;
static complex IntF1, IntF2, IntF3, IntF4;
static complex DelayF1[256];
static complex DelayF2[256];
static complex DelayF3[256];
static complex DelayF4[256];
static uint32_t Index = 0u;
static uint8_t Combination[4] = {0, 1, 2, 3};
static uint8_t Inversion = 0; 
static float TimeError = 0.f;
static float Timer = 0.f;
static levelFrequency HalfLeft = {0.f, 0.f, 0.f, 0.f};
static levelFrequency HalfRight = {0.f, 0.f, 0.f, 0.f};
static float RelativeFrequencyShift = 0.f;
static complex Nco = {1.f, 0.f};
static complex NcoShift = {1.f, 0.f};

// Description master functions
static void Reset(void){
	RelativeBaudRate = 0.f;
	VcoF1 = VcoF2 = VcoF3 = VcoF4 = (complex){1.f, 0.f};	
	ShiftF1 = ShiftF2 = ShiftF3 = ShiftF4 = (complex){1.f, 0.f};
	IntF1 = IntF2 = IntF3 = IntF4 = (complex){0.f, 0.f};
	for(uint32_t i = 0; i < 256; i++)
		DelayF1[i] = DelayF2[i] = DelayF3[i] = DelayF4[i] = (complex){0.f, 0.f};
	Index = 0u;
	Combination[0] = 0;
	Combination[1] = 1;
	Combination[2] = 2;
	Combination[3] = 3;
	Inversion = 0;
	TimeError = 0.f;
	Timer = 0.f;
	HalfLeft = (levelFrequency){0.f, 0.f, 0.f, 0.f};
	HalfRight = (levelFrequency){0.f, 0.f, 0.f, 0.f};
	RelativeFrequencyShift = 0.f;
	Nco = (complex){1.f, 0.f};
	NcoShift = (complex){1.f, 0.f};
}

static void RelativeBaudRateSet(float relativeBaudRate){
	RelativeBaudRate = relativeBaudRate;
	InitParameters();
}
static float RelativeBaudRateGet(void){return RelativeBaudRate;}
static void RelativeFrequencyShiftSet(float relativeFrequencyShift){
	RelativeFrequencyShift = -relativeFrequencyShift;
	NcoShift.re = cosf(2.f * PI * RelativeFrequencyShift);
	NcoShift.im = sinf(2.f * PI * RelativeFrequencyShift);
	Nco = (complex){1.f, 0.f};
}
static float RelativeFrequencyShiftGet(void){return RelativeFrequencyShift;}
static void AddSempl(complex sempl){
	
	// Inversion check
	if(Inversion)
		sempl = Invers(sempl);	

	// Debuger
	DebugSempl.Show(sempl);
	
	// Nco shift
	Vco.Update(&Nco, &NcoShift);
	sempl = Complex.Multiply(sempl, Nco);
	
	// AGC
	sempl.re *= Agc.Gain();
	sempl.im *= Agc.Gain();

	// VCOs
	Vco.Update(&VcoF1, &ShiftF1);
	Vco.Update(&VcoF2, &ShiftF2);
	Vco.Update(&VcoF3, &ShiftF3);
	Vco.Update(&VcoF4, &ShiftF4);
	complex lineF1 = Complex.Multiply(sempl, VcoF1);
	complex lineF2 = Complex.Multiply(sempl, VcoF2);
	complex lineF3 = Complex.Multiply(sempl, VcoF3);
	complex lineF4 = Complex.Multiply(sempl, VcoF4);
	
	// for ALE
	if(RelativeBaudRate - (1000.f/12000.f) == 0.f)
		RelativeBaudRate = 100.f/12000.f;
	
	// Integrators
	Index++;
	Index &= 0xFF;
	DelayF1[Index] = lineF1;
	DelayF2[Index] = lineF2;
	DelayF3[Index] = lineF3;
	DelayF4[Index] = lineF4;
	IntF1 = Complex.Add(IntF1, lineF1);
	IntF2 = Complex.Add(IntF2, lineF2);
	IntF3 = Complex.Add(IntF3, lineF3);
	IntF4 = Complex.Add(IntF4, lineF4);
	uint32_t endIndex = Index - (uint32_t)(1.f / RelativeBaudRate);
	endIndex &= 0xFF;
	IntF1 = Complex.Sub(IntF1, DelayF1[endIndex]);
	IntF2 = Complex.Sub(IntF2, DelayF2[endIndex]);
	IntF3 = Complex.Sub(IntF3, DelayF3[endIndex]);
	IntF4 = Complex.Sub(IntF4, DelayF4[endIndex]);
	
	// Level detect
	static levelFrequency level;
	level.f1 = Complex.Abs(IntF1) * RelativeBaudRate;
	level.f2 = Complex.Abs(IntF2) * RelativeBaudRate;
	level.f3 = Complex.Abs(IntF3) * RelativeBaudRate;
	level.f4 = Complex.Abs(IntF4) * RelativeBaudRate;
	
	// Synchronization
	if(Timer < 0.75f){
		HalfLeft.f1 += level.f1;
		HalfLeft.f2 += level.f2;
		HalfLeft.f3 += level.f3;
		HalfLeft.f4 += level.f4;
	}
	
	if(Timer >= 0.25f){
		HalfRight.f1 += level.f1;
		HalfRight.f2 += level.f2;
		HalfRight.f3 += level.f3;
		HalfRight.f4 += level.f4;
	}
	
	Timer += RelativeBaudRate;
	Timer += TimeError;	
	
	// Timer
	if(Timer >= 1.f){
		Timer -= 1.f;
		
		// Time error update
		float halfLeftMax = max(HalfLeft);
		float halfRightMax = max(HalfRight);
		halfLeftMax = absolute(halfLeftMax);
		halfRightMax = absolute(halfRightMax);
		TimeError = halfRightMax - halfLeftMax;
		TimeError *= RelativeBaudRate;
		TimeError = LoopFilter.Update(TimeError);//LoopFilter.Update(sign(error));
		HalfLeft = HalfRight = (levelFrequency){0.f, 0.f, 0.f, 0.f};
	
		// Data send
		static SymbolToBits Converter;
		Converter.Symbol = symbol(level);
		Converter.Symbol = Combination[Converter.Symbol];	
		
		StoronaDriver.Bit(Converter.Bit.b1);		
		StoronaDriver.Bit(Converter.Bit.b0);
		
		
		// AGC update
		Agc.Update(max(level));
		
		// Debug		
		errorDebug(level);
	}

}
static void CombinationSet(uint8_t combination){
	
	static union {
		uint8_t Combination;
		struct {
			uint8_t F3:2;
			uint8_t F2:2;
			uint8_t F1:2;
			uint8_t F0:2;
		} Position;
	} Converter;
	
	Converter.Combination = combination;
	
	Combination[0] = Converter.Position.F0;
	Combination[1] = Converter.Position.F1;
	Combination[2] = Converter.Position.F2;
	Combination[3] = Converter.Position.F3;
	
}

static void InversionSet(uint8_t inv){
	Inversion = inv;
}

// Description slave functions
static void InitParameters(void){

	// AGC
	Agc.Reset();
	
	// VCOs
	float twoPiF = 2.f * PI * RelativeBaudRate;	
	ShiftF1.re = cosf(+3.f * twoPiF);
	ShiftF1.im = sinf(+3.f * twoPiF);
	ShiftF2.re = cosf(+1.f * twoPiF);
	ShiftF2.im = sinf(+1.f * twoPiF);
	ShiftF3.re = cosf(-1.f * twoPiF);
	ShiftF3.im = sinf(-1.f * twoPiF);
	ShiftF4.re = cosf(-3.f * twoPiF);
	ShiftF4.im = sinf(-3.f * twoPiF);
	VcoF1 = VcoF2 = VcoF3 = VcoF4 = (complex){1.f, 0.f};
	
	// Integrators
	IntF1 = IntF2 = IntF3 = IntF4 = (complex){0.f, 0.f};
	Index = 0;
	for(uint32_t i = 0; i < 256; i++)
		DelayF1[i] = DelayF2[i] = DelayF3[i] = DelayF4[i] = (complex){0.f, 0.f};
	
	// Time recovery
	TimeError = 0.f;
	Timer = 0.f;
	HalfLeft = (levelFrequency){0.f, 0.f, 0.f, 0.f};
	HalfRight = (levelFrequency){0.f, 0.f, 0.f, 0.f};
		
}

static complex Invers(complex sempl){
	return (complex){sempl.im, sempl.re};
}

static float max(levelFrequency level){
	float max = 0.f;
	if(level.f1 > max) max = level.f1;
	if(level.f2 > max) max = level.f2;
	if(level.f3 > max) max = level.f3;
	if(level.f4 > max) max = level.f4;
	return max;
}
static uint8_t symbol(levelFrequency level){
	uint8_t symbol = 0u;
	float max = 0.f;
	if(level.f1 > max) {max = level.f1; symbol = 0u;}
	if(level.f2 > max) {max = level.f2; symbol = 1u;}
	if(level.f3 > max) {max = level.f3; symbol = 2u;}
	if(level.f4 > max) {max = level.f4; symbol = 3u;}
	return symbol;
}
static float absolute(float value){
	if(value > 0.f) return value;
	else return - value;
}
static void errorDebug(levelFrequency level){
	#define LENGHT 30
	#define NUMBER 10
	#define AVERAGE 16
	static uint32_t y = 0, x = 0;
	static levelFrequency levelArray[NUMBER][LENGHT];
	static levelFrequency levelaccum[LENGHT];
	static SymbolToBits etalon, current;
	static uint32_t countError = 0;
	static uint32_t errorAccum = 0;
	static uint32_t delayAccum[AVERAGE];
	static uint32_t index = 0;

	levelaccum[x].f1 += level.f1 - levelArray[y][x].f1;			
	levelaccum[x].f2 += level.f2 - levelArray[y][x].f2;
	levelaccum[x].f3 += level.f3 - levelArray[y][x].f3;
	levelaccum[x].f4 += level.f4 - levelArray[y][x].f4;
	levelArray[y][x] = level;
	x++;
	x %= LENGHT;

	if(x == 0){
		countError = 0;		
		
		Print("Data ");
		
		for(uint32_t i = 0; i < LENGHT; i++){
			current.Symbol = symbol(levelArray[y][i]);
			etalon.Symbol = symbol(levelaccum[i]);
			if(etalon.Bit.b0 != current.Bit.b0)	countError++;
			if(etalon.Bit.b1 != current.Bit.b1)	countError++;
			Print(current.Bit.b1);			
			Print(current.Bit.b0);

		}
		
		errorAccum += countError - delayAccum[index];
		delayAccum[index] = countError;
		index++;
		index %= AVERAGE;
		
		float averageError = (float)errorAccum / (float)AVERAGE;
		averageError *= 100.f;
		averageError /= (2.f * (float)LENGHT);
		Print(" Error ");
		Print((uint32_t)averageError);
		Print(".");
		Print((uint32_t)(10.f * averageError) % 10);
		PrintLn("%");

		y++;
		y %= NUMBER;
	}

}
