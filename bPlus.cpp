#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
#define pb push_back
#define mp make_pair
#define f first
#define s second
#define sc(n) scanf("%d",&n)
#define scl(n) scanf("%lld",&n)
#define pr(n) printf("%d",n)
#define prl(n) printf("%lld",n)
#define nl printf("\n")
#define fast_io ios_base::sync_with_stdio(false);cin.tie(NULL)

const int ninf = -1000000001;
const int pinf = 1000000001;
// output file
ofstream out_file("output.txt");

int B, M;

typedef struct Node{
	// To remove a node from tree
	bool isDead;
	// To check if the current is leaf or not
	bool isLeaf;
	// contains keys which are there in the current node
	vector<int> keys;
	// In case of a node split buffer contains the new node added
	Node* buffer;
	// pointer to children nodes of current node
	vector<Node*> pt;
	// pointer to parent nodes of current node
	Node* parent;
}Node;

typedef Node* pointerToNode;

Node* createNode(int n, bool check)
{
	pointerToNode node = new Node;
	node->isLeaf = check;
	node->isDead = false;
	node->keys = vector<int>(n, pinf);
	node->buffer = NULL;
	node->pt = vector<Node*>(n+1);
	node->parent = NULL;
	return node;
}

void reviveNode(pointerToNode parent, int value ,pointerToNode child)
{
	if(parent != NULL)
	{
		int i = 1,f = 0,pars = (parent->pt).size();
		while(i < pars)
		{
			if(parent->pt[i] == child)
			{
				f = 1;
				parent->keys[i - 1] = value;
			}
       		i++;
		}
		if(f && parent->isDead)
			reviveNode(parent->parent, value, parent);
	}
}

int count(pointerToNode leaf, int value)
{
	int i,cnt = 0,f = 0,last_index = (leaf->pt).size() - 1; 
	while(leaf != NULL)
	{
		for(i = 0;i < (leaf->keys).size();i++)
		{
			if(leaf->keys[i] == value)
				cnt++;
			else if(leaf->keys[i] > value && leaf->keys[i] != pinf)
				{
					f = 1;
					break;
				}
		}
		if(f)
			break;
		leaf = leaf->pt[last_index];
	}
	return cnt;
}

int range(pointerToNode leaf, int lowerb, int upperb)
{
	int i,cnt = 0, f = 0, last_index = (leaf->pt).size() - 1;
	while(leaf != NULL)
	{
		for(i = 0;i < (leaf->keys).size();i++)
		{
			if((leaf->keys[i] <= upperb) && (leaf->keys[i] >= lowerb))
				cnt++;
			else if((leaf->keys[i] > upperb) && (leaf->keys[i] != pinf))
			   {
			   		f = 1;
			   		break;
			   }
		}
		if(f)
			break;
		leaf = leaf->pt[last_index];
	}
	return cnt;
}

// When a new root is made in the tree after insertion, the following functions returns new root. Otherwise returns NULL.
Node* insertBtree(pointerToNode node, int value)
{
	pointerToNode root = NULL;
 	bool split = true;
	if(node->keys[(node->keys).size() - 1] == pinf)
		split = false;
	// To get number of keys on a node.
	int node_size = (node->keys).size();
	// Case where splitting is not required
	// In this case, only need to insert node in leaf. No need to update parent nodes in any way.
	// If the element to be inserted is smallest of the current node, then needs to be inserted at 0th index.
	// Otherwise inserted appropriately.
	if(!split)
	{
		// Pointer also needs to be inserted if node is in between.
		bool inserting = false;
		int tempkey = pinf,i=0,j;
		pointerToNode tempt = NULL;
		while(i < node_size)
		{
			if(inserting)
			{
				swap(node->keys[i], tempkey);
				if(!node->isLeaf)
				{
					j = i+1;
					swap(node->pt[j], tempt);
				}
				i++;
			}
			else
			{
				if((node->keys[i] == pinf) || (value < node->keys[i]))
				{
					if(!node->isLeaf)
					{
						tempt = node->pt[i + 1];
						node->pt[i + 1] = node->buffer;
					}
					tempkey = node->keys[i];
					node->keys[i] = value;
					inserting = true;
				}
				if((node->isDead) && (value != node->keys[0]))
				{
					node->isDead = false;
					reviveNode(node->parent, value, node);
				}
				i++;
			}
      	}
	}
	else
	{
		int i, upperb, newVal;
		vector<int> tempkey = node->keys;
		vector<Node*> tempt = node->pt;

		// Finding index to insert the new value
		vector<int>::iterator it = upper_bound((tempkey).begin(), (tempkey).end(), value);
		tempkey.insert(it, value);
		int tempIndex = it - tempkey.begin();

		pointerToNode newNode = createNode(node_size, node->isLeaf);
		if(!node->isLeaf)
			tempt.insert(tempt.begin() + tempIndex + 1, node->buffer);
		// Adding parent to new node. Will be same as original node parent.
		newNode->parent = node->parent;
		if(node->isLeaf)
		{
			// Adjacent leaf nodes are connected
			newNode->pt[node_size] = node->pt[node_size];
			node->pt[node_size] = newNode;

			// Calculating bound on number of pointers, a leaf node have after split
			upperb = ceil(double(node_size + 1.0)/2);
		}
		else
		{
			// Non Leaf case
			// Calculating bound on number of pointers, a leaf node have after split
			upperb = ceil(double(node_size + 2.0)/2);

			// Pointers redistributed, as new node comes in
      		int len = (tempt).size();
			for (i = 0;i < len; i++)
			{
				if(i < upperb)
					node->pt[i] = tempt[i];
				else
				{
					newNode->pt[i - upperb] = tempt[i];
					// All parent pointers of child node updated to newNode
					newNode->pt[i - upperb]->parent = newNode;
					if(i <= node_size)
						node->pt[i] = NULL;
				}
			}

			// A key is deleted and passed to the node above, when insertion goes to non-leaf level
			newVal = tempkey[upperb - 1];
			tempkey.erase(tempkey.begin() + upperb - 1);
			upperb--;
		}
		// newNode keys are set using below loop
		for (i = 0;i < (tempkey).size(); i++)
		{
			if(i < upperb)
				node->keys[i] = tempkey[i];
			else
			{
				newNode->keys[i - upperb] = tempkey[i];
				// A split is made, shifted keys are removed, as they become empty
				if(i < node_size)
				{
					// node->pt[i]->isDead = true;
					node->keys[i] = pinf;
				}
			}
		}

		// When value inserted in the dead node then update the dead flag to false.
 		if((value != node->keys[0]) && (node->isDead) && (tempIndex < upperb))
		{
			node->isDead = false;
			reviveNode(node->parent, value, node);
		}

		// Last key of node is node->keys[ubp - 1] is largest key, tempIndex will have index of smallest newest key of newNode
		it = upper_bound((newNode->keys).begin(), (newNode->keys).end(), node->keys[upperb - 1]);
		tempIndex = it - newNode->keys.begin();
		if(node->isLeaf)
			newVal = newNode->keys[tempIndex];
		else if(newNode->keys[tempIndex] == pinf)
		{
			// All the values are same as 1000000001 or pinf, then parent needs to be updated to deadKey
			newVal = newNode->keys[0];
			newNode->isDead = true;
		}


		// Updating parent about the insertion
		// Case when there is no parent, new root is created
		if(node->parent == NULL)
		{
			root = createNode(node_size, false);
			node->parent = root;
			newNode->parent = root;
			root->pt[0] = node;
			root->pt[1] = newNode;
			root->keys[0] = newVal;
		}
		else
		{
			// Function of buffer is mentioned above in declaration 
			// It basically contains new node to be added to parent.
			// Since there are chances that all pointers are assigned to some node or the other, it needs to be maintained
			node->parent->buffer = newNode;
			root = insertBtree(node->parent, newVal);
		}
	}
	return root;
}

