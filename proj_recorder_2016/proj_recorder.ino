// ***********************************************************************************
// project recorder
// team 5: Yang Wang
// ***********************************************************************************

// ---------------- Modify these before run (if you know how) -------------------------
//
#define MODE 1
#define M1_FINGER_MAINTAIN 1000 // how long you should maintain your fingers in mode 1
#define M2_UNIT_NOTE_LEN 2000   // lower value means higher speed in mode 2
#define INTERVAL_RATIO 0.2      // relative time interval for indicating note
const int gc_ledsockets[8] = {2, 3, 4, 5, 6, 7, 8, 9}; // pin ordinals on arduino
const int gc_inputsockets[8] = {13, 12, 11, 10, 14, 15, 16, 17};  // similar
const bool gc_note_hole[9][8] = { // confirmed on a real recorder
  1, 1, 1, 1, 1, 1, 1, 1, // 0 --- do not cover, not enlightened, LOW
  1, 1, 1, 1, 1, 1, 1, 0,
  1, 1, 1, 1, 1, 1, 0, 0,
  1, 1, 1, 1, 1, 0, 1, 1,
  1, 1, 1, 1, 0, 0, 0, 0,
  1, 1, 1, 0, 0, 0, 0, 0,
  1, 0, 1, 1, 1, 0, 0, 0,
  1, 0, 1, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, // default (null) note
};
// the music score
const int gc_seq_attri[][2] = {
  3, 1,
  4, 1,
  5, 1,
  3, 1,
  7, 3,
  5, 1,
  4, 2,
  7, 2,
  4, 2,
  3, 1,
  1, 1,
  5, 3,
  3, 1,
  2, 4,
//  3, 1,
//  2, 1,
//  1, 2,
//  2, 2,
//  3, 1,
//  4, 1,
//  0, 2,
//  3, 2,
//  4, 1,
//  5, 1,
//  6, 2,
//  6, 1,
//  5, 1,
//  4, 1,
//  3, 1,
//  4, 4,
  8, 100};  // should end with a null note
//const int gc_seq_attri[][2] = { 7, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 8, 1 };
const String gc_free = "FREE MODE";     // yes, the free mode
const String gc_m1done = "MODE 1 DONE"; // a constant string for mode 1
const String gc_m2done_half = "PERFORMANCE MARK: "; // a partial constant string in mode 2
const String gc_keyboard_base = "________     ";

// ----------------------------- dynamic variables --------------------------
//
#define _countof(x) (sizeof(x) / sizeof (x[0]))
int g_cseq = 0;       // for mode 1&2
int g_curr_seq = 0;   // for mode 1&2
int g_render_seq = 0; // for output module 1, mode 1&2
bool g_isNull = 0;    // for output module 1, mode 1&2
unsigned long g_seq_timespan;   // for mode 2
unsigned long g_seq_strt_time;  // for mode 2
int g_cloops = 0;               // for mode 2
int g_cmatches = 0;             // for mode 2
bool g_beingRight = 0;                  // for mode 1
unsigned long g_rightness_strtTime = 0; // for mode 1

// ------------------------------- features ----------------------------------
//
// output module 1: led lights for fingering output
//
void ShowFingering() {
  int notecode = gc_seq_attri[g_render_seq][0];
  const bool *pIsHigh = gc_note_hole[notecode];
  for (int i = 0; i < 8; i++) { // i --- hole code
    digitalWrite(gc_ledsockets[i], pIsHigh[i]);
  }
}

// output module 2: output to a serial monitor emulator
//
void SeeStrings(const String *pstr1, const String *pstr2) {
  // clear the screen first; the code is from Internet
  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command

  // display
  if (pstr1)  Serial.println(*pstr1);
  else        Serial.println();
  if (pstr2)  Serial.println(*pstr2);
  else        Serial.println();
}

// input module: sensors
//
int SensorToNotecode() {  // return the sensed notecode immediately
  bool states[8];
  for (int i = 0; i < 8; i++) states[i] = digitalRead(gc_inputsockets[i]);
  for (int i = 0; i < 8; i++) { // determine which note it is; i --- notecode
    bool isThis = true;
    for (int j = 0; j < 8; j++) {
      if (states[j] != gc_note_hole[i][j]) {
        isThis = false;
        break;
      }
    }
    if (isThis) return i;
  }
  return 8;
}

