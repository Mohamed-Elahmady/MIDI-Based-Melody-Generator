/**
 * @Author     : Mohamed El-Ahmady
 * @Date       :  20 AUG 2025
 * @Brief      : Egyption National Anthem using Buzzer & Timer 1 Preipheral
 * @References :
 * https://stackoverflow.com/questions/5367255/midi-or-wav-file-to-an-array-of-frequencies-and-duration
 */

#include "main.h"
#include <avr/io.h>
#include <math.h>
#include <stdint.h>
#include <util/delay.h>

#define SPEAKER_PIN PD5
#define MELODY_LEN 115
#define F_CPU 8000000UL

/* ---- Melody (Egypt National Anthem) ---- */
const uint8_t melody[MELODY_LEN] = {
    60, 65, 65, 60, 65, 65, 64, 65, 67, 69, 65, 69, 70, 72, 69, 67, 65,
    69, 67, 69, 65, 64, 67, 65, 60, 65, 65, 60, 65, 65, 64, 65, 67, 69,
    65, 69, 70, 72, 69, 67, 65, 69, 67, 69, 65, 64, 67, 65, 69, 69, 69,
    67, 65, 70, 69, 67, 67, 70, 70, 70, 69, 67, 72, 70, 69, 69, 69, 69,
    69, 67, 65, 70, 69, 67, 67, 69, 70, 72, 72, 72, 72, 69, 67, 65, 69,
    67, 69, 65, 64, 67, 65, 60, 65, 65, 60, 65, 65, 64, 65, 67, 69, 65,
    69, 70, 72, 69, 67, 65, 69, 67, 69, 65, 64, 67, 65};

const uint16_t durations[MELODY_LEN] = {
    162,  656, 373, 146, 658,  219, 192,  154, 1178, 329,  643,  329, 238,
    720,  462, 162, 400, 228,  351, 319,  779, 492,  127,  1035, 138, 686,
    372,  124, 656, 246, 181,  152, 1192, 348, 637,  303,  230,  734, 441,
    152,  368, 187, 368, 227,  640, 440,  131, 766,  305,  103,  728, 275,
    225,  755, 472, 73,  2025, 419, 77,   771, 286,  244,  758,  559, 85,
    2089, 244, 106, 682, 351,  271, 725,  562, 77,   1138, 214,  124, 666,
    316,  92,  729, 437, 135,  360, 297,  352, 313,  771,  403,  122, 1052,
    151,  675, 368, 166, 624,  209, 187,  186, 1173, 311,  620,  325, 287,
    775,  448, 133, 410, 232,  329, 224,  732, 392,  100};

/* ---- MIDI ? Frequency ---- */
uint16_t midiToFreq(uint8_t midi) {
  return (uint16_t)(440.0 * pow(2.0, ((float32_t)midi - 69.0) / 12.0));
}

/* ---- Timer1 Init (CTC Mode) ---- */
void timer1_init() {
  DDRD |= (1 << SPEAKER_PIN);          // PD5 (OC1A) as output
  TCCR1A = (1 << COM1A0);              // Toggle OC1A on compare match
  TCCR1B = (1 << WGM12) | (1 << CS10); // CTC mode, no prescaler
}

/* ---- Set Frequency ---- */
void set_frequency(uint16_t freq) {
  if (freq == 0) {
    TCCR1A &= ~(1 << COM1A0); // Stop toggle (silence)
  } else {
    TCCR1A |= (1 << COM1A0); // Enable toggle
    uint32_t top = (F_CPU / (2UL * freq)) - 1;
    if (top > 65535)
      top = 65535;
    OCR1A = (uint16_t)top;
  }
}

void delay_ms_variable(uint16_t ms) {
  while (ms--) {
    _delay_ms(1);
  }
}

/* ---- Main ---- */
int main(void) {
  timer1_init();
  while (1) {
    for (uint8_t i = 0; i < MELODY_LEN; i++) {
      uint16_t f = midiToFreq(melody[i]);
      set_frequency(f);
      delay_ms_variable(durations[i]); // play note
      set_frequency(0);                // short break
      _delay_ms(30);
    }
    _delay_ms(2000); // pause before replay
  }
}