/////////////////////////////////////////////////////////////////////////////
// csci 6101 project, a coding template of Hungary & KM algorithm
// by Yang Wang, B00711141
// The Hungary Algorithm itself can achieve a maximum matching;
// however, to use KM, at least one perfect matching is required
/////////////////////////////////////////////////////////////////////////////
#pragma once
#define max(x, y) ((x)>(y)?(x):(y))
#include <memory>
//
// modify or init these things before invoking HKMprocess
//
#define KM
#define MIN_INFI -1000000000		// meanwhile for no edge
#define MAX_INFI 1000000000
#define TYPE int
#define MAX_CLEFT 75
#define MAX_CRIGHT 75
extern int cleft, cright;	// if not balanced, set cleft to be the smaller
extern TYPE weight[MAX_CLEFT][MAX_CRIGHT];
//
// facilities
//
// variables handled by the template
extern int maty[MAX_CRIGHT];
extern bool treeleft[MAX_CLEFT];
extern bool treeright[MAX_CRIGHT];
// KM addon
#ifdef KM
extern TYPE leftLabel[MAX_CLEFT];
extern TYPE rightLabel[MAX_CRIGHT];
extern TYPE d;
void InitLabels();
#endif
// InitMatch
void InitMatch();
// augmenting path searching
bool DFS(int theLeft);
// main routine of the algorithm
int HKMprocess();