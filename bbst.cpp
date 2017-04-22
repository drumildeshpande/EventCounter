#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<string>
#include<iostream>
#include<fstream>
#include<cmath>
#define SIZE 100000000
using namespace std;

enum colors {RED,BLACK};

struct tree_node
{
	int ID;
	int count;
	colors color; 
	struct tree_node* left;
	struct tree_node* right;
	struct tree_node* parent;
};
tree_node* root = NULL;

int init_id[SIZE];
int init_cnt[SIZE];

tree_node* initTree(tree_node*,int,int,int,int);
tree_node* bstInsert(int,int);
void inorder1(tree_node*);
void balance(tree_node*);
tree_node* leftRotate(tree_node*);
tree_node* rightRotate(tree_node*);
int increase(int,int);
int reduce(int,int);
tree_node* search(int);
int inrange(tree_node*,int,int);
tree_node* next(int);
tree_node* prev(int);
void fixup(tree_node*);

int main(int argc, char *argv[])
{
	int N,id,count,i=0;
	string line,t;
	ifstream fin(argv[1]);
	if(argc==2)
	{
		getline(fin,line);
		N = atoi(line.c_str());
		while(i<N && !fin.eof())	// parse input into id and count and store them in an array.
		{
			getline(fin,line);
			size_t pos = line.find(" ");
			t = line.substr(0,pos);
			id = atoi(t.c_str());
			line.erase(0,pos+1);
			count = atoi(line.c_str());
			//root = bstInsert(id,count);
			init_id[i] = id;
			init_cnt[i] = count;
			i++;
		}
		int height = log(N)/log(2);
		root = initTree(root,0,N-1,height,0);	// build Intial tree
		//inorder1(root);
		//cout<<"done"<<endl;
		char buffer[256];
		string line,command,temp,quit;
		string delimiter = " ";
		int arg1 , arg2;
		arg1 = arg2 = 0;
		bool flag = true;
		while(flag)
		{
			getline(cin,line);		// parse commands
			size_t pos = line.find(delimiter);
			command = line.substr(0,pos);
			if(pos!= string::npos)	
			{
				line.erase(0,pos+1);
				pos = line.find(delimiter);
				temp = line.substr(0,pos);
				arg1 = atoi(temp.c_str());
				if(pos!= string::npos)
				{
				
					line.erase(0,pos+1);
					pos = line.find(delimiter);
					temp = line.substr(0,pos);
					arg2 = atoi(temp.c_str());
				}
			}
			if(command == "quit")
			{
				flag = false;
			}
			else if(command == "increase")
			{
				int new_count = increase(arg1,arg2);
				cout<<new_count<<endl;
			}
			else if(command == "reduce")
			{
				int new_count = reduce(arg1,arg2);
				cout<<new_count<<endl;
				//inorder1(root);
			}
			else if(command == "count")
			{
				tree_node* new_count = search(arg1);
				if(new_count != NULL)
				{
					cout<<new_count->count<<endl;
				}
				else
				{
					cout<<"0"<<endl;
				}
			
			}
			else if(command == "inrange")
			{
				int total_cnt = inrange(root,arg1,arg2);
				cout<<total_cnt<<endl;
			}
			else if(command == "next")
			{
				tree_node* succ = next(arg1);
				if(succ!=NULL)
				{
					cout<<succ->ID<<" "<<succ->count<<endl;
				}
				else
					cout<<"0 0"<<endl;
			}
			else if(command == "previous")
			{
				tree_node* pred = prev(arg1);
				if(pred!=NULL)
				{
					cout<<pred->ID<<" "<<pred->count<<endl;
				}
				else
					cout<<"0 0"<<endl;
			}
		}
		//inorder1(root);
	}
	return 0;
}

