/////////////////////////////////////////////////////////////////////////////
// csci 6101 project, a solution to problem "hexagon game"
// by Yang Wang, B00711141
/////////////////////////////////////////////////////////////////////////////
#define _CRT_SECURE_NO_WARNINGS
#define NM_INFILE "D-large-practice.in"
#define NM_OUTFILE "D-small.out"
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <memory>
#include <queue>
#include <ctime>
#include "hungary_km_template.h"
//
// int main();
//
int main() {
	clock_t progstt_time = clock();
	//
	// input and output files
	//
	FILE *fin = fopen(NM_INFILE, "r");
	FILE *fout = fopen(NM_OUTFILE, "w");
	//
	// case loop
	//
	int ccase = 0;
	fscanf(fin, " %d", &ccase);
	char line1st[400];
	char line2nd[250];
	int sttpos[75];	// its contents are normalized to 0-based after input
	int value[75];		// values 0 - 50
	// for case level
	//// adjacent matrix for the hexagon board, 4219 nodes max
	//char *adjmat = new char[17799961];	// 4219 * 4219
	// no more adjacent matrix; use array[6] instead
	typedef int Neighbor[6];
	Neighbor pos_ineighbor[4219];	// each position saves its neighbors
	int cneighbor[4219];						// neighbor count for each position
	int ichecker_idiag_idiagpos[75][3][75];	// 3 bipartite graphs
	int idiag_idiagpos[3][75];				// positions on diagonals
	// for per checker
	bool bfs_visited[4219];	// hashing for BFS shortest path search
	int bfs_len_got[4219];		// temporary path length storage
	// start
	fgets(line1st, 400, fin);	// consume the odd '\n'
	time_t timepassed = clock();
	for (int ic = 0; ic < ccase; ic++) { // case ic
		//
		// read two lines of the case and retrieve actual numbers
		//
		assert(fgets(line1st, 400, fin) != nullptr);
		assert(fgets(line2nd, 250, fin) != nullptr);
		int s = 0;
		char *sttmatch = line1st;
		while (*sttmatch != '\n')
			sttpos[s++] = strtol(sttmatch, &sttmatch, 10) - 1;	// normalize
		sttmatch = line2nd;
		for (int i = 0; i < s; i++) value[i] = strtol(sttmatch, &sttmatch, 10);
		//
		// add adjacency to positions on the board
		//
		// memset(adjmat, 0, 17799961*sizeof(char));
		memset(cneighbor, 0, 4219 * sizeof(int));
		int cpos = (3 * s*s + 1) / 4 ;		// total count of pos in this case
		int szmin_row = (s + 1) / 2;		// the first row and the last row
		int szrow = szmin_row;
		int pos_tovisit = 0;
		for (int irow = 0; irow < s; irow++) {	// enumerate hexboard rows
			// now szrow == size of row i
			// now pos_tovisit = the first postion in this row
			int bkp_szrow = szrow;	// backup
			// different strategies for different rows
			for (int ipos_rela = 0; ipos_rela < szrow; ipos_rela++) {
				// each relative position in this row
				int ipos_real = ipos_rela + pos_tovisit;		// the absolute pos
				int ipos_tgt[6]; int ctgt = 0;		// abs positions adjacent to this
				// determine the target positions
				if (irow < s / 2) {	// each row in [first row, middle row)
					ctgt = 3;
					ipos_tgt[0] = ipos_real + szrow;
					ipos_tgt[1] = ipos_tgt[0] + 1;
					ipos_tgt[2] = ipos_real + 1;
					// the last position doesn't have a right neighbor
					if (ipos_rela == szrow - 1) ctgt--;
				}
				else if (irow < s - 1) {	// each row in [middle row, last row)
					if (ipos_rela == 0) {	// no downward-left neighbor
						ctgt = 2;
						ipos_tgt[0] = ipos_real + szrow;
						ipos_tgt[1] = ipos_real + 1;
					}
					else if (ipos_rela == szrow - 1) {
						// no downward-right neighbor, no right neighbor
						ctgt = 1;
						ipos_tgt[0] = ipos_real + szrow - 1;
					}
					else {	// happy intermediate positions
						ctgt = 3;
						ipos_tgt[0] = ipos_real + szrow;
						ipos_tgt[1] = ipos_tgt[0] - 1;
						ipos_tgt[2] = ipos_real + 1;
					}
				}
				else {						// the last row										
					ctgt = 1;
					ipos_tgt[0] = ipos_real + 1;
					// the last position doesn't have a right neighbor
					if (ipos_rela == szrow - 1) ctgt--;
				}
				// add edges
				for (int i = 0; i < ctgt; i++) {	// each edge
					// adjmat[ipos_real * cpos + ipos_tgt[i]] = 1;
					// adjmat[ipos_tgt[i] * cpos + ipos_real] = 1;
					pos_ineighbor[ipos_real][cneighbor[ipos_real]++]
						= ipos_tgt[i];
					pos_ineighbor[ipos_tgt[i]][cneighbor[ipos_tgt[i]]++]
						= ipos_real;
				}
			} // for: relative positions of the row
			// update row size for the next row
			if (irow < s / 2) szrow++;
			else szrow--;
			// update the base number for ipos_rela, i.e., pos_tovisit
			pos_tovisit += bkp_szrow;
		} // for: rows
		assert(pos_tovisit == cpos && szrow == szmin_row - 1);	// checks
		//// adjmat test
		//printf("   "); for (int i = 0; i < cpos; i++) printf("%2d ", i); printf("\n");
		//for (int i = 0; i < cpos; i++) {
		//	printf("%2d ", i);
		//	for (int j = 0; j < cpos; j++) {
		//		printf("%2d ", int(adjmat[i*cpos + j]));
		//	}
		//	printf("\n");
		//}
		//printf("\n");
		// neighbor array test
		//for (int i = 0; i < cpos; i++) {
		//	printf("%d:", i);
		//	for (int j = 0; j < cneighbor[i]; j++) {
		//		printf(" %d", pos_ineighbor[i][j]);
		//	}
		//	printf("\n");
		//}
		//printf("\n");
		//
		// compute the shortest path for each checker to arrive at any
		// position on any one of the three diagonals: breadth-first-search
		//
		// first, find all positions in three diagonals
		int center_pos = cpos / 2;
		int idiagpos_center = s / 2;
		for (int i = 0; i < 3; i++) idiag_idiagpos[i][idiagpos_center] = center_pos;
		int diff1 = s, diff2 = s - 1;
		for (int i = 1; i <= s / 2; i++) {
			int lower_idiagpos = idiagpos_center - i;
			int higher_idiagpos = idiagpos_center + i;
			idiag_idiagpos[0][lower_idiagpos] = center_pos - i;
			idiag_idiagpos[0][higher_idiagpos] = center_pos + i;
			idiag_idiagpos[1][lower_idiagpos]
				= idiag_idiagpos[1][lower_idiagpos + 1] - diff1;
			idiag_idiagpos[1][higher_idiagpos]
				= idiag_idiagpos[1][higher_idiagpos - 1] + diff1;
			idiag_idiagpos[2][lower_idiagpos]
				= idiag_idiagpos[2][lower_idiagpos + 1] - diff2;
			idiag_idiagpos[2][higher_idiagpos]
				= idiag_idiagpos[2][higher_idiagpos - 1] + diff2;
			diff1--; diff2--;
		}
		//// diagonal filling test
		//for (int i = 0; i < 3; i++) {
		//	for (int j = 0; j < s; j++) {
		//		printf("%d ", idiag_idiagpos[i][j]);
		//	}
		//	printf("\n");
		//}
		//printf("\n");
		// start
		for (int ichecker = 0; ichecker < s; ichecker++) { // each checker
			memset(bfs_visited, 0, 4219 * sizeof(bool));	// reset
			std::queue<int> bfsq;
			int sttpos_checker = sttpos[ichecker];
			bfsq.push(sttpos_checker);
			bfs_visited[sttpos_checker] = true;
			bfs_len_got[sttpos_checker] = 0;
			// breadth-first search
			while (bfsq.size() > 0) {
				int currpos = bfsq.front(); bfsq.pop();
				for (int i = 0; i < cneighbor[currpos]; i++) {	// each neighbor
					int neighborpos = pos_ineighbor[currpos][i];
					if (bfs_visited[neighborpos]) continue;
					bfsq.push(neighborpos);
					bfs_visited[neighborpos] = true;
					bfs_len_got[neighborpos] = bfs_len_got[currpos] + 1;
				}
			}
			// compute path length, taking the checker values in consideration
			//// for testing, ignore the checker value
			//for (int i = 0; i < 75; i++) value[i] = 1;
			for (int idiag = 0; idiag < 3; idiag++) {
				for (int idiagpos = 0; idiagpos < s; idiagpos++) {
					ichecker_idiag_idiagpos[ichecker][idiag][idiagpos]
						= bfs_len_got[idiag_idiagpos[idiag][idiagpos]]
							* value[ichecker];
				}
			}
		} // for: each checker
		//// shortest path test
		//for (int i = 0; i < s; i++) {
		//	for (int j = 0; j < 3; j++) {
		//		for (int k = 0; k < s; k++) {
		//			printf("%d ", ichecker_idiag_idiagpos[i][j][k]);
		//		}
		//		printf("\n");
		//	}
		//	printf("\n");
		//}
		//printf("\n");
		//
		// three weighted, balanced and complete bipartite graph 
		// have been constructed, now run the KM algorithm
		//
		// first initialize the required vars in the template
		cleft = s; cright = s;
		int maxneg = MIN_INFI;
		// run KM algorithm for each diagonal
		for (int idiag = 0; idiag < 3; idiag++) { // each diagonal
			// copy negative weights
			for (int i = 0; i < s; i++) {
				for (int j = 0; j < s; j++) {
					weight[i][j] = - ichecker_idiag_idiagpos[i][idiag][j];
				}
			}
			// run it
			HKMprocess();
			// get the maximum sum of negative weights for this diag
			int curr_maxneg = 0;
			for (int i = 0; i < s; i++) curr_maxneg += weight[maty[i]][i];
			maxneg = max(maxneg, curr_maxneg);
		} // for: each diagonal
		//
		// output
		//
		fprintf(fout, "Case #%d: %d\n", ic + 1, -maxneg);
		printf("%d, %.0f\n", s,
			double(clock() - timepassed) / CLOCKS_PER_SEC * 1000);
		timepassed = clock();
	} // for: cases
	//
	// program ends
	//
	// delete[] adjmat;
	fclose(fin);
	fclose(fout);
	
	system("pause");
	return 0;
}
