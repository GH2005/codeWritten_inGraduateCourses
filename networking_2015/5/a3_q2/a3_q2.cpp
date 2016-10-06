#include <cstdio>
#include <vector>
#include <algorithm>
#define TYPE_LEN 20
#define IF_LEN 5

struct RoutingEntry {
	int c0;		// 4 -> 0.0.0.0, 3 -> 255.0.0.0 ... 0 -> 255.255.255.255.
	int m[4];		// the mask.
	int n[4];		// the network address.
	int t[4];		// the next hop.
	char *strType;	
	char *str_if;
	bool operator < (const RoutingEntry &m) const {
		return this->c0 < m.c0;
	}
};

int main() {
	//
	// read a routing table.
	//
	std::vector<RoutingEntry> rt; rt.reserve(100);
	char *nm_rt = new char[FILENAME_MAX];
	printf("Input a routing-table filename:\n");
	scanf(" %s", nm_rt);
	FILE *f_rt = fopen(nm_rt, "r");
	while (!feof(f_rt)) {
		// the mask.
		int the_c0 = 4; int the_m[4] = {};
		fscanf(f_rt, " %d", &the_m[0]); if (the_m[0] == 255) the_c0--;
		for (int i = 1; i < 4; i ++) {
			fscanf(f_rt, ".%d", &the_m[i]);
			if (the_m[i] == 255) the_c0--;
		}
		// the nw address.
		int the_n[4]; fscanf(f_rt, " %d", &the_n[0]);
		for (int i = 1; i < 4; i++) fscanf(f_rt, ".%d", &the_n[i]);
		// the next hop.
		int the_t[4]; fscanf(f_rt, " %d", &the_t[0]);
		for (int i = 1; i < 4; i++) fscanf(f_rt, ".%d", &the_t[i]);
		//
		// save this entry to vector rt.
		//
		rt.push_back(RoutingEntry());
		RoutingEntry *ptheEntry = &rt[rt.size() - 1];
		ptheEntry->strType = new char[TYPE_LEN];
		ptheEntry->str_if = new char[IF_LEN];
		fscanf(f_rt, " %s %s", ptheEntry->strType, ptheEntry->str_if);
		ptheEntry->c0 = the_c0;
		for (int i = 0; i < 4; i++) {
			ptheEntry->m[i] = the_m[i];
			ptheEntry->n[i] = the_n[i];
			ptheEntry->t[i] = the_t[i];
		}
	}
	fclose(f_rt); delete[] nm_rt;
	// sort the routing table.
	std::sort(rt.begin(), rt.end());
	//
	// read packets and decide outlets.
	//
	char *nm_pkt = new char[FILENAME_MAX];
	printf("Input a packet-list filename:\n");
	scanf(" %s", nm_pkt);
	FILE *f_pkt = fopen(nm_pkt, "r");
	while (!feof(f_pkt)) {
		int theIP[4]; fscanf(f_pkt, " %d", &theIP[0]);
		for (int i = 1; i < 4; i++) fscanf(f_pkt, ".%d", &theIP[i]);
		// decide the packet's outlet.
		for (int i = 0; i < rt.size(); i++) {	// for every routing entry.
			int masked[4];
			for (int j = 0; j < 4; j++) masked[j] = theIP[j] & rt[i].m[j];
			bool match = true;
			for (int j = 0; j < 4; j++) if (masked[j] != rt[i].n[j]) match = false;
			if (!match) continue;
			else {
				if (strcmp(rt[i].strType, "direct") == 0) {	// directly connected.
					printf("Packet with destination address %3d.%3d.%3d.%3d",
						theIP[0], theIP[1], theIP[2], theIP[3]);
					printf(" will be sent to one of its directly connected networks");
					printf(" on interface %3s.\n", rt[i].str_if);
				}
				else {
					printf("Packet with destination address %3d.%3d.%3d.%3d",
						theIP[0], theIP[1], theIP[2], theIP[3]);
					printf(" will be forwarded to %3d.%3d.%3d.%3d out",
						rt[i].t[0], rt[i].t[1], rt[i].t[2], rt[i].t[3]);
					printf(" on interface %3s.\n", rt[i].str_if);
				}
				break;
			}
		}
	}
	fclose(f_pkt);  delete[] nm_pkt;
	std::vector<RoutingEntry>::iterator it = rt.begin();
	for (; it != rt.end(); it++) { delete[] it->strType; delete[] it->str_if; }
	printf("Press any key to exit:\n");
	scanf("%*c%*c");
	return 0;
}