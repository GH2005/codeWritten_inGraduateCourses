#include <cstdio>
#include <map>
#include <ctime>

inline void PrintFDB(std::map<char, int> &fdb);

int main() {
	// parameters.
	int mxPortIdx = 0;  char lastHost = 'A'; int cItemSnap = 0;

	// the forwarding database.
	std::map<char, int> fdb;

	// initialization via a file.
	printf("Input the file name of a bridge snapshot:\n");
	char *nmINsnap = new char[FILENAME_MAX];
	scanf(" %s", nmINsnap);
	FILE *fINsnap = fopen(nmINsnap, "r");
	fscanf(fINsnap, " %d %c %d", &mxPortIdx, &lastHost, &cItemSnap);
	char c = 'A'; int p = 0;
	for (int i = 0; i < cItemSnap; i++) {
		fscanf(fINsnap, " %c %d", &c, &p);
		fdb.insert(std::pair<char, int>(c, p));
	}
	PrintFDB(fdb);
	fclose(fINsnap);
	delete[] nmINsnap;

	// a global loop.
	int choice = -1;
	printf("Input 0 -> exit or 1 -> generate random frames:\n");
	scanf(" %d", &choice);
	while (choice != 0) {
		// random frames.
		printf("Input the number of random frames to be generated:\n");
		int cFrame = 0;  scanf(" %d", &cFrame);
		srand((unsigned)time(NULL));
		for (int i = 0; i < cFrame; i++) {
			char src = 'A' + rand() % (lastHost - 'A' + 1);
			char tgt = 'A' + rand() % (lastHost - 'A' + 1);
			int port = rand() % (mxPortIdx + 1);
			printf("Frame from %c to %c arrives at port %3d ---> ", src, tgt, port);

			// main logic of the bridge.
			// source.
			std::map<char, int>::iterator its = fdb.find(src);
			if (its != fdb.end()) {	// entry found.
				if (its->second == port) printf("No FDB update      ---> ");
				else { its->second = port; printf("FDB update [%c %3d] ---> ", its->first, its->second); }
			}
			else { // entry not found.
				fdb.insert(std::pair<char, int>(src, port));
				printf("FDB update [%c %3d] ---> ", src, port);
			}
			// destiniation.
			std::map<char, int>::iterator itd = fdb.find(tgt);
			if (itd != fdb.end()) {	// destination found.
				if (itd->second == port) printf("Frame is discarded\n");
				else printf("Frame is sent out through port %3d\n", itd->second);
			}
			else {	// destination not found.
				printf("Frame is broadcasted on output ports\n");
			}
		}

		// output the final FDB.
		PrintFDB(fdb);

		printf("Input 0 -> exit or 1 -> generate random frames:\n");
		scanf(" %d", &choice);
	}

	return 0;
}

inline void PrintFDB(std::map<char, int> &fdb) {
	printf("Current forwarding database:\n");
	std::map<char, int>::iterator it = fdb.begin();
	while (it != fdb.end()) {
		printf("[%c %3d]\n", it->first, it->second); it++;
	}
}