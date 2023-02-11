// SCRATCH


// void log_states(uint8_t states[], int numStates) {
//   Serial.println("STATES");
//   Serial.println(numStates);
//   for (int i=0;i<numStates;i++) {
//     if (states[i] > 0) {
//       Serial.print(states[i]);
//     } else {
//       Serial.print(".");
//     }
//     if ((i%10) == 9) {
//       Serial.print(" "); 
//     }
//   }
//   Serial.println("");
// }

// void log_int_states(int states[], int numStates) {
//   Serial.println("STATES");
//   Serial.println(numStates);
//   for (int i=0;i<numStates;i++) {
//     if (states[i] > 0) {
//       Serial.print(states[i]);
//     } else {
//       Serial.print(".");
//     }
//     if ((i%10) == 9) {
//       Serial.print(" "); 
//     }
//   }
//   Serial.println("");
// }

// void log_hsv_states(CHSV states[], int numStates) {
//   Serial.print("STATES (HSV) - "); Serial.println(numStates);
//   for (int i=0;i<numStates;i++) {
//     if (states[i].v > 0) {
//       Serial.print(states[i].h); Serial.print("|"); Serial.print(states[i].s); Serial.print("|"); Serial.print(states[i].v); Serial.print(" ");
//     } else {
//       Serial.print(".");
//     }
//     if ((i%10) == 9) {
//       Serial.print(" "); 
//     }
//   }
//   Serial.println("");
// }

// void log_idx_start_end(int idx, int start, int end_) {
//   Serial.print("clear idx "); Serial.print(idx); Serial.print(" - "); Serial.print(start); Serial.print(" - "); Serial.print(end_); Serial.println();    
// }

// void log_idx_rgb(int i, CRGB color) {
//   Serial.print(i); Serial.print(" - "); Serial.print(color.r); Serial.print(","); Serial.print(color.g); Serial.print(","); Serial.println(color.b);
// }

// void log_idx_hsv(int i, CHSV color) {
//   Serial.print(i); Serial.print(" - "); Serial.print(color.h); Serial.print(","); Serial.print(color.s); Serial.print(","); Serial.println(color.v);
// }


// CLEAR FUNCTIONS
// void clear_leds(I2SClocklessLedDriver driver, CRGB color = CRGB::Black) {
//   for(int i=0;i<max(NUM_LEDS_PER_STRIP, TOTAL_LEDS);i++) {
//     driver.setPixel(i,color.r,color.g,color.b, 0);
//   }
// }

// void clear_hues(CHSV currentHues[], int num_hues, uint8_t hue=0, uint8_t sat=255, uint8_t valu=255) {
//   for (int i=0;i<num_hues;i++) {
//     currentHues[i] = CHSV(hue, sat, valu);
//   }
// }


// LOAD PATH FUNCTION

// void load_path_states(uint8_t states_indiv[], int states_indiv_path[], int8_t path_follower[], int num_leds, uint8_t path_index) {
//   for(int i=0;i<LIGHTABLE_LEDS;i++) {
//     int sq_ = floor(i/SQ_SIZE);                                             // find the sq_ 
//     int offset_ = i % SQ_SIZE;                                              // and the offset
//     int i_ = path_follower[((sq_ * LOOKUP_COLS) + path_index)];             // map to a path (a col in big table)
//     int i__ = path_follower[((abs(i_) * LOOKUP_COLS) + DRAW_REMAP_COL)];    // draw remap col adj. for gaps in total-length strings
//     int dir = sign(i_);                                                     // sign is direction, accounting for paths opp. to string direction
//     if (dir < 0) {
//       // Serial.print(i); Serial.print(" "); Serial.println((i__ * SQ_SIZE) + (SQ_SIZE - offset_));
//       states_indiv_path[i] = (i__ * SQ_SIZE) + (SQ_SIZE - offset_);
//     } else {
//       // Serial.print(i); Serial.print(" "); Serial.println((i__ * SQ_SIZE) + offset_);
//       states_indiv_path[i] = (i__ * SQ_SIZE) + offset_;
//     }
//   }
// }

// // SEED FUNCTIONS

// void seed_states_single_color(CHSV states[], int num_states, uint8_t hue=0, uint8_t sat=255, uint8_t valu=255) {
//   for (int i=0; i< num_states; i++) {
//     states[i] = CHSV(hue, sat, valu);
//   }
// }

