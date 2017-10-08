/*This is b_tree.c a binary tree structure infrasturcture created by Gerald Jones for cs494 Lab 2*/

#include "b_tree.h"

void BtreeDiskRD(btree * bT, unsigned int LBA)
{
	jdisk_read(bT->dp,LBA,(void *)(bT->Buff) );
	bT->CLBA = LBA;
	return;
}

//NODE * makenode(FILE * fp, unsigned int LBA, int type)
NODE * makenode(btree * bT, unsigned int LBA, int type)
{
	//read needed disk into buffer
	if( bT->CLBA != LBA) BtreeDiskRD(bT, LBA);
	


	NODE * node = (NODE *)malloc(sizeof(NODE));

	//buff is a char pointer pointing to a disk
	bT->Buff = node->dp;

	node->type = type;
	node->dp[0] = type;				//node type
	node->dp[1] = 0;				//current number of keys
	node->disknumber = LBA;
	node->keyCnt = 0;;
	node->m = bT->MaxKey+1;
	node->keysize = bT->keysize;
	node->childCnt = 0;;
	node->keys = (node->dp+2);
	unsigned int idx = 2 + (node->m-1)*node->keysize;
	node->kids = node->dp + idx;
	

	return node;

}



void * BTREEcreate(char * JDisk)
{	
	int pnm = 1;

	//tr=3, tw=2, on=1, ze=0
	unsigned int tr,tw, on, ze;

	if(pnm) printf("the name of the jdisk is %s\n", JDisk);


	//ask for space for a btree
	btree  * bT = (btree *)malloc(sizeof( btree ) );

	//get a pointer to the jdisk
	bT->dp = jdisk_attach(JDisk);
	//get the size of the jdisk
	bT->disksize = jdisk_size(bT->dp);
	//calculate the total number of disk sectors
	bT->total = bT->disksize/SEC_SZ;
	
	//calculate and set the D and S values
	//I don't think i need this
	GetS(bT->total, bT);

	printf("S %u, D %u\n", bT->S, bT->D);
	
	
	
	//create a buffer that will hold a disk sector of bytes
	bT->Buff = (unsigned char * ) malloc(sizeof(unsigned char)*SEC_SZ);

	//read the first part of the jdisk to get the basic infor
	//Sect 0(bytes 0-3) = keysize as an 32 bit integer
	//sect 1(bytes 4-7) = LBA of root node on jdisk
	//sect 1(bytes 8-15(8-11if longs are 4 bytes ) ) = LBA of 1st free disk on jdisk, aka the number of used diskes(load sequentially)
	//jdisk_read(bT->dp,0,(void *)(bT->Buff) );
	BtreeDiskRD(bT, 0);

	//create a integer pointer to the buffer to move through as ints
	unsigned int * uip = (unsigned int *)bT->Buff;

	//unsigned int num = (unsigned int)bT->Buff[0];
	//read the key size as an int
	unsigned int num = (unsigned int )uip[0];
	
	//read the root number as an int
	unsigned int rt = (unsigned int)uip[1];

	tr = (unsigned int)bT->Buff[0];
	tw = (unsigned int)bT->Buff[1];
	on = (unsigned int)bT->Buff[2];
	ze = (unsigned int)bT->Buff[3];

	printf("%x %x %x %x\n",ze,on,tw,tr);

	//bT->keysize = 16*16*16*16*ze + 16*16*16*16*on + 16*16*tw + tr;
	bT->keysize = num;

	printf("Keys are %u bytes\n", (unsigned int) bT->keysize);
	printf("the keysize is %x\n", (unsigned int) bT->keysize);
	printf("the root node is disk %u\n", (unsigned int)rt);
	printf("the root is %x\n", (unsigned int) rt);

	bT->Root = rt;

	printf("the LBA of the Root is %u\n", (unsigned int) bT->Root);
	printf("the LBA of the Root in hex is %x\n", (unsigned int) bT->Root);


	//create a pointer to the first free sector, and the size of the 
	//current tree
	//read size of tree/first free disk as a long
	unsigned long * lp = (unsigned long* )(bT->Buff + 8);
	unsigned long ul = lp[0];;

	bT->size = (unsigned long)ul;

	printf("the LBA of the first free aka tree size is %ld\n", (unsigned long) bT->size);
	printf("the LBA of the first free aka tree size in hexis %x\n", (unsigned long) bT->size);

	bT->numkys = 0;
	//bT->


	return (void *)bT;
}


//calculates and sets S and D for the btree
unsigned short GetS(unsigned int total, btree * bT)
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
	bT->D = (total - rs);
	bT->S = rs;
	return rs;
}

int GetMaxKey(int keysize)
{

	return (SEC_SZ-6)/(keysize+4);
}


