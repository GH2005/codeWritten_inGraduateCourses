/////////////////////////////////////////////////////////////////////////////
// csci 6101 project, a coding template of Hungary & KM algorithm
// by Yang Wang, B00711141
// The Hungary Algorithm itself can achieve a maximum matching;
// however, to use KM, at least one perfect matching is required
/////////////////////////////////////////////////////////////////////////////
#include "hungary_km_template.h"

int cleft, cright;	// if not balanced, set cleft to be the smaller
TYPE weight[MAX_CLEFT][MAX_CRIGHT];
//
// facilities
//
// variables handled by the template
int maty[MAX_CRIGHT];
bool treeleft[MAX_CLEFT];
bool treeright[MAX_CRIGHT];
// KM addon
#ifdef KM
TYPE leftLabel[MAX_CLEFT];
TYPE rightLabel[MAX_CRIGHT];
TYPE d;
#endif

// KM addon
#ifdef KM
void InitLabels() {
	for (int i = 0; i < cleft; i++) {
		TYPE maxweight = MIN_INFI;
		for (int j = 0; j < cright; j++) {
			if (weight[i][j] > maxweight) maxweight = weight[i][j];
		}
		leftLabel[i] = maxweight;
	}
	memset(rightLabel, 0, sizeof(rightLabel));
}
#endif
// InitMatch
void InitMatch() {
	for (int i = 0; i < cright; i++) maty[i] = -1;
}
// augmenting path searching
bool DFS(int theLeft) {
	treeleft[theLeft] = true;
	for (int i = 0; i < cright; i++) {	// each right node
		if (treeright[i]) continue;	// only a new right node counts
		#ifdef KM
		TYPE t = leftLabel[theLeft] + rightLabel[i] - weight[theLeft][i];
		if (t == 0) { // it's a prerequisite to be in the equal subgraph
			treeright[i] = true;
			if (maty[i] == -1 || DFS(maty[i])) {
				maty[i] = theLeft;
				return true;
			}
		}
		else if (d > t) {
			d = t;
		}
		#else
		if (weight[theLeft][i] == MIN_INFI) continue;
		treeright[i] = true;
		if (maty[i] == -1 || DFS(maty[i])) {
			maty[i] = theLeft;
			return true;
		}
		#endif
	} // for: each right node
	// when false is returned, the alternate tree is a full one
	return false;
}
// main routine of the algorithm
int HKMprocess() { // return count of DFSSucc.
	#ifdef KM
	InitLabels();
	#endif // KM
	InitMatch();
	int csucc_dfs  = 0;
	for (int i = 0; i < cleft; i++) { // each left node
		memset(treeleft, 0, sizeof(treeleft));
		memset(treeright, 0, sizeof(treeright));
		#ifdef KM
		d = MAX_INFI;
		while (!DFS(i)) {
			// update labels.
            for (int j = 0; j < cleft; j++) {
                if (!treeleft[j]) continue;
                leftLabel[j] -= d;
            }
            for (int k = 0; k < cright; k++) {
				if (!treeright[k]) continue;
                rightLabel[k] += d;
            }
			d = MAX_INFI;
			memset(treeleft, 0, sizeof(treeleft));
			memset(treeright, 0, sizeof(treeright));
        }
        #else
        if (DFS(i)) csucc_dfs++;
        #endif
	}
	return csucc_dfs;
}