Node* lookup(pointerToNode node, int value, bool up)
{
	while(!(node->isLeaf))
	{
		int i = 0,lowerb = ninf,upperb = pinf;
		int node_size = (node->keys).size(),ind;
		for(;i < node_size;i++)
		{
      		ind = i;
      		upperb = node->keys[i];
			if(upperb == pinf)
				break;
			if(lowerb <= value && value < upperb)
				break;
			else if(lowerb <= value && value == upperb && !up && node->pt[i + 1]->isDead)
				break;
			else
				ind += 1;
			lowerb = upperb;
		}
		node = node->pt[ind];
	}
	return node;
}

Node* insert(pointerToNode node, int value)
{
	pointerToNode temp = insertBtree(lookup(node, value, true), value);
	if(temp != NULL)
		node = temp;
	return node;
}

bool find(pointerToNode leaf, int value)
{
  	int i;
	for(i = 0;i < (leaf->keys).size();i++)
		if(leaf->keys[i] == value)
			return true;
	return false;
}

void fileRead(int n, string fileName)
{
	string line;
	pointerToNode root = createNode(n, true);
	vector<string> ins,ans;
	ifstream infile(fileName.c_str());
	int i, value1, value2;
	int num = 0, bound = min(1,(M-1)*B/10);
	// Reading the file to get the commands and then giving the required output in file
	while(!infile.eof())
	{
		ins.clear();
		num = 0;
		while(num < bound && getline(infile, line))
		{
			ins.pb(line);
			num++;
		}
		for(i = 0;i < num;i++)
		{
			line = ins[i];
			if(line.find("INSERT") != string::npos)
			{
				istringstream(line.substr(7,(line).size()-7)) >> value1;
	  			root = insert(root, value1);
			}
			else if(line.find("RANGE") != string::npos)
			{
				istringstream(line.substr(6,(line).size()-6)) >> value1 >> value2;
				out_file<<range(lookup(root, value1, false), value1, value2)<<endl;
			}
			else if(line.find("COUNT") != string::npos)
			{
				istringstream(line.substr(6,(line).size()-6)) >> value1;
				out_file<<count(lookup(root, value1, false), value1)<<endl;
			}
			else if(line.find("FIND") != string::npos)
			{
				istringstream(line.substr(5,(line).size()-5)) >> value1;
				if(find(lookup(root, value1, false), value1))
					out_file<<"YES"<<endl;
				else
					out_file<<"NO"<<endl;
			}
			else
				out_file<<"Invalid command: "<<line<<endl;
		}
	}
	return;
}

int main(int argc, char const *argv[])
{
	fast_io;
	if(argc!=4)
	{
		cout<<"Incomplete arguments"<<endl;
		cout<<"Format should be [File Name] M B"<<endl;
		return 0;
	}
	M = atoi(argv[2]);
	B = atoi(argv[3]);
	string filename;
	filename = argv[1];
	// Degree of tree node is denoted by n, depends on value of B.
	int n = (B - 8)/12;
	n = (n < 2)? 2:n;
	B = (B <= 20)? 20:B;
	fileRead(n, filename);
	return 0;
}
