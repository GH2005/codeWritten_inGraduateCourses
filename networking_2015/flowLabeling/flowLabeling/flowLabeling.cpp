#include <cstdio>
#include <map>
#include <cassert>
#define IANA_FILENAME "0-1023-service-names-port-numbers.txt"
#define FLOW_FILENAME "1998Testing.flow"
#define LABELED_TCP "1998Testing_labeled_tcp.flow"
#define LABELED_UDP "1998Testing_labeled_udp.flow"
#define FLOW_LEN 300

int main() {

	// read the iana file.
	//
	FILE *f_iana = fopen(IANA_FILENAME, "r");
	std::map<int, char *> m_iana;
	char *strApp = new char[20];
	char *tmp_uk = new char[20];
	int port = -1;
	do {
		fscanf(f_iana, " %s", strApp);
		if (strcmp(strApp, "unknown") != 0) {
			fscanf(f_iana, " %s", tmp_uk);
			assert(strcmp(tmp_uk, "unknown") == 0);	// my assumption.
		}
		fscanf(f_iana, " %d", &port); assert(port >= 0 && port <= 1023);
		// add it to or modify it in m_iana.
		m_iana[port] = new char[20]; strcpy(m_iana[port], strApp);
		// ignore the remainder.
		fscanf(f_iana, "%*[^\n]"); fscanf(f_iana, "%*c");
	} while (!feof(f_iana));
	delete[] tmp_uk;
	delete[] strApp;
	fclose(f_iana);

	// read the flow file and add AppLabels.
	// 
	FILE *fFlow = fopen(FLOW_FILENAME, "r");
	FILE *fTCP = fopen(LABELED_TCP, "w");
	FILE *fUDP = fopen(LABELED_UDP, "w");
	//
	int p1 = -1, p2 = -1; int prot = 0;
	char *carrier = new char[FLOW_LEN];
	char *carrier1 = new char[FLOW_LEN];
	char *carrier2 = new char[FLOW_LEN];
	do {
		fscanf(fFlow, " %[^,]", carrier); 
		fscanf(fFlow, ",%d,", &p1);
		fscanf(fFlow, "%[^,]", carrier1); 
		fscanf(fFlow, ",%d", &p2);
		fscanf(fFlow, ",%d", &prot);
		fscanf(fFlow, "%s", carrier2);
		//
		FILE *f_right = prot == 6 ? fTCP : fUDP;
		//
		fprintf(f_right, "%s", carrier);
		fprintf(f_right, ",%d,%s", p1, carrier1);
		fprintf(f_right, ",%d,%d%s", p2, prot, carrier2);
		// look up the iana map and add the label.
		int p = p1 > p2 ? p2 : p1;
		fprintf(f_right, ",%s\n", m_iana[p]);
		fscanf(fFlow, "%*c");
	} while (!feof(fFlow));
	delete[] carrier;
	fclose(fFlow);
	fclose(fTCP);
	fclose(fUDP);

	//
	for (int i = 0; i < 1024; i++) delete[] m_iana[i];
	return 0;
}