// ------------------------------- setup ----------------------------------
//
void setup() {
  // initialize pins
  for (int i = 0; i < 8; i++) { // i --- hole code
    pinMode(gc_ledsockets[i], OUTPUT);
    pinMode(gc_inputsockets[i], INPUT);
  }

  // serial output
  Serial.begin(1000000);
  
  // 3-second preface
  if (MODE == 1 || MODE == 2) {
    String preface = "MODE 0 STARTING IN 3...";
    preface.setCharAt(5, '0' + MODE);
    for (int i = 3; i > 0; i--) {
      preface.setCharAt(preface.length() - 4, '0' + i);
      SeeStrings(&preface, NULL);
      delay(1000);
    }
  }
  else {
    String preface = "FREE MODE STARTING IN 3...";
    for (int i = 3; i > 0; i--) {
      preface.setCharAt(preface.length() - 4, '0' + i);
      SeeStrings(&preface, NULL);
      delay(1000);
    }
  }
  SeeStrings(NULL, NULL);
  delay(1000);
  
  // initialize variables
  g_cseq = _countof(gc_seq_attri) - 1;
  g_curr_seq = 0;
  g_render_seq = 0;
  g_isNull = 0;
  g_seq_timespan = M2_UNIT_NOTE_LEN * gc_seq_attri[g_curr_seq][1];
  g_seq_strt_time = millis();
  g_cloops = 0;
  g_cmatches = 0;
  g_beingRight = 0;
  g_rightness_strtTime = 0;
}

// ------------------------------ refreshing ---------------------------------
//             
void loop() {
  //
  // read sensor input
  //
  int played_note = SensorToNotecode();
  int note_of_curr_seq = gc_seq_attri[g_curr_seq][0];
  //
  // update data
  //
  // main logics
  if (MODE == 1) {
    if (g_isNull) {
      if (millis() >= g_rightness_strtTime + M1_FINGER_MAINTAIN * INTERVAL_RATIO) {
        // null time is over
        g_isNull = 0;
        if (g_curr_seq < g_cseq) {
          g_curr_seq++;          
        }
        g_render_seq = g_curr_seq; // for output module 1
        g_beingRight = 0;
      }
    }
    else {  // not in null time
      if (played_note == note_of_curr_seq) { // the player does it right
        if (g_beingRight) {
          if (millis() >= g_rightness_strtTime + M1_FINGER_MAINTAIN) {
            // being right for enough time; enter null time
            // always being right during null time
            g_render_seq = g_cseq;  // for output module 1
            g_isNull = 1;
            g_rightness_strtTime = millis();
          }
        }
        else { 
          g_beingRight = 1;
          g_rightness_strtTime = millis();
        }
      }
      else {  // the player doesn't make it right this moment
        g_beingRight = 0;
      }
    }
  } // MODE == 1
  else if (MODE == 2) {
    // update fingering data
    unsigned long curr_time = millis();
    if (curr_time >= g_seq_strt_time + g_seq_timespan) { // a seq has stood for enough time
      if (g_curr_seq < g_cseq) {
        g_curr_seq++;              
      }
      g_seq_timespan = M2_UNIT_NOTE_LEN * gc_seq_attri[g_curr_seq][1];
      g_render_seq = g_curr_seq; // for output module 1
      g_seq_strt_time = curr_time;
      g_isNull = 0;
    }
    else if (!g_isNull && curr_time >= g_seq_strt_time + g_seq_timespan * (1 - INTERVAL_RATIO)) {
      g_render_seq = g_cseq;  // for output module 1
      g_isNull = 1;
    }
    // precision statistics
    if (g_curr_seq < g_cseq) { // stop counting when the tune finishes
      g_cloops++;
      if (played_note == note_of_curr_seq) g_cmatches++;
    }
  } // MODE 2
  else {  // free mode
    g_render_seq = g_cseq;  // for output module 1
  }
  
  // for output module 2
  // string 1
  const String *p1;
  String str1_note = gc_keyboard_base;
  String str1_msg = String(gc_m2done_half + double(g_cmatches)/double(g_cloops) * 100 + "%");
  if (MODE == 1 || MODE == 2) {
    if (g_curr_seq < g_cseq) {  // a valid music seq
      str1_note.setCharAt(note_of_curr_seq, note_of_curr_seq + '0');
      str1_note = "You should play " + str1_note; 
      p1 = &str1_note;
    }
    else {  // the null note
      if (MODE == 1) p1 = &gc_m1done;
      else if (MODE == 2) p1 = &str1_msg;
    }
  }
  else {  // free mode
     p1 = &gc_free;
  }
  // string 2
  String str2 = gc_keyboard_base;
  str2.setCharAt(played_note, played_note + '0');
  str2 = "You are playing " + str2;
  //
  // "draw a frame"
  //
  ShowFingering();        // output module 1
  SeeStrings(p1, &str2);  // output module 2
  // a small delay
  delay(100);
}