// // Value is static (defaults to 1)
// void seed_states_random(uint8_t states[], int num_states, uint8_t prob=12, uint8_t valu=1) {
//   for (int i=0; i< num_states; i++) {
//     if (random8() < prob) {
//       states[i] = valu;
//     }
//   }
// }

// // Value is a parameterized random range
// void seed_stars_and_states_random(uint8_t states[], int num_states, uint8_t states_indiv_state[], uint8_t prob=12, uint8_t valu=1, uint8_t min_rand=1, uint8_t rand_range= 24) {
//   for (int i=0; i< num_states; i++) {
//     if (random8() < prob) {
//       states[i] = valu;
//       states_indiv_state[i] = random8(rand_range) + min_rand;
//     }
//   }
// }

// void seed_leds_single(uint8_t states[], int idx=0) {
//   clear_states(states, 0);
//   states[idx]=1;
// }

// void seed_states_every_n(uint8_t states[], int num_states, int n=2, uint8_t valu=1, uint8_t jitter=0) {
//   // Serial.print(num_states); Serial.print(" seed every N"); Serial.println(n);
//   int rnd_offset = random(jitter) - (jitter/2);
//   for (int i=0; i<num_states; i++) { 
//     if ((i%n) == 0) {
// //      Serial.print(i);
//       states[(i+rnd_offset)]=valu;
//       rnd_offset = random(jitter) - (jitter/2);
//     }
//   }
//   // Serial.println();
// }

// void seed_stars_rand(uint8_t states[], int num_states, uint8_t density=12, uint8_t min_value=32, uint8_t range=192) {
  
//   Serial.print(" seed N random values: "); Serial.println(num_states);
//   for (int i=0; i< num_states; i++) {
//     if (random8() < density) {
//       // this will bias the distro towards the start of the array
//       states[i] = (random8() % range) + min_value;
//     }
//   }
// }

// void seed_states_bottom(uint8_t states[], int num_states) {
//   states[0]=1;
//   states[14]=1;
//   states[13]=1;
//   states[28]=1;
//   states[27]=1;
//   states[35]=1;
//   states[42]=1;
//   states[56]=1;
//   states[49]=1;
//   states[64]=1;
// }

// // TODO: seed states top

// void seed_bodies(uint8_t bodies[], int num_bodies) {
//   for (int i=0; i < num_bodies; i++) {
//     bodies[i] = 0;
//   }
// }

// void seed_hues_mono(CHSV currentHues[], int num_hues, uint8_t hue=0, uint8_t sat=255, uint8_t valu=255) {
// //  or write other functions
//   for (int i=0;i<num_hues;i++) {
//     currentHues[i] = CHSV(hue, sat, valu);
//   }
// }


// // STARS

// void display_stars_osc_bright(I2SClocklessLedDriver driver, uint8_t states_indiv[], int states_indiv_path[], uint8_t states_indiv_state[], CHSV color, uint8_t brightness_pulse_freq) {
//   // uint8_t color_i = 0;
//   CRGB temp = CRGB(0, 0, 0);
//   hsv2rgb_rainbow( color, temp);
  
//   for(int i=0;i<LIGHTABLE_LEDS;i++) {
    
//     if (states_indiv[i] > 0) {

//         // bright (BPM) will vary according to normed input data
//         driver.setPixel(states_indiv_path[i], temp.r, temp.g, temp.b, (beatsin8(brightness_pulse_freq, states_indiv_state[i], 224)));
    
//     }        
//   } 
// }

// void display_stars_twinkle_bright(I2SClocklessLedDriver driver, uint8_t states_indiv[], int states_indiv_path[], uint8_t states_indiv_state[], CHSV color, uint8_t bright) {
//   CRGB temp = CRGB(0, 0, 0);
//   hsv2rgb_rainbow( color, temp);
  
//   for(int i=0;i<LIGHTABLE_LEDS;i++) {
    
//     if (states_indiv[i] > 0) {

//         // bright (BPM) will vary according to normed input data
//         driver.setPixel(states_indiv_path[i], temp.r, temp.g, temp.b, beatsin8(states_indiv_state[i], 24, bright));
    
//     }        
//   } 
// }

// void display_stardust(I2SClocklessLedDriver driver, uint8_t states_indiv[], int states_indiv_path[],  CHSV color[], uint8_t bright_freq=24, uint8_t bright_max=224, uint8_t tail_length=1, uint8_t tail_speed=10) {
//   uint8_t color_i = 0;
//   CRGB temp = CRGB(0, 0, 0);

