#ifndef _BP_TREE_
#define _BP_TREE_

#include "jdisk.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define SEC_SZ  1024

//using namespace std:

//represents a node on a B-tree
//for an order M b-tree has at most m kids,
//at least (ceil(m/2))kids, and if it has 
//k kids then it has k-1 keys
typedef struct node 
{
		int disknumber;
		int type;
		int keyCnt;
		int m;
		int keysize;
		int childCnt;
		char * dp;
		char * keys; // a set of pointer to a pointer to a SEC_SZ number of bytes
		char * kids; //a set of pointers to sectors on jdisks/nodes

}NODE;


typedef struct Btree 
{
	//void btree(void * dp);
	//void DBtree();
	//int GetMaxKey();
	//unsigned short GetS(unsigned int Total_Sectors)
	unsigned int D;		//number of disk sectors
	unsigned int S;		//number of disk sectors
	unsigned int disksize;		//size of disk in bytes
	unsigned int total;		//total number of disk sectors
	int keysize;        //size in bytes of buffers that store keys
	int MaxKey;
	unsigned int CLBA;
	unsigned int Root;		//LBA of root disk
	long size;				//the size of the tree(aka, the first one on the free list)
	unsigned char numkys;
	char * filename;
	long filesize;
	void * dp; //disk pointer to a jdisk
	FILE * fp;
	FILE * bts;
	int btidx;
	unsigned char * Buff;	//a disksize buffer for reading off of disk
	char * jdsk;			//name of jdisk that is being read
} btree;

//read the given disk from the given btree into btree buffer
void BtreeDiskRD(btree * bt, unsigned int LBA);

unsigned short GetS(unsigned int total, btree * bT);

//calculates the max key size
int GetMaxKey(int keysize);


//creates a btree from the jdisk filename of size size, and
//key size key_size
void *b_tree_create(char *filename, long size, int key_size);

//
void *b_tree_attach(char *filename);


unsigned int b_tree_insert(void *b_tree, void *key, void *record);


unsigned int b_tree_find(void *b_tree, void *key);

void *b_tree_disk(void *b_tree);
int b_tree_key_size(void *b_tree);
void b_tree_print_tree(void *b_tree);
btree * loadB_tree(FILE * fp);


//represents and item held in a node on the b-tree
//has a void start pointing to a type *, where
//type = 1--char
//	   = 2--int
//	   = 3 string
//	   = 4 cstring
//	   = 5 float
//	   = 6 long
typedef struct
{
		int type;
		void * value;
		char * name;
}item;




typedef struct
{
	char * key;
	NODE less;
	NODE more;
	unsigned int num;
}bkey;

NODE makeNode(int keysize, int keycnt, int childcnt);


void * splitNode(NODE nd);

#endif
