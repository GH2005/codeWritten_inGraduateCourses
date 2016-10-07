// csci 6708, assignment 2, answer 2
// coded by Yang Wang B00711141

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cstring>
#include <vector>
#include <cassert>
#include <iostream>
#define ACL_FILE "acl_collection.txt"
#define PKT_FILE "packets.txt"

struct AclItem {
	int prot;				// 0 -> ip, 1 -> tcp, 2 -> udp, 3 -> icmp
	int src_ip[4];			// all -1 for any
	int src_wc[4];			// wildcard mask, -1 for any
	int dst_ip[4];			// all -1 for any
	int dst_wc[4];			// wildcard mask, -1 for any
	int src_port;			// -1 for unset
	int dst_port;			// -1 for unset
	bool is_ext;
	bool is_permit;
	AclItem() {
		prot = 0;
		for (int i = 0; i < 4; i++) {
			src_ip[i] = -1; src_wc[i] = -1;
			dst_ip[i] = -1; dst_wc[i] = -1;
		}
		src_port = -1; dst_port = -1;
		is_ext = 0; is_permit = 0;
	}
};

void StrToIp(char *str, int arr[]) {
	char *start = str;
	for (int i = 0; i < 4; i++) {
		arr[i] = atoi(start);
		start = strchr(start, '.') + 1;
	}
}

struct Packet {
	int src_ip[4], dst_ip[4], prot, src_pt, dst_pt;
};

bool Match(AclItem *p_item, Packet *p_pkt) {
	// return true if it is a match
	// protocol
	if (p_item->prot != 0 && p_item->prot != p_pkt->prot)
		return false;
	// source IP
	if (p_item->src_ip[0] != -1) {
		for (int i = 0; i < 4; i++) { // enumerate bytes
			int item_or_wc = p_item->src_ip[i] | p_item->src_wc[i];
			int pkt_or_wc = p_pkt->src_ip[i] | p_item->src_wc[i];
			if (item_or_wc != pkt_or_wc) return false;
		}
	}
	// destination IP
	if (p_item->dst_ip[0] != -1) {
		for (int i = 0; i < 4; i++) { // enumerate bytes
			int item_or_wc = p_item->dst_ip[i] | p_item->dst_wc[i];
			int pkt_or_wc = p_pkt->dst_ip[i] | p_item->dst_wc[i];
			if (item_or_wc != pkt_or_wc) return false;
		}
	}
	// source port
	if (p_item->src_port != -1
		&& p_item->src_port != p_pkt->src_pt) return false;
	// destination port
	if (p_item->dst_port != -1
		&& p_item->dst_port != p_pkt->dst_pt) return false;
	return true;
}