//   for(int i=0;i<LIGHTABLE_LEDS;i++) {
// //    Serial.print("display pixel "); Serial.println(i);

//     if (states_indiv[i] == 1) {

//       hsv2rgb_rainbow( color[color_i], temp);
      
//       if (tail_length == 0) {
//         tail_length = beatsin8(tail_speed, 1, 5);
//       }
//       for(int t=0; t<tail_length; t++) {
//         driver.setPixel(states_indiv_path[(i - t)], temp.r, temp.g, temp.b, (beatsin8(bright_freq, 24, bright_max) + random(24)));
//       }
//     }  else {

//       driver.setPixel(states_indiv_path[i], 0, 0, 0, 0);

//     }
//   }
// }

// void display_stardust_perm(I2SClocklessLedDriver driver, uint8_t states_indiv[], int8_t path_follower[], CHSV color[], uint8_t bright=24) {
//   uint8_t color_i = 0;
//   CRGB temp = CRGB(0, 0, 0);
//   for(int i=0;i<LIGHTABLE_LEDS;i++) {
// //    Serial.print("display pixel "); Serial.println(i);
//     int sq_ = floor(i/SQ_SIZE);                                             // find the sq_ 
//     int offset_ = i % SQ_SIZE;                                              // and the offset
//     int i_ = path_follower[((sq_ * LOOKUP_COLS) + FWD_PATH_COL_INF_KNOT)];     // map to a path (a col in big table)
//     int i__ = path_follower[((abs(i_) * LOOKUP_COLS) + DRAW_REMAP_COL)];    // draw remap col adj. for gaps in total-length strings
//     int dir = sign(i_);                                                     // sign is direction, accounting for paths opp. to string direction
//     if (states_indiv[i] == 1) {
      
//       hsv2rgb_rainbow( color[color_i], temp);
// //      Serial.println("");
// //      Serial.println(dir);
// //      Serial.println(i__);
// //      Serial.println(offset_);
// //      Serial.println(SQ_SIZE);
//       if (dir < 0) {
//         // bright (BPM) will vary according to normed input data
//         driver.setPixel(((i__ * SQ_SIZE) + (SQ_SIZE - offset_)), temp.r, temp.g, temp.b, (beatsin8(bright, 24, 224) + random(24)));
//       } else {
//         // bright (BPM) will vary according to normed input data
//         driver.setPixel(((i__ * SQ_SIZE) + offset_), temp.r, temp.g, temp.b, (beatsin8(bright, 24, 224) + random(24)));
//       }
//       // color_i++;
//     }
//   }
// }


// // SEGMENTS

// void draw_clear_segment(I2SClocklessLedDriver driver, int8_t path_follower[], int idx) { //, CHSV color=CHSV(0,0,0)) {
// //  CRGB temp = CRGB(0, 0, 0);
// //  hsv2rgb_rainbow( color, temp);
//   int start = path_follower[ (idx * LOOKUP_COLS) + DRAW_REMAP_COL ] * SQ_SIZE;
//   int end_ = start + SQ_SIZE;
//   for (int i = start; i < end_; i++) {
//     driver.setPixel(i, 0, 0, 0, 0);
//   }
// }


// // light + color segments
// void draw_solid_segment_sq_off(I2SClocklessLedDriver driver, int8_t path_follower[], CHSV color, int idx, int offset_=0, uint8_t bright=128) {
//   CRGB temp = CRGB(0, 0, 0);
//   hsv2rgb_rainbow(color, temp);
// //  Serial.println(path_follower[ (idx * LOOKUP_COLS) + DRAW_REMAP_COL ]);
//   int start = (path_follower[ (idx * LOOKUP_COLS) + DRAW_REMAP_COL ] * SQ_SIZE) + (offset_ % SQ_SIZE);
//   int end_ = start + SQ_SIZE;
// //  Serial.print("SOLID "); Serial.print(start); Serial.print(" - "); Serial.print(end_); Serial.println("");
//   for (int i = start; i < end_; i++) {
// //    Serial.print(i); Serial.print(" - "); Serial.print(temp.r); Serial.print(" "); Serial.print(temp.g); Serial.print(" "); Serial.println(temp.b);
// //    Serial.print("color v: "); Serial.println(color.v);
//     driver.setPixel(i, temp.r, temp.g, temp.b, bright);
//   }
// }

