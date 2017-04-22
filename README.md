# **Event Counter**

The project uses Red Black Trees to build an event counter. 
Each event has two fields of information: ID and *count*, where count is the number of active events with the given ID.
The RB-Tree stores IDs only when the *count* for that ID is greater than 0.
The features supported by the application are 
1. Increase(ID,m)
2. Reduce(ID,m)
3. Count(ID)
4. InRange(ID1, ID2)
5. Next(ID)
6. Previous(ID)

## **Using/Running the project**

To run the project:
	Go to Project directory and use command 'make' to create a binary.
	Then use : ./bbst <test filename> < <command filename>
		eg. ./bbst test_100.txt < commands.txt 
	to test.

Compiler used was standard GNU C++ (g++) compiler. 

## **Function prototypes :**

a) tree_node* initTree(tree_node* p,int start,int end,int max_ht,int cur_ht) => 

	This Function builds the initial tree from the input in O(n) time. The function takes in 5 input parameters, a tree node ,  a start index of array , an end index of array, max height the tree can have for n items, current height of the tree. This function recursively builds the tree nad then returns the root node.

b) tree_node* bstInsert(int id,int count) =>
	
	This function takes in the id and count for new node to be inserted. Creates a tree node inserts in int the Tree. If required calls the balance function to balance the Red Blakc tree. It returns the root node.

c) void balance(tree_node* node) =>
	
	This function is called if after an insertion the Tree needs to be balanced. The function takes input as a tree node where there is a coloring issue to be fixed. The function then balances the tree by recoloring or rotation, to push the coloring issue up the tree until it is fixed.

d) tree_node* leftRotate(tree_node* node) =>

	The function takes tree node as an input. It performs a left roatation and return bakc the same node in the roatated tree.

e) tree_node* rightRotate(tree_node* node) =>

	The function takes tree node as an input. It performs a right roatation and return bakc the same node in the roatated tree.

f) int increase(int id,int m) =>
	
	This function is called when an 'Increase id m' command is encountered. The function takes id and m as input and returns the new count of the 'id'. If there was no node in tree with ID euqal to 'id' then it calls 'bstInsert' to create a new node.

g) tree_node* search(int id) =>

	This function takes and id as input. This function searches for a node with this 'id' in the Tree and returns pointer to that node. If no such node is available NULL pointer is retuned. This function is used for 'count' command.

h) int inrange(tree_node* node,int id_low,int id_high) =>
	
	This function is used to do a range search on the Red Black Tree. It takes 3 inputs, a tree node, low id and high id for the range to be searched. The function returns the sum of counts of all IDs in the range in RB Tree.

i) tree_node* next(int id) =>
	
	This function takes an id as input. If id is present in tree the function returns pointer to the successor of node with ID = 'id'. Else the function returns pointer to the node with smallest ID greater than id in the Tree. Note that this function is used to find successor as well as next.

j) tree_node* prev(int id) =>
	
	This function takes an id as input. If id is present in tree the function returns pointer to the predecessor of node with ID = 'id'. Else the function returns pointer to the node with greatest ID smaller than id in the Tree. Note that this function is used to find predecessor as well as previous.

k) int reduce(int id,int m) =>

	This function is called when a 'reduce id m' command is encountered. The function takes two inputs, id and m. If id is not present in tree it reutns zero. If Id is present and count becomes zero on reduction, then the node is removed and fixup() function is called to balance tree. If the count does not become zero on reduction then the new count is returned or else a zero is retuned.

l) void fixup(tree_node* node) =>

	This function is called to fixup the coloring problem after a node is deleted form the Tree. The pointer of this node is given as an input. The function does recoloring and roatation as required to push the recoloring problem up the tree till it is solved.




