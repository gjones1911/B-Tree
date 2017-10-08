# B-Tree
This is the third lab for cs494 fall 2017. It is an implementation of a B-Tree.
from:http://cis.stvincent.edu/html/tutorials/swd/btree/btree.html
"A B-tree is a specialized multiway tree designed especially for use on disk. In a B-tree each node may contain a large number of keys. The number of subtrees of each node, then, may also be large. A B-tree is designed to branch out in this large number of directions and to contain a lot of keys in each node so that the height of the tree is relatively small. This means that only a small number of nodes must be read from disk to retrieve an item. The goal is to get fast access to the data, and with disk drives this means reading a very small number of records. Note that a large node size (with lots of keys in the node) also fits with the fact that with a disk drive one can usually read a fair amount of data at once."

A B-tree of order m consists of:
--an order tree where each node has at most m children
--if k is the actual number of children in the node then
  k-1 is the number of keys in the node
--keys are in ascending order in each node
--All leaves are on the bottom level.
--All internal nodes (except perhaps the root node) have at least ceil(m / 2)     (nonempty) children.
--The root node can have as few as 2 children if it is an internal node, and 
  can obviously have no children if the root node is a leaf (that is, the 
  whole tree consists only of the root node)
--Each leaf node (other than the root node if it is a leaf) must contain at 
  least ceil(m / 2) - 1 keys.
--if m = 4 then At each node the following holds true:
	*subtree at node-branch[0] has only keys less than node-key[0]
	*subtree at node-branch[1] has only keys more than node-key[0]
	 but less than node-key[1]
	*subtree at node-branch[2] has only keys greater than node-key[1]
	 but less than node-key[2]
	*subtree at node-branch[3] has only keys greater than node-key[2]
