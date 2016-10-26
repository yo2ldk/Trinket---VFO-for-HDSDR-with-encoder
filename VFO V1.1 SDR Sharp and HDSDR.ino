#include "TrinketHidCombo.h"
 
#define VFO_UP 0
#define VFO_DOWN 2
#define TRINKET_PINx  PINB
#define MODE 5
static uint8_t enc_prev_pos   = 0;
static uint8_t enc_flags      = 0;
static char    sw_was_pressed = 0;
 
void setup()
{
  pinMode(VFO_UP, INPUT);
  pinMode(VFO_DOWN, INPUT);
  digitalWrite(VFO_UP, LOW);
  digitalWrite(VFO_DOWN, LOW);
 
  pinMode(MODE, INPUT);
  digitalWrite(MODE, HIGH);
 
  TrinketHidCombo.begin(); 
  if (digitalRead(VFO_UP) == HIGH) {
    enc_prev_pos |= (1 << 0);
  }
  if (digitalRead(VFO_DOWN) == HIGH) {
    enc_prev_pos |= (1 << 1);
  }
}
 
void loop()
{
  int8_t enc_action = 0; 
  uint8_t enc_cur_pos = 0;
  // read in the encoder state first
  if (bit_is_clear(TRINKET_PINx, VFO_UP)) {
    enc_cur_pos |= (1 << 0);
  }
  if (bit_is_clear(TRINKET_PINx, VFO_DOWN)) {
    enc_cur_pos |= (1 << 1);
  }
  if (enc_cur_pos != enc_prev_pos)
  {
    if (enc_prev_pos == 0x00)
    {
      // this is the first edge
      if (enc_cur_pos == 0x01) {
        enc_flags |= (1 << 0);
      }
      else if (enc_cur_pos == 0x02) {
        enc_flags |= (1 << 1);
      }
    }
 
    if (enc_cur_pos == 0x03)
    {
        enc_flags |= (1 << 4);
    }
    else if (enc_cur_pos == 0x00)
    {
      // this is the final edge
      if (enc_prev_pos == 0x02) {
        enc_flags |= (1 << 2);
      }
      else if (enc_prev_pos == 0x01) {
        enc_flags |= (1 << 3);
      }
      if (bit_is_set(enc_flags, 0) && (bit_is_set(enc_flags, 2) || bit_is_set(enc_flags, 4))) {
        enc_action = 1;
      }
      else if (bit_is_set(enc_flags, 2) && (bit_is_set(enc_flags, 0) || bit_is_set(enc_flags, 4))) {
        enc_action = 1;
      }
      else if (bit_is_set(enc_flags, 1) && (bit_is_set(enc_flags, 3) || bit_is_set(enc_flags, 4))) {
        enc_action = -1;
      }
      else if (bit_is_set(enc_flags, 3) && (bit_is_set(enc_flags, 1) || bit_is_set(enc_flags, 4))) {
        enc_action = -1;
      }
 
      enc_flags = 0; 
    }
  }
 
  enc_prev_pos = enc_cur_pos;
  if (enc_action > 0) {
    TrinketHidCombo.pressKey(0x01,0xXX);
    TrinketHidCombo.pressKey(0,0);
    delay(10);
  }
  else if (enc_action < 0) {
    TrinketHidCombo.pressKey(0x01,0xXX);
    TrinketHidCombo.pressKey(0,0);
    delay(10);
  }
  if (bit_is_set(TRINKET_PINx, MODE)) 
  {
    if (sw_was_pressed == 0)
    {
      TrinketHidCombo.pressKey(0x01,0xXX);
      TrinketHidCombo.pressKey(0,0);
     delay(10);
     }
    sw_was_pressed = 1;
  }
  else
  {
    if (sw_was_pressed != 0) {
      delay(5); 
    }
    sw_was_pressed = 0;
  }
 
  TrinketHidCombo.poll(); 
}
