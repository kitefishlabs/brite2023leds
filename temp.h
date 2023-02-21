#define NUM_LEVELS 11

int PW[22] = {47,47,45,43,42,41,38,38, 100,98,96, 47,47,45,43,42,41,38,38, 100,98,96};
int H = 14;
int D[8] = { 8, 9, 9, 8, 8, 9, 8, 8 };

int LEVELMAP[NUM_LEVELS][2][8] = {
  //  strip-idx  start-degrees  end-degrees #leds1 actual-led-idx.                                       length      direction on/off
    { { 0,       205,           335,        47,     ((PW[3]*2) + D[1] + (PW[2]*2) + (PW[1]*2) + D[0]),   (PW[0]*2),  1, 1},
      { 2,        25,           155,        47, (0 + (PW[11+3]*2)+D[5]+(PW[10+3]*2) + (PW[9+3]*2)+D[4]+(PW[8+3]*2)),    (PW[8+3]*2),  -1, 1}},
    { { 0,       205,           335,        47,     ((PW[3]*2) + D[1] + (PW[2]*2) + (PW[1]*2)),          (PW[1]*2), -1, 1},
      { 2,        25,           155,        47, (0 + (PW[11+3]*2)+D[5]+(PW[10+3]*2)),                                   (PW[9+3]*2),   1, 1}},
    { { 0,       205,           335,        45,     ((PW[3]*2) + D[1]),                                  (PW[2]*2),  1, 1},
      { 2,        25,           155,        45, (0 + (PW[11+3]*2)+D[5]+(PW[10+3]*2)),                                   (PW[10+3]*2), -1, 1}},
    { { 0,       205,           335,        43,      (PW[3]*2),                                          (PW[3]*2), -1, 1},
      { 2,        25,           155,        43,  0,                                                                     (PW[11+3]*2),  1, 1}},
    { { 1,       205,           335,        42, (H + (PW[7]*2) + D[3] + (PW[6]*2) + (PW[5]*2) + D[2]),   (PW[4]*2),  1, 1},
      { 3,        25,           155,        42, (H + (PW[15+3]*2)+D[7]+(PW[14+3]*2) + (PW[13+3]*2)+D[6]+(PW[12+3]*2)),  (PW[12+3]*2), -1, 1}},
    { { 1,       205,           335,        41, (H + (PW[7]*2) + D[3] + (PW[6]*2) + (PW[5]*2)),          (PW[5]*2), -1, 1},
      { 3,        25,           155,        41, (H + (PW[15+3]*2)+D[7]+(PW[14+3]*2)),                                   (PW[13+3]*2),  1, 1}},
    { { 1,       205,           335,        38, (H + (PW[7]*2) + D[3]),                                  (PW[6]*2),  1, 1},
      { 3,        25,           155,        38, (H + (PW[15+3]*2)+D[7]+(PW[14+3]*2)),                                   (PW[14+3]*2), -1, 1}},
    { { 1,       205,           335,        38, (H + (PW[7]*2)),                                         (PW[7]*2), -1, 1},
      { 3,        25,           155,        38,  H,                                                                     (PW[15+3]*2),  1, 1}},
    { { 4,         0,           360,       100,  (PW[8]*2), (PW[8]*2),   -1, 1},
      { 4,         0,           360,       100,  (PW[8]*2), (PW[8]*2),   -1, 1}},
    { { 5,         0,           360,        98,  (PW[9]*2), (PW[9]*2),   -1, 1},
      { 5,         0,           360,        98,  (PW[9]*2), (PW[9]*2),   -1, 1}},
    { { 6,         0,           360,        96,  (PW[10]*2), (PW[10]*2), -1, 1},
      { 6,         0,           360,        96,  (PW[10]*2), (PW[10]*2), -1, 1}}
};

void setup() {
  // put your setup code here, to run once:
  
  int *index  = 0;
  int *strip  = 0;
  
  Serial.begin(115200);
  Serial.println(angle_to_index(60, 0));
  Serial.println(angle_to_index(60, 1));
  Serial.println(angle_to_index(60, 2));
  Serial.println(angle_to_index(60, 1));
  Serial.println(angle_to_index(60, 5));
  Serial.println(angle_to_index(160, 9));
  Serial.println(angle_to_index(260, 7));
  Serial.println(angle_to_index(10, 1));
  Serial.println(angle_to_index(20, 4));
  Serial.println(angle_to_index(300, 2));
  Serial.println(angle_to_index(2, 3));
  Serial.println(angle_to_index(359, 1));

  // Serial.println(*index);
  // Serial.println(*strip);

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(10); // this speeds up the simulation
}


int angle_to_index(int angle, int lvl) {
  
  int side = 0;
  if ((angle % 360) < 180) {
    side = 1;
  }

  int* lvlsd = LEVELMAP[lvl][side];
  
  int theta_start = lvlsd[1];
  int theta_end = lvlsd[2];

  int stripid = lvlsd[0];
  int seg_offset = lvlsd[4];
  int seg_length = lvlsd[5];


  int dir = lvlsd[6];
  // Serial.print(theta_start); Serial.print(" ");
  // Serial.print(theta_end); Serial.print(" ");
  // Serial.print(stripid); Serial.print(" ");
  // Serial.println(seg_offset);
  // Serial.print(seg_length); Serial.print(" ");
  // Serial.print(side); Serial.print(" ");
  // Serial.println(dir); 
  // Serial.print((float((angle - theta_start) % 360) / float(theta_end - theta_start))); Serial.print(" ");
  // Serial.println((float((theta_end - angle) % 360) / float(theta_end - theta_start)));
  
  if (lvl < 8) {
    if ((angle >= theta_start) && (angle <= theta_end)) {
      if (dir > 0) {
        return seg_offset + int((float((angle - theta_start) % 360) / float(theta_end - theta_start)) * float(seg_length));
      } else {
        return seg_offset - int((float((theta_end - angle) % 360) / float(theta_end - theta_start)) * float(seg_length));
      }
    }
    
  } else {
        return int((float((360 - angle) % 360) / 360.0) * float(seg_length));
    
  }
  return -1;

}