// // light + color segments
// void draw_solid_segment_sq(I2SClocklessLedDriver driver, int8_t path_follower[], CHSV color, int idx, uint8_t bright=128) {
//   draw_solid_segment_sq_off(driver, path_follower, color, idx, 0, bright);
// }

// // void draw_solid_blob_off(CRGB leds[], int8_t path_follower[], int idx, int offset_, int len, CHSV color) {
// //   int start = (path_follower[ (idx * LOOKUP_COLS) + DRAW_REMAP_COL ] * SQ_SIZE) + (offset_ % SQ_SIZE);
// //   int end_ = start + len;
// // //  Serial.print("SOLID "); Serial.print(start); Serial.print(" - "); Serial.print(end_); Serial.println("");
// //   for (int i = start; i < end_; i++) {
// //     leds[i]=color;
// //   }
// // }

// void display_color_segments(I2SClocklessLedDriver driver, uint8_t states[], int8_t path_follower[], CHSV colors[], bool mirror=false) {

// int color_i = 0;
//   for(int i=0;i<LIGHTABLE_SEGMENTS;i++) {
//     if (states[i] == 1) {
// //      Serial.print("display color segment SQ "); Serial.print(color_i); Serial.print(" "); Serial.print(states[i]); Serial.print(" "); Serial.println(i);
//       draw_solid_segment_sq(driver, path_follower, colors[color_i], i);
//       if (mirror == true) {
//         int ii = abs(path_follower[ (i * LOOKUP_COLS) +  5] % LIGHTABLE_SEGMENTS);
//         draw_solid_segment_sq(driver, path_follower, colors[color_i], ii);
//       }
//     } else {
//       draw_clear_segment(driver, path_follower, i);
//     }
//   }
// }

// void display_color_segments_jitter(I2SClocklessLedDriver driver, uint8_t states[], int8_t path_follower[], CHSV color[], int jitter=0, int max_body_length=3) {
//   int jit = 0;
//   for(int i=0;i<LIGHTABLE_SEGMENTS;i++) {
//     jit = random(jitter) - (jitter/2);
//     if (states[i+jit] == 1) {
// //      Serial.print("display color segment SQ w Jitter"); Serial.println(i);
//       draw_solid_segment_sq(driver, path_follower, color[0], i);
//     } else {
//       draw_clear_segment(driver, path_follower, i);
//     }
//   }
// }

// void display_dynamic_segments(I2SClocklessLedDriver driver, uint8_t states_indiv[], int states_indiv_path[], bool states_indiv__[], uint8_t segment_counts[], bool smear, CHSV color0, CHSV color1, int max_body_length) {
//   int w = 48;
//   int indiv_counter = 0;
//   CRGB temp = CRGB(0, 0, 0);
//   CHSV color_ = color0;
//   for(int i=0;i<(LIGHTABLE_LEDS);i++) {
    
//     if (states_indiv[i] == 50) {
    
//       // Serial.println(path_follower[((sq_ * LOOKUP_COLS) + FWD_PATH_COL_INF_KNOT)]);
//       // Serial.print("display color segment SQ: "); Serial.print(sq_); Serial.print(" "); Serial.print(offset_); Serial.print(" "); Serial.print(i_); Serial.print(" "); Serial.print(i__); Serial.print(" "); Serial.println(dir);
      
//       // first point (leader/head)
//       hsv2rgb_rainbow(color_, temp);
//       uint8_t segment_accum = 0;
//       // Serial.println(states_indiv_path[i]);
//       driver.setPixel(states_indiv_path[i], temp.r, temp.g, temp.b, w);
      
//       for (size_t s = 0; s < max_body_length; s++)
//       {
//         // 2 alternating colors are fed in at the top (to allow for animation changes)
//         if ((s % 2) == 0) {
//           color_ = color0;
//         } else {
//           color_ = color1;
//         }

//         hsv2rgb_rainbow( color_, temp);
//         for (size_t j=0; j<segment_counts[(indiv_counter*NUM_LEADERS)+ s]; j++) {
//           int j_ = (i - j - segment_accum) % LIGHTABLE_LEDS;
//   //          Serial.print("display bodies: "); Serial.print(j_); Serial.print(" "); Serial.print(sq_); Serial.print(" "); Serial.print(offset_); Serial.print(" "); Serial.println(i__);
//           driver.setPixel(states_indiv_path[j_], temp.r, temp.g, temp.b, w);
//         }
//         // post-iteration, don't include a gap after the head
//         segment_accum += segment_counts[s];
//       }