//im going to try to save the btree structure in the file after the setup stuff in disk/sector
//0
void * b_tree_create(char *filename, long size, int key_size)
{

	if(size%SEC_SZ != 0)
	{
		printf("Bad disk size %d\n",size);
		exit(0);
	}


		printf("The file size is %d\n",(long int)size);
		printf("The key size is %d\n",key_size);

	long i;

	FILE * fp = fopen(filename,"w");
	
	if(fp == NULL)
	{
		fprintf(stderr,"issue opening file %s\n",filename);
		exit(0);
	}

	//grab some space for a btree
	btree * bt = (btree *)malloc(sizeof(btree));

	//initialize the btree
	bt->filename = filename;
	bt->size = size;
	bt->keysize = key_size;
	bt->MaxKey = GetMaxKey(key_size);
	bt->fp = fp;

	//set up the initial node and root
	unsigned int root = 1;
	unsigned long lba = 1;

	fwrite(&key_size,4,1,fp);
	fwrite(&root,4,1,fp);
	fwrite(&lba,8,1,fp);
	printf("the fseek put it at in disk 0 is %d\n",(long int)ftell(fp));

	char * btreespace;

	int btidx = (int)ftell(fp);
	printf("the btidx is %d\n",btidx);

	bt->bts = fp;
	bt->btidx = btidx;

	fwrite(bt,1,sizeof(btree),fp);
	printf("the fseek is at this point %d\n",(long int)ftell(fp));


	//set the size of the file
	char NLL[1] = {'\0'};

	//fill the rest with null minus the 16 already used bytes
	for( i = 0; i < size-16 - sizeof(btree); i++)
	{
		fwrite(NLL,1,1,fp);
	}




	//set up the empty root node
	fseek ( fp, 0, SEEK_SET );
	fseek ( fp, 1*SEC_SZ , SEEK_SET );

	printf("the fseek put it at %d\n",(long int)ftell(fp));

	//set as an external node(0)
	unsigned char type = 0;
	
	fwrite(&type,1,1,fp);

	//set initial key count to 0
	unsigned char cnt = 0;
	fwrite(&cnt,1,1,fp);

	//keys start at fp go to fp+kysize*maxkey
	//char * kys = fp;

	int maxkey = GetMaxKey(key_size);

	printf("max key size is %d\n",maxkey);

	//fseek ( fp, 2+(maxkey*key_size)+1, SEEK_SET );
	//set the pointer to where the vals are
	fseek ( fp, 2+(maxkey*key_size)+1, SEEK_SET );
	printf("the fseek put it at %d\n",(long int)ftell(fp));

	printf("the size of a btree is %d\n",(int)sizeof(btree));
	//char * vals = fp;
	fclose(fp);



	fp = fopen(filename,"r");

	if(fp == NULL)
	{
		fprintf(stderr,"issue opening file %s\n",filename);
		exit(0);
	}

	loadB_tree(fp);


	return (void*)bt;
}
//get a pointer to this btree
void *b_tree_attach(char *filename)
{
	btree * bT;

	FILE * fp = fopen(filename,"r");

	if(fp == NULL)
	{
		fprintf(stderr,"issue opening file %s\n",filename);
		exit(0);
	}

	//bT = 

	return (void *)bT;
}

btree * loadB_tree(FILE * fp)
{
	btree * bt  = (btree *) malloc(sizeof(btree));
	//put file pointer at where the btree should be
	fseek(fp,16,SEEK_SET);

	//read btree into pointer
	fread(bt,1,sizeof(btree),fp);


	//check btree
	printf("the btree 's key size is %d\n", bt->keysize);
	printf("the btree 's maxkey is %d\n", bt->MaxKey);
	printf("the btree's name is %s\n",bt->filename);



	return bt;
}

unsigned int b_tree_insert(void *b_tree, void *key, void *record)
{
	unsigned int rtn;
	return rtn;

}

unsigned int b_tree_find(void *b_tree, void *key)
{
	unsigned int rtn;
	return rtn;
}

void *b_tree_disk(void *b_tree)
{
	btree * bT;
	return (void *)bT;

}

int b_tree_key_size(void *b_tree)
{
	unsigned int rtn;
	return (int)rtn;

}

void b_tree_print_tree(void *b_tree)
{
	btree * bT;
}

unsigned char * GetHigh(btree * bT, unsigned int LBA,unsigned int keynum)
{
	//read needed disk into buffer
	if( bT->CLBA != LBA) BtreeDiskRD(bT, LBA);

	//get the index of the key
	unsigned int idx = (LBA*SEC_SZ+2) * keynum;
	printf("idx is %u\n",idx);

	bT->Buff[idx];
	
}

//returns if this node is internal(1) or external(0)
int NODETYPE( btree * bT, unsigned int LBA)
{
	//read needed disk into buffer
	if( bT->CLBA != LBA) BtreeDiskRD(bT, LBA);

	return bT->Buff[0];
	
}

unsigned char * GETNODE(btree * bt, unsigned int LBA)
{
	
}

//returns the number of keys currently in this node
unsigned int NUMKYS( btree * bT, unsigned int LBA)
{
	//read needed disk into buffer
	if( bT->CLBA != LBA) BtreeDiskRD(bT, LBA);

	return bT->Buff[1];
}



//returns a pointer to the asked for key number
//you give it the LBA of the node on the disk
unsigned char *  GetKey(btree * bT, unsigned int keynumber,unsigned int LBA)
{
	//read needed disk into buffer
	if( bT->CLBA != LBA) BtreeDiskRD(bT, LBA);
	
	unsigned char * key = (unsigned char *)malloc( sizeof(char)*bT->keysize);
					
	             //   = disknumber(sec_sz) + 2 + kynm*keysize
	unsigned int idx = bT->keysize*(keynumber-1)+(LBA*SEC_SZ)+2;

	return memcpy(key, bT->Buff+idx,bT->keysize);
	
}

unsigned char GetVal(btree * bT, unsigned int keynum, unsigned int LBA, int hl)
{

	
}