/* Function to insert new node into the Red Black Tree */
tree_node* bstInsert(int id,int count)
{
	tree_node* node = root;
	tree_node* prev;
	if(root==NULL)
	{
		node = new tree_node;
		node->ID = id;
		node->count = count;
		node->left = node->right = NULL;
		node->parent = NULL;
		node->color = RED;
		root = node;
	}
	else
	{
		while(node!= NULL)
		{
			prev = node;
			if(node->ID <= id)
			{
				node = node->right;
			}
			else
			{
				node = node->left;
			}
		}
		node = new tree_node;
		node->ID = id;
		node->count = count;
		node->left = node->right = NULL;
		node->parent = prev;
		node->color = RED;
		if(node->ID <= prev->ID)
			prev->left = node;
		else
			prev->right = node;
	}
	//inorder1(root);
	balance(node);
	return root;
}

/* Function to print InOrder traversal of the Red Black Tree */
void inorder1(tree_node* root)
{
	if(root!=NULL)
	{
		inorder1(root->left);
		if(root->parent == NULL)
			cout<<"ID:"<<root->ID<<", count:"<<root->count<<", color:"<<root->color<<endl;
		else		
			cout<<"ID:"<<root->ID<<", count:"<<root->count<<", parentId:"<<root->parent->ID<<", color:"<<root->color<<endl;
		inorder1(root->right);
	}
}

/* Function to Balance the Red Black Tree after Insertion */
void balance(tree_node* node)
{
	if(node == root)
	{
		node->color = BLACK;
	}
	else
	{
		while(node != root && node->parent->parent!=NULL && node->parent->color == RED)	// continue till fix is required
		{
			tree_node* grandParent = node->parent->parent;
			//cout<<grandParent->ID<<endl;
			if(grandParent->left == node->parent)	// Parent is left child
			{
				if(grandParent->right == NULL || (grandParent->right->color == BLACK))	// Uncle black
				{
					if(node == node->parent->right)	// Left-Right case
					{
						node = node->parent;
						node = leftRotate(node);						
					}
					node->parent->color = BLACK;	// left-left Case
					grandParent->color = RED;
					node = rightRotate(grandParent);
				}
				else
				{
					if(grandParent->right->color == RED)	// if Uncle is Red
					{
						node->parent->color = BLACK;		// Exchange colors to push coloring problem upwards
						grandParent->right->color = BLACK;
						grandParent->color = RED;
						node = grandParent;			
					}
				}
			}
			else	// Parent is right child
			{
				if((grandParent->left == NULL) || (grandParent->left->color == BLACK))	// black uncle
				{
					if(node == node->parent->left)	//right-left case
					{
						node = node->parent;
						node = rightRotate(node);
					}
					node->parent->color = BLACK;	//right-right case
					grandParent->color = RED;
					node = leftRotate(grandParent);
				}
				else
				{
					if(grandParent->left->color == RED)		// if Uncle is red
					{
						node->parent->color = BLACK;
						grandParent->left->color = BLACK;
						grandParent->color = RED;
						node = grandParent;
					}
				}
			}
		}	
	}
	root->color = BLACK;
}


/* Function For Left Roatation */
tree_node* leftRotate(tree_node* node)
{
	if(node->right == NULL)
	{
		return node;
	}
	else
	{
		tree_node* temp = node->right;
		if(temp->left!=NULL)
		{
			node->right = temp->left;
			temp->left->parent = node;
		}
		else
		{
			node->right = NULL;
		}

		if(node->parent==NULL)
		{
			root = temp;
			root->parent = NULL;
		}
		else
		{
			if(node == node->parent->left)
			{
				temp->parent = node->parent->left;
				node->parent->left = temp;
			}
			else
			{
				temp->parent = node->parent;
				node->parent->right = temp;
			}
		}
		temp->left = node;
		node->parent = temp;
	}
	return node;
}