//       indiv_counter = (indiv_counter + 1) % NUM_LEADERS;
      
//     } else {

//       if (smear == false) {
//         driver.setPixel(states_indiv_path[i], 0, 0, 0, 0);
//       }
//     }
//   }
// }


// void display_dynamic_segments_fade(I2SClocklessLedDriver driver, uint8_t states_indiv[], int states_indiv_path[], bool states_indiv__[], uint8_t segment_counts[], bool smear, CHSV color0, CHSV color1) {
//   int w = 24;
//   int indiv_counter = 0;
//   CRGB temp = CRGB(0, 0, 0);
//   for(int i=0;i<(LIGHTABLE_LEDS);i++) {
    
    
//     if (states_indiv[i] == 50) {  
//       CHSV color_ = color0;
    
//       // Serial.println(path_follower[((sq_ * LOOKUP_COLS) + FWD_PATH_COL_INF_KNOT)]);
//       // Serial.print("display color segment SQ: "); Serial.print(sq_); Serial.print(" "); Serial.print(offset_); Serial.print(" "); Serial.print(i_); Serial.print(" "); Serial.print(i__); Serial.print(" "); Serial.println(dir);
      
//       // first point (leader/head)
//       hsv2rgb_rainbow(color_, temp);
//       uint8_t segment_accum = 0;
//       // Serial.println(states_indiv_path[i]);
//       driver.setPixel(states_indiv_path[i], temp.r, temp.g, temp.b, w);
      
//       for (size_t s = 0; s < 1; s++)
//       {
//         // 2 alternating colors are fed in at the top (to allow for animation changes)
//         // if ((s % 2) == 0) {
//         //   color_ = color0;
//         // } else {
//         //   color_ = color1;
//         // }

//         for (size_t j=1; j<3; j++) { //} segment_counts[(indiv_counter*NUM_LEADERS)+ s]; j++) {
//           int j_ = (i - j - segment_accum) % LIGHTABLE_LEDS;
//   //          Serial.print("display bodies: "); Serial.print(j_); Serial.print(" "); Serial.print(sq_); Serial.print(" "); Serial.print(offset_); Serial.print(" "); Serial.println(i__);
//           // color_.s = color_.s / 2;
//           color_.v = color_.v / 2;
//           hsv2rgb_rainbow(color_, temp);
//           driver.setPixel(states_indiv_path[j_], temp.r, temp.g, temp.b, w);
//         }
//         // post-iteration, don't include a gap after the head
//         segment_accum += segment_counts[s];
//       }

//       indiv_counter = (indiv_counter + 1) % NUM_LEADERS;
      
//     } else {

//       if (smear == false) {
//         driver.setPixel(states_indiv_path[i], 0, 0, 0, 0);
//       }
//     }
//   }
// }



// void display_color_sqs(I2SClocklessLedDriver driver, CHSV states[], int8_t path_follower[], uint8_t bright) {
//   int color_count = 0;
//   for(int i=0;i<LIGHTABLE_SEGMENTS;i++) {
// //  Serial.print("display color segment SQ "); Serial.print(color_count); Serial.print(" "); Serial.println(i);
// //  Serial.println(bright_normed);
//     draw_solid_segment_sq(driver, path_follower, states[i], i, bright);
//   }
// }

// void display_single_color_sqs(I2SClocklessLedDriver driver, CHSV color, int8_t path_follower[], uint8_t bright) {
  
//   for(int i=0;i<LIGHTABLE_SEGMENTS;i++) {
// //  Serial.print("display color segment SQ "); Serial.print(color_count); Serial.print(" "); Serial.println(i);
// //  Serial.println(bright_normed);
//     draw_solid_segment_sq(driver, path_follower, color, i, bright);
//   }
// }


// // NOISE POINTS!
// void update_leds_fill_points(uint8_t states_indiv[], int num_states, uint8_t fill_rate, uint8_t valu) {
//   for (size_t i=0; i< num_states; i++) {
//     if (random8() < fill_rate) {
//       states_indiv[i] = valu;
//     }
//   }
// }

// // PATH
// // lightable = total number of leds addressable (can be shorter than physical strand

