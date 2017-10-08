#include "b_tree.h"

int main(int argc, char ** argv)
{


	if( argc != 4)
	{
		fprintf(stderr,"usage: ./gtest filename filesize(m*SEC_SZ) keysize\n");
		exit(0);
	}

	btree * bt;

	 //bt = (btree *) BTREEcreate(argv[1]);

//	long size = (long)SEC_SZ;
	long int size = (long int )atol(argv[2]);
	int kysz = atoi(argv[3]);

	long give = (long)size;

	
	printf("the size is NOW %d\n",give);


	printf("the size is NOW %ld\n",size);
	printf("the key size is NOW %d\n",kysz);

	bt = b_tree_create(argv[1], (long)size, kysz);

	return 0;
}