/* Function for Right Rotation */
tree_node* rightRotate(tree_node* node)
{
	if(node->left == NULL)
	{
		return node;
	}
	else
	{
		tree_node* temp = node->left;
		if(temp->right!=NULL)
		{
			node->left = temp->right;
			temp->right->parent = node;
		}
		else
		{
			node->left = NULL;
		}

		if(node->parent==NULL)
		{
			root = temp;
			root->parent = NULL;
		}
		else
		{
			if(node == node->parent->left)
			{
				temp->parent = node->parent->left;
				node->parent->left = temp;
			}
			else
			{
				temp->parent = node->parent->right;
				node->parent->right = temp;
			}
		}
		temp->right = node;
		node->parent = temp;
	}
	return node;	
}

/* Function to increase count of a Node */
int increase(int id,int m)
{
	int ans = 0;
	tree_node* node = root;
	bool flag = false;
	while(node!=NULL && flag == false)
	{
		if(node->ID < id)
		{
			node = node->right;
		}
		else if(node->ID > id)
		{
			node = node->left;
		}
		else
		{
			node->count = node->count + m;
			ans = node->count;
			flag = true;
		}
	}
	if(ans == 0)
	{
		root = bstInsert(id,m);
		ans = m;
	}
	return ans;
}

/* Function to search for a Node in RB Tree */ 
tree_node* search(int id)
{
	bool flag = false;
	tree_node* node = root;
	while(node!=NULL && flag == false)
	{
		if(node->ID < id)
		{
			node = node->right;
		}
		else if(node->ID > id)
		{
			node = node->left;
		}
		else
		{
			return node;
		}
	}
	return NULL;
}

/* Function to do Range Search in RB Tree */
int inrange(tree_node* node,int id_low,int id_high)
{
	if(node!=NULL)
	{
		if(node->ID < id_low)
		{
			inrange(node->right,id_low,id_high);
		}
		else if(node->ID > id_high)
		{
			inrange(node->left,id_low,id_high);
		}
		else if((node->ID >= id_low) && (node->ID <= id_high))
		{
			return (node->count + inrange(node->left,id_low,id_high) + inrange(node->right,id_low,id_high));
		}
	}
	else
	{
		return 0;
	}
}

/* Function to Find successor and Next of a node in RB Tree */
tree_node* next(int id)
{
	//cout<<"in next";
	tree_node* node = search(id);
	if(node!= NULL)
	{
		if(node->right!=NULL)		// find Min node greated than id in the right subtree
		{
			node = node->right;
			while(node->left!=NULL)
			{
				node = node->left;
			}
			return node;
		}
		else
		{
			tree_node* temp = node->parent;
			while(temp!=NULL && temp->right == node)	// go above a tree till a node that is left child is seen
			{
				node = temp;
				temp = temp->parent;
			}
			return temp;
		}
	}
	else
	{
		tree_node* x = root;
		tree_node* y = root;
		bool flag = true;
		int min;
		min = abs(x->ID-id);
		while(x!=NULL)
		{
			if(x->ID < id)
			{
				x = x->right;
			}
			else
			{
				x = x->left;
			}
			if(x!= NULL && (min > abs(x->ID-id)) && x->ID > id)
			{
				min = abs(x->ID-id);
				y = x;
			}
		}
		if(y->ID > id)
			return y;
		else
			return NULL;
	}
}

/* Function to find Predecessor and Previous in Red Black Tree */
tree_node* prev(int id)
{
	tree_node* node = search(id);
	if(node!= NULL)
	{	
		if(node->left != NULL)		// find Max node less than id in the left subtree
		{
			node = node->left;
			while(node->right!=NULL)
			{
				node = node->right;
			}
			return node;
		}
		else
		{
			tree_node* temp = node->parent;
			while(temp!=NULL && temp->left == node)	// go above a tree till a node that is right child is seen
			{
				node = temp;
				temp = temp->parent;
			}
			return temp;
		}
	}
	else
	{
		tree_node* x = root;
		tree_node* y = root;
		bool flag = true;
		int min;
		min = abs(x->ID-id);
		while(x!=NULL)
		{
			if(x->ID < id)
			{
				x = x->right;
			}
			else
			{
				x = x->left;
			}
			if(x!= NULL && (min > abs(x->ID-id)) && x->ID < id)
			{
				min = abs(x->ID-id);
				y = x;
			}
		}
		if(y->ID < id)
			return y;
		else
			return NULL;
	}
}