// bool update_leds_follow_path(uint8_t states[], int num_states, int8_t path_follower[], uint8_t path_idx=2, uint8_t rate=12, uint8_t valu=1) {
//   bool dirty = false;
//   for(int i=0;i<num_states;i++) {
//     if ((states[i] > 0) && (random8() < rate)) { // update_rate = .5%
//       int ii = abs(path_follower[ (i * LOOKUP_COLS) +  path_idx] % LIGHTABLE_SEGMENTS);
//       // Serial.print("path traversal - i => 0: "); Serial.print(i); Serial.print(", ii => 1: "); Serial.print(ii); Serial.println("");
//       // collision avoidance
//       if (states[ii] != valu) {
//         dirty = true;
//         states[ii] = valu;
//         states[i] = 0;
//       }
//     }
//   }
//   return dirty;
// }


// bool update_leds_move_point(uint8_t states_indiv[], int num_states, bool states_indiv__[], uint8_t hop=1, uint8_t prob=4, uint8_t valu=1) {
  
//   bool dirty = false;
//   int ii = -1;
//   uint8_t step_ = hop;
//   // uint8_t step_ = random8(hop) + 1;
  
//   for(int i=0;i<num_states;i++) {
  
//     if ((states_indiv[i] > 0) && (random8() < prob)) { // update_rate = 5%
//       states_indiv__[i] = true;
//       // Serial.print("state flags: ");  Serial.println(states_indiv__[i]);
//       if (hop > 0) {
//         ii = (i + step_) % LIGHTABLE_LEDS;
//         states_indiv__[ii] = true;
//       } else {
//         ii = (i - step_) % LIGHTABLE_LEDS;
//         states_indiv__[ii] = true;
//       }
//       // Serial.print("path traversal - i => 0: "); Serial.print(i); Serial.print(", ii => 1: "); Serial.print(ii); Serial.println("");
//       if ((states_indiv[i] == valu) && (states_indiv[ii] != valu)) {
//         states_indiv[ii] = states_indiv[i];
//         states_indiv[i] = 0;
//         dirty = true;

//       }       
//     //  Serial.print(states_indiv[ii]); Serial.print(" "); Serial.println(states_indiv[i]);
//     } else {
//       states_indiv__[i] = false;
//     }
//   }
//   return dirty;
// }



// bool update_bodies(uint8_t bodies[], int num_bodies, int num_segments, int max_body_length, uint8_t prob=2, uint8_t shrink_prob=1) {
//   bool dirty = false;
//   int accum = 0;
//   for (size_t i=0; i<num_bodies; i++) {
//     for (size_t j=0; j<num_segments; j++) {
//       if (random8() < prob) {
//         bodies[(i*max_body_length)+j] = min(bodies[(i*max_body_length)+j] + 1, MAX_BODY_SEGMENT_SIZE);
//         dirty = true;
//       }
//       if (random8() < shrink_prob) {
//         bodies[(i*max_body_length)+j] = max(bodies[(i*max_body_length)+j] - 1, 0);
//         dirty = true;
//       }
//     }
//   }
//   return dirty;
// }


// // Update by a random speed (higer value = faster progression through color space.
// void update_current_hues(CHSV currentHues[], int numHues, int hueSpeed=1) {

//   for (int i=0; i<numHues; i++) {
//     currentHues[i] = CHSV(((currentHues[i].h + random(hueSpeed) + 0) % 256), currentHues[i].s, currentHues[i].v);
// //    log_idx_hsv(i, currentHues[i]);
//   }
// }

// void update_current_hues_damped(CHSV currentHues[], int numHues, int hueSpeed=1) {
//   uint8_t min_hue = 255;
//   for (int i=0; i<numHues; i++) {
//     min_hue = min(currentHues[i].h, min_hue);
// //    log_idx_hsv(i, currentHues[i]);
//   }
// //  Serial.print("min_hue "); Serial.println(min_hue);
//   for (int i=0; i<numHues; i++) {
// //    Serial.print(((currentHues[i].h + random(hueSpeed) + 0) % 256)); Serial.print(" ");
//     currentHues[i] = CHSV( min(((currentHues[i].h + random(hueSpeed) + 0) % 256), long(min_hue + 64)), currentHues[i].s, currentHues[i].v);
// //    log_idx_hsv(i, currentHues[i]);
//   }
//   Serial.println();
// }




//  counter++;
//  time2=ESP.getCycleCount();
//  driver.showPixels();
//  time3=ESP.getCycleCount();
//  Serial.printf("Calcul pixel fps:%.2f   showPixels fps:%.2f   Total fps:%.2f \n",(float)240000000/(time2-time1),(float)240000000/(time3-time2),(float)240000000/(time3-time1));
//  off++;
  // delay(2000);

  // delay(10);