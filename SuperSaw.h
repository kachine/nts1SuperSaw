/*
 * File: SuperSaw.h
 *
 * NTS-1 SuperSaw Oscillator like JP8k
 *
 */
#include "userosc.h"
#include <limits.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define NUM_SAWS          7
#define BASE_SAW_ID       3
#define INIT_DETUNE_PARAM 0.5
#define INIT_MIX_PARAM    0.5

struct _voice {
  float agc;
  float detune;
  float mixBasetone;
  float mixOvertone;
  float phase[NUM_SAWS];  // Phase ratio within 0 to 1
};
struct _voice VOICE;      // Global variable to keep voice parameters

// Frequency ratio offset from basetone
static const float lutFreqRatioOffset[] = {
  -0.11002313f,
  -0.06288439f,
  -0.01952356f,
  0.f,
  0.01991221f,
  0.06216538f,
  0.10745242f};

float conv_detune(float linearDetuneValue);
float conv_mix_basetone(float linearMixValue);
float conv_mix_overtone(float linearMixValue);
float calc_agc_coeff(void);
float osc_supersaw(uint8_t notenumber);
void OSC_INIT(uint32_t platform, uint32_t api);
void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames);
void OSC_NOTEON(const user_osc_param_t * const params);
void OSC_NOTEOFF(const user_osc_param_t * const params);
void OSC_PARAM(uint16_t index, uint16_t value);
