/*
 * File: SuperSaw.c
 *
 * NTS-1 SuperSaw Oscillator like JP8k
 *
 */
#include <SuperSaw.h>

// Detune param to actual ratio converter
float conv_detune(float linearDetuneValue){
  return (10028.7312891634f * powf(linearDetuneValue, 11)) - (50818.8652045924f * powf(linearDetuneValue, 10)) + (111363.4808729368f * powf(linearDetuneValue, 9)) -
         (138150.6761080548f * powf(linearDetuneValue, 8)) + (106649.6679158292f * powf(linearDetuneValue, 7)) - (53046.9642751875f * powf(linearDetuneValue, 6)) +
         (17019.9518580080f * powf(linearDetuneValue, 5))  - (3425.0836591318f * powf(linearDetuneValue, 4))   + (404.2703938388f * powf(linearDetuneValue, 3)) -
         (24.1878824391f * powf(linearDetuneValue, 2))     + (0.6717417634f * linearDetuneValue)               + 0.0030115596f;
}

// Mix param to actual amplitude value converter for base frequency tone
float conv_mix_basetone(float linearMixValue){
  return -0.55366f * linearMixValue + 0.99785f;
}

// Mix param to actual amplitude value converter for overtones
float conv_mix_overtone(float linearMixValue){
  return -0.73764f * powf(linearMixValue, 2) + 1.2841f * linearMixValue + 0.044372f;
}

// Automatic gain control coefficient calculator
float calc_agc_coeff(void){
  return 1.f / (VOICE.mixBasetone + VOICE.mixOvertone * (NUM_SAWS-1));
}

// SuperSaw oscillator
float osc_supersaw(uint8_t notenumber, uint8_t pitchmodifier){
  float sample = 0.f;

  for(uint8_t sawId = 0; sawId < NUM_SAWS; sawId++){
    // Wave generation
    sample += osc_sawf(VOICE.phase[sawId]) * ( sawId == BASE_SAW_ID ? VOICE.mixBasetone : VOICE.mixOvertone);
    
    // Step a phase ratio
    VOICE.phase[sawId] += osc_w0f_for_note(notenumber, pitchmodifier) * (1.f + lutFreqRatioOffset[sawId] * VOICE.detune);
    // Keep the phase ratio within 0 <= phase < 1
    VOICE.phase[sawId] -= (uint32_t) VOICE.phase[sawId];
  }

  return sample * VOICE.agc;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

void OSC_INIT(uint32_t platform, uint32_t api)
{
  // Initialize
  VOICE.detune = conv_detune(INIT_DETUNE_PARAM);
  VOICE.mixBasetone = conv_mix_basetone(INIT_MIX_PARAM);
  VOICE.mixOvertone = conv_mix_overtone(INIT_MIX_PARAM);

  for(uint8_t sawId = 0; sawId < NUM_SAWS; sawId++){
    VOICE.phase[sawId] = _osc_rand() / UINT_MAX;  // Randomize phase ratio within 0 to 1
  }
}

void OSC_CYCLE(const user_osc_param_t * const params,
               int32_t *yn,
               const uint32_t frames)
{
  q31_t * __restrict y = (q31_t *)yn;
  const q31_t * y_e = y + frames;
  float sample = 0.f;

  // MIDI note# and pitch modifier of current process
  // If pitch bend message has already received, note value may be differ from actual MIDI note#
  // Pitch modifier value takes within 0 to 255, the value indicate 1/255 of semitone
  // The pitch modifier is always upperward, so downer pitch bend is processed as a combination of note# decrement and adequate upperward pitch modifier.
  uint8_t note = params->pitch >> 8;
  uint8_t mod = params->pitch & 0xFF;

  while( y != y_e ) {
    // Generate wave
    sample = osc_supersaw(note, mod);
    *(y++) = f32_to_q31(sample);
  }
}

void OSC_NOTEON(const user_osc_param_t * const params)
{
  // Nothing to do
}

void OSC_NOTEOFF(const user_osc_param_t * const params)
{
  // Nothing to do
}

void OSC_PARAM(uint16_t index, uint16_t value)
{
  switch (index) {
    case k_user_osc_param_shape: // Detune amount
      // 10bit parameter, Mapped to OSC mode A knob and MIDI CC#54
      VOICE.detune = conv_detune(param_val_to_f32(value));
      break;
    case k_user_osc_param_shiftshape: // Mix amount
      // 10bit parameter, Mapped to OSC mode B knob and MIDI CC#55
      VOICE.mixBasetone = conv_mix_basetone(param_val_to_f32(value));
      VOICE.mixOvertone = conv_mix_overtone(param_val_to_f32(value));
      VOICE.agc = calc_agc_coeff();
      break;
    default:
      break;
  }
}

#pragma GCC diagnostic pop
