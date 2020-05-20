#include "modulatorQFSK.h"
#include "stdlib.h"
#include "math.h"
#include "define.h"
#include "bitBuffer.h"
#include "vco.h"

// Master functions
static void RelativeBaudRateSet(float);
static float RelativeBaudRateGet(void);
static void RelativeFrequencyShiftSet(float);
static float RelativeFrequencyShiftGet(void);
static complex GetSempl(void);
static void CombinationSet(uint8_t);
static void Reset(void);

// Slave functions
static uint8_t SymbolUpdate(void);
static void Init(void);

// Master struct
const struct modulatorQfskFunctions ModulatorQFSK = {
	
	{
		RelativeBaudRateSet,
		RelativeBaudRateGet
	},
	
	{
		RelativeFrequencyShiftSet,
		RelativeFrequencyShiftGet,
	},
	
	GetSempl,
	
	{
		CombinationSet
	},
	
	Reset
};

// Parameters
static float RelativeBaudRate = 0.f;
static float PhaseSymbol = 0.f;
static complex Generator = (complex){1.f, 0.f};
static complex Deviation[4] = {{1.f, 0.f}, {1.f, 0.f}, {1.f, 0.f}, {1.f, 0.f}};
static uint32_t Symbol = 0;
static uint8_t Combination[4] =	{0, 1, 2, 3};
static float RelativeFrequencyShift = 0.f;
static complex Nco = {1.f, 0.f};
static complex NcoShift = {1.f, 0.f};

// Description master functions
static void Reset(void){
	RelativeBaudRate = 0.f;
	PhaseSymbol = 0.f;
	Generator = (complex){1.f, 0.f};
	Deviation[0] = Deviation[1] = Deviation[2] = Deviation[3] = (complex){1.f, 0.f};
	Symbol = 0;
	Combination[0] = 0;
	Combination[1] = 1;
	Combination[2] = 2;
	Combination[3] = 3;
	RelativeFrequencyShift = 0.f;
	Nco = (complex){1.f, 0.f};
	NcoShift = (complex){1.f, 0.f};
}

static void RelativeBaudRateSet(float relativebaudRate){
	RelativeBaudRate = relativebaudRate;
	Init();
}
static float RelativeBaudRateGet(void){return RelativeBaudRate;}
static void RelativeFrequencyShiftSet(float relativeFrequencyShift){
	RelativeFrequencyShift = relativeFrequencyShift;
	NcoShift.re = cosf(2.f * PI * RelativeFrequencyShift);
	NcoShift.im = sinf(2.f * PI * RelativeFrequencyShift);
	Nco = (complex){1.f, 0.f};
}
static float RelativeFrequencyShiftGet(void){return RelativeFrequencyShift;}
static complex GetSempl(void){
	
	PhaseSymbol += RelativeBaudRate;
	if(PhaseSymbol >= 1.f){
		PhaseSymbol -= 1.f;
		Symbol = SymbolUpdate();
		Symbol = Combination[Symbol];		
	}
	
	Vco.Update(&Generator, &Deviation[Symbol]);
	
	Vco.Update(&Nco, &NcoShift);
	
	return Complex.Multiply(Generator, Nco);
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

// Description slave functions
static uint8_t SymbolUpdate(void){
	
	static union {
		uint8_t Symbol;
		struct {
			uint8_t b0:1;
			uint8_t b1:1;
			uint8_t unused:6;
		} Bit;
	} Converter = {0};

	Converter.Bit.b1 = BitBuffer.Bit.Get();	
	Converter.Bit.b0 = BitBuffer.Bit.Get();

	return Converter.Symbol;
	//return (uint8_t)rand() & 0x3;

}

static void Init(void){
	
	// Deviation
	float twoPiF = 2.f * PI * RelativeBaudRate;	
	Deviation[0].re = cosf(-3.f * twoPiF);
	Deviation[0].im = sinf(-3.f * twoPiF);
	Deviation[1].re = cosf(-1.f * twoPiF);
	Deviation[1].im = sinf(-1.f * twoPiF);
	Deviation[2].re = cosf(+1.f * twoPiF);
	Deviation[2].im = sinf(+1.f * twoPiF);
	Deviation[3].re = cosf(+3.f * twoPiF);
	Deviation[3].im = sinf(+3.f * twoPiF);

	// Generator
	Generator = (complex) {1.f, 0.f};
	
	// Symbol
	PhaseSymbol = 0.f;
	Symbol = 0;
}
