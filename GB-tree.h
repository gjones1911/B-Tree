#ifndef _BP_TREE_
#define _BP_TREE_

#include "jdisk.h"

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cmath>

#define SEC_SZ  1024

using namespace std:

class B-tree
{
	public:
		B-tree(void * dp);
		~B-tree();
		int keysize;
		int GetMaxKey();
		unsigned short GetS(unsigned int Total_Sectors)
		unsigned int D;		//number of disk sectors
		unsigned int S;		//number of fat sectors
		unsigned int Root;		//LBA of root disk
		long size;				//the size of the tree(aka, the first one on the free list)
		unsigned char numkys;
		void * dp; //disk pointer 
		unsigned char * Buff;
		string jdsk;
};

B-tree::B-tree(char * JDisk)
{
	//get the tdisk;
	dp = jdisk_attach(JDisk);
	unsigned int disksize = jdisk_size(dp);
	unsigned int total = disksize/SEC_SZ;
	GetS(total);
	Buff = (unsigned char * ) malloc(sizeof(unsigned char)*SEC_SZ);
	
	//read in first sector on disk to get info
	//read byte 0 -> 3 to get key size
	jdisk_read(dp,0,(void *)Buff);

	(unsigned int)keysize =(unsigned int)Buff[0];

	printf("the keysize is %u\n", (unsigned int) keysize);


	(unsigned int)Root =(unsigned int)Buff[(4/2) -1];
	
	
	printf("the LBA of the Root is %u\n", (unsigned int) Root);
	

	(unsigned long)size =(unsigned long)Buff[1];
	
	
	printf("the LBA of the Root is %l\n", (unsigned long) Root);


}

unsigned short B-Tree::GetS(unsigned int total)
{
	unsigned int rs = 1, d = 0, shortsNded = 0, currentBytes = 0, N = 0;

	while(1)
	{
		d = (total - rs);

		shortsNded = ((d+1))*2;

		currentBytes = rs*SEC_SZ;

		if(rs*SEC_SZ >= shortsNded)
		{
			break;
		}
		else rs++;

	}
	D = (total - rs);
	S = rs;
	return rs;
}




int B-tree::GetMaxKey()
{

	return (SEC_SZ-6)/(keysize+4);
}


//represents and item held in a node on the b-tree
//has a void start pointing to a type *, where
//type = 1--char
//	   = 2--int
//	   = 3 string
//	   = 4 cstring
//	   = 5 float
//	   = 6 long
class item
{
	public:
		int type;
		void * value;
		string name;
};


//represents a node on a B-tree
//for an order M b-tree has at most m kids,
//at least (ceil(m/2))kids, and if it has 
//k kids then it has k-1 keys
class NODE
{
	public:
		int keyCnt;
		int childCnt;
		item key[];
		NODE();
		~NODE();
		NODE kids[];

};


void *b_tree_create(char *filename, long size, int key_size);
void *b_tree_attach(char *filename);

unsigned int b_tree_insert(void *b_tree, void *key, void *record);
unsigned int b_tree_find(void *b_tree, void *key);

void *b_tree_disk(void *b_tree);
int b_tree_key_size(void *b_tree);
void b_tree_print_tree(void *b_tree);



#endif