int main() {
	FILE *f_acl = fopen(ACL_FILE, "r");
	FILE *f_pkt = fopen(PKT_FILE, "r");

	char *acl_lead = new char[4];
	char *pkt_lead = new char[13];
	fscanf(f_acl, " %s", acl_lead);
	while (strcmp(acl_lead, "END") != 0) {	// enumerate ACLs
		int index;
		fscanf(f_acl, " %d", &index);
		//
		// read items of this ACL into a vector
		//
		std::vector<AclItem> acl;
		char *sub_acl_lead = new char[12];
		char *permit_str = new char[7];
		char *prot_str = new char[5];
		fscanf(f_acl, " %s", sub_acl_lead);
		while (strcmp(sub_acl_lead, "interface") != 0) {
			// enumerate items
			int acl_num = 0;
			fscanf(f_acl, " %d", &acl_num);		// acl number
			fscanf(f_acl, " %s", permit_str);		// permit/deny
			AclItem item;
			if (strcmp(permit_str, "permit") == 0)
				item.is_permit = true;
			else item.is_permit = false;
			char *ip_buffer = new char[16];
			if (acl_num < 100) {	// standard acl
				item.is_ext = 0;
				fscanf(f_acl, " %s", ip_buffer);	// source ip
				if (ip_buffer[0] != 'a') { // a real dot-formatted
					// if it is "any", things are already done.
					StrToIp(ip_buffer, item.src_ip);
					fscanf(f_acl, " %s", ip_buffer);	// wildcard mask
					StrToIp(ip_buffer, item.src_wc);
				}
				fscanf(f_acl, " %s", sub_acl_lead);
			}
			else {					// extended acl
				item.is_ext = 1;
				fscanf(f_acl, " %s", prot_str);	// protocol
				if (strcmp(prot_str, "tcp") == 0) item.prot = 1;
				else if (strcmp(prot_str, "udp") == 0) item.prot = 2;
				else if (strcmp(prot_str, "icmp") == 0) item.prot = 3;
				fscanf(f_acl, " %s", ip_buffer);	// source ip
				if (ip_buffer[0] != 'a') { // a real dot-formatted
					// if it is "any", things are already done.
					StrToIp(ip_buffer, item.src_ip);
					fscanf(f_acl, " %s", ip_buffer);	// wildcard mask
					StrToIp(ip_buffer, item.src_wc);
				}
				fscanf(f_acl, " %s", ip_buffer);	// "eq" or dest ip
				if (strcmp(ip_buffer, "eq") == 0) {	// src port
					fscanf(f_acl, " %d", &item.src_port);
					fscanf(f_acl, " %s", ip_buffer);
				}
				// destination ip read
				if (ip_buffer[0] != 'a') { // a real dot-formatted
					// if it is "any", things are already done.
					StrToIp(ip_buffer, item.dst_ip);
					fscanf(f_acl, " %s", ip_buffer);	// wildcard mask
					StrToIp(ip_buffer, item.dst_wc);
				}
				fscanf(f_acl, " %s", ip_buffer);	// "eq" or sub_acl_lead
				if (strcmp(ip_buffer, "eq") == 0) {	// dst port
					fscanf(f_acl, " %d", &item.dst_port);
					fscanf(f_acl, " %s", sub_acl_lead);
				}
				else {	// next sub_acl_lead read already
					strcpy(sub_acl_lead, ip_buffer);
				}
			}
			delete[] ip_buffer;
			// add the item into vector
			acl.push_back(item);
		} // while - enumerate items
		fscanf(f_acl, " %*s ip access-group %*d %*s");
		delete[] sub_acl_lead;
		delete[] permit_str;
		delete[] prot_str;
		//
		// judge every packet of the PACKET_GROUP for this ACL
		//
		printf("Judgements on packets of group %d ", index);
		printf("based on ACL %d:\n", index);
		fscanf(f_pkt, " PACKET_GROUP %*d");	// omit the header
		char *srcip_buf = new char[16];
		char *dstip_buf = new char[16];
		char *prot_buf = new char[5];
		fscanf(f_pkt, " %s", srcip_buf);
		while (strcmp(srcip_buf, "END_GROUP") != 0) {
			// srcip_buf contains really an IP
			// enumerate packets of this group
			Packet pkt;
			StrToIp(srcip_buf, pkt.src_ip);
			fscanf(f_pkt, " %s", dstip_buf);	// destination IP
			StrToIp(dstip_buf, pkt.dst_ip);
			fscanf(f_pkt, " %s", prot_buf);	// protocol
			if (strcmp(prot_buf, "tcp") == 0) pkt.prot = 1;
			else if (strcmp(prot_buf, "udp") == 0) pkt.prot = 2;
			else if (strcmp(prot_buf, "icmp") == 0) pkt.prot = 3;
			else pkt.prot = 0;
			fscanf(f_pkt, " %d %d", &pkt.src_pt, &pkt.dst_pt);

			// judge it and give an output line.
			bool permit = false;
			for (int i = 0; i < acl.size(); i++) {
				if (Match(&acl[i], &pkt)) {
					permit = acl[i].is_permit;
					break;
				}
			}
			printf("%s\t%s\t%s\t%d\t%d\t", srcip_buf, dstip_buf,
				prot_buf, pkt.src_pt, pkt.dst_pt);
			if (permit) printf("permitted\n");
			else printf("denied\n");

			fscanf(f_pkt, " %s", srcip_buf);
		} // while - enumerate packets of this group
		delete[] srcip_buf, dstip_buf, prot_buf;

		printf("\n");
		fscanf(f_acl, " %s", acl_lead);
	} // while - enumerate ACLs

	delete[] acl_lead;
	delete[] pkt_lead;
	fclose(f_acl);
	fclose(f_pkt);
	system("pause");
	return 0;
}