/* Function to build initial Red Black Tree */
tree_node* initTree(tree_node* p,int start,int end,int max_ht,int cur_ht)
{
	tree_node* node = new tree_node;
	if(start>end)
	{
		return NULL;
	}
	int mid = (start+end)/2;
	node->ID = init_id[mid];
	node->count = init_cnt[mid];
	if((max_ht-cur_ht)%2==0)
	{
		node->color = RED;
	}
	else
	{
		node->color = BLACK;
	}

	if(start == end)
	{
		return node;
	}

	node->left = initTree(node,start,mid-1,max_ht,cur_ht+1);
	node->right = initTree(node,mid+1,end,max_ht,cur_ht+1);
	if(node->left != NULL)	
		node->left->parent = node;
	if(node->right !=NULL)
		node->right->parent = node;

	return node;
}

/* Function to reduce count of a node */
int reduce(int id,int m)
{
	int ans= 0;
	tree_node* node = search(id);
	if(node!=NULL)
	{
		node->count = node->count-m;
		tree_node* y = new tree_node;
		tree_node* x = new tree_node;
		tree_node* nil = new tree_node;
		nil->parent = nil->left = nil->right = NULL;
		nil->ID = nil->count = 0;
		if(node->count <= 0)
		{
			if((node->left == NULL) || (node->right == NULL))
			{	
				y = node;
			}
			else
			{
				y = next(node->ID);
			}
	
			if(y->left!= NULL)
				x = y->left;
			else
				x = y->right;
			if(x==NULL)
			{
				x = nil;
			}
			x->parent = y->parent;
			if(y->parent == NULL)
			{
				root = x;
			}
			else
			{
				if(y == y->parent->left)
				{
					if(x!=nil)
						y->parent->left = x;
					else
						y->parent->left = NULL;
				}
				else
				{
					if(x!=nil)
						y->parent->right = x;
					else
						y->parent->right = NULL;
				}
			}

			if(y != node)
			{
				node->ID = y->ID;
				node->count = y->count;
			}
			if(y->color == BLACK)
			{
				fixup(x);
			}	
			delete(y);
		
		}
		else
		{
			ans = node->count;
		}
	}
	return ans;
}

/* Function to fix the RB Tree after Deletion */
void fixup(tree_node* node)
{
	tree_node* w = new tree_node;
	while(node!=root && node->color == BLACK)
	{
		if(node == node->parent->left)
		{
			w = node->parent->right;
			if(w->color == RED)
			{
				w->color = BLACK;
				node->parent->color = RED;
				node = leftRotate(node->parent);
				w = node->parent->right;
			}
			if(w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				node = node->parent;
			}
			else
			{
				if(w->right->color == BLACK)
				{
					w->left->color = BLACK;
					w->color = RED;
					node = rightRotate(w);
					w = node->parent->right;
				}
				w->color = node->parent->color;
				node->parent->color = BLACK;
				w->right->color = BLACK;
				node = leftRotate(node->parent);
				node = root;
			}
		}
		else
		{
			w = node->parent->left;
			if(w->color == RED)
			{
				w->color = BLACK;
				node->parent->color = RED;
				node = rightRotate(node->parent);
				w = node->parent->left;
			}
			if(w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				node = node->parent;
			}
			else
			{
				if(w->left->color == BLACK)
				{
					w->right->color = BLACK;
					w->color = RED;
					node = leftRotate(w);
					w = node->parent->left;
				}
				w->color = node->parent->color;
				node->parent->color = BLACK;
				w->left->color = BLACK;
				node = rightRotate(node->parent);
				node = root;
			}
		}
	}
	node->color = BLACK;
}





