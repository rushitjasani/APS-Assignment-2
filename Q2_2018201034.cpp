// @auther : Rushit Jasani
// problem : B Tree in C++
#include <bits/stdc++.h>
using namespace std;

#define DEGREE 3
#define MX (DEGREE - 1)
#define MN ((MX) / 2)

/*  structure of node [ btreeNode ] */
typedef struct node_tree
{
    long long size;
    long long val[MX + 1];
    struct node_tree *link[MX + 1];
} btreeNode;

btreeNode *root;

void btree_insert(long long);
bool findNodeToInsert(long long, long long *, btreeNode *, btreeNode **);
btreeNode *createNewNode(long long, btreeNode *);
void split(long long, long long *, long long, btreeNode *, btreeNode *, btreeNode **);
void addVal(long long, long long, btreeNode *, btreeNode *);
void ino_suc(btreeNode *, long long);
void rm_val(btreeNode *, long long);
void do_rsh(btreeNode *, long long);
void do_lsh(btreeNode *, long long);
void mergeNodes(btreeNode *, long long);
void adjst_node(btreeNode *, long long);
bool findNodeToDelete(long long, btreeNode *myNode);
void del_node(long long, btreeNode *);
void btree_search(long long, btreeNode *);
void inOrder(btreeNode *);

int main()
{
    long long value, choice;
    while (true)
    {
        cout << "\n1.INS\t2.SEARCH\t3.DELETE\n";
        cin >> choice;
        switch (choice)
        {
        case 1:
            cin >> value;
            btree_insert(value);
            break;
        case 2:
            cin >> value;
            btree_search(value, root);
            break;
        case 3:
            cin >> value;
            del_node(value, root);
            break;
        default:
            cout << "Wrong Opt\n";
            break;
        }
        inOrder(root);
    }
}

/* insert 'value' in tree */
void btree_insert(long long value)
{
    bool hv_to_crt_new_node;
    btreeNode *child; //if node is splitted in process, child will point to that new node.
    long long i;      //if node is splitted, then the value which needs to go in parent will be stored in i.
    //set value to apropriate node. if node is splitted
    //and their parent need to be create then it will return true.
    hv_to_crt_new_node = findNodeToInsert(value, &i, root, &child);
    //if true then create new node with gnovalue i, and child as child.
    if (hv_to_crt_new_node)
        root = createNewNode(i, child);
}

/* Creating New Node */
btreeNode *createNewNode(long long value, btreeNode *child)
{
    btreeNode *newNode;
    newNode = (btreeNode *)malloc(sizeof(btreeNode));
    newNode->val[1] = value;
    newNode->size = 1;
    newNode->link[0] = root;
    newNode->link[1] = child;
    return newNode;
}

bool findNodeToInsert(long long val, long long *pval, btreeNode *node, btreeNode **link)
{
    long long select_link;
    if (!node) // if reach to leaf node, create simple value and child pair for future use.
    {
        *pval = val;
        *link = NULL;
        return true;
    }
    if (val < node->val[1]) //if value is less than first value of node, then select left child of that node.
        select_link = 0;
    else
    { //traverse to right link to go more deeper in tree.
        for (select_link = node->size; (val < node->val[select_link] && select_link > 1); select_link--)
            ;
        if (val == node->val[select_link]) //if same value found, return directly.
            return false;
    }
    if (findNodeToInsert(val, pval, node->link[select_link], link))
    { //bottom up call after reached correct positio.
        //check in recently modified child that if node is of size < MX or not..if yes then add that value in node. else split node.
        if (node->size < MX)
            addVal(*pval, select_link, node, *link);
        else
        {
            split(*pval, pval, select_link, node, *link, link);
            return true;
        }
    }
    return false;
}

/* Places the value in appropriate position */
void addVal(long long val, long long pos, btreeNode *node, btreeNode *child)
{
    //val = value to be inserted
    //pos = it is prev index from where we want to insert our node.
    //*node = poiner to the node in which we are inserting.
    //*child = pointer to child of newly created node.
    long long j = node->size;
    while (j > pos)
    { //shift all values to one step right to make space at pos.
        node->val[j + 1] = node->val[j];
        node->link[j + 1] = node->link[j];
        j--;
    }
    //insert new values at pos+1
    node->val[j + 1] = val;
    node->link[j + 1] = child;
    node->size++; //incrementing size of that node.
}

/* Split nodes in two node. */
void split(long long val, long long *pval, long long pos, btreeNode *node, btreeNode *child, btreeNode **newNode)
{
    long long mid, j;
    //mid : starting position from which we shift values to newly created child..
    if (pos > MN)
        mid = MN + 1;
    else
        mid = MN;

    *newNode = (btreeNode *)malloc(sizeof(btreeNode)); // creating new node.
    j = mid + 1;
    while (j <= MX)
    {
        //insert in new node from position 0..
        (*newNode)->val[j - mid] = node->val[j];
        (*newNode)->link[j - mid] = node->link[j];
        j++;
    }
    //change size of both node to apropriate value.
    node->size = mid;
    (*newNode)->size = MX - mid;

    if (pos <= MN) //adding data to old node.
        addVal(val, pos, node, child);
    else //adding data to newly created node.
        addVal(val, pos - mid, *newNode, child);
    *pval = node->val[node->size];                //set the new root value in pval.
    (*newNode)->link[0] = node->link[node->size]; // set Right link of value which is going to be root, to left of new node.
    node->size--;                                 //decrease old nodes size coz last value is used in becoming root.
    return;
}

/* copy successor for the value to be deleted */
void ino_suc(btreeNode *myNode, long long pos)
{
    btreeNode *tmp;
    tmp = myNode->link[pos]; // goes to right child of current node.
    for (; tmp->link[0] != NULL;)
        tmp = tmp->link[0];         //go to the left most child of the tmp.
    myNode->val[pos] = tmp->val[1]; //copy successors value to the current node.
    return;
}

/* removes the value from the given node (leaf node) and rearrange other values */
void rm_val(btreeNode *myNode, long long pos)
{ //left shift all values from pos+1 to end.
    long long i = pos + 1;
    while (i <= myNode->size)
    {
        myNode->val[i - 1] = myNode->val[i];
        myNode->link[i - 1] = myNode->link[i];
        i++;
    }
    myNode->size--;
}

/* shifts value from parent to right child */
void do_rsh(btreeNode *myNode, long long pos)
{
    btreeNode *x = myNode->link[pos];
    long long j = x->size;

    while (j > 0)
    {
        x->val[j + 1] = x->val[j];
        x->link[j + 1] = x->link[j];
    }
    x->val[1] = myNode->val[pos];
    x->link[1] = x->link[0];
    x->size++;

    x = myNode->link[pos - 1];
    myNode->val[pos] = x->val[x->size];
    myNode->link[pos] = x->link[x->size];
    x->size--;
    return;
}

/* shifts value from parent to left child */
void do_lsh(btreeNode *myNode, long long pos)
{
    long long j = 1;
    btreeNode *x = myNode->link[pos - 1];

    x->size++;
    x->val[x->size] = myNode->val[pos];
    x->link[x->size] = myNode->link[pos]->link[0];

    x = myNode->link[pos];
    myNode->val[pos] = x->val[1];
    x->link[0] = x->link[1];
    x->size--;

    while (j <= x->size)
    {
        x->val[j] = x->val[j + 1];
        x->link[j] = x->link[j + 1];
        j++;
    }
    return;
}

/* merge nodes */
void mergeNodes(btreeNode *myNode, long long pos)
{
    long long j = 1;
    btreeNode *rt_child = myNode->link[pos], *left_child = myNode->link[pos - 1];

    left_child->size++;
    left_child->val[left_child->size] = myNode->val[pos];

    while (j <= rt_child->size)
    { //taking data from right child if any data exist and putting in left child.
        left_child->size++;
        left_child->val[left_child->size] = rt_child->val[j];
        left_child->link[left_child->size] = rt_child->link[j];
        j++;
    }
    j = pos;
    while (j < myNode->size)
    { //Left shifting in parent node after sending parent_data to left child.
        myNode->val[j] = myNode->val[j + 1];
        myNode->link[j] = myNode->link[j + 1];
        j++;
    }
    myNode->size--; //reducing size of parent node.
    free(rt_child);
}

/* adjusts the given node */
void adjst_node(btreeNode *myNode, long long pos)
{
    if (pos == 0)
    {                                   //if left mdst link then check only in right sibling.
        if (myNode->link[1]->size > MN) //right sibling has sufficient data, then perform shift
            do_lsh(myNode, 1);
        else
            mergeNodes(myNode, 1); //if right sibling has not sufficient data then merge.
    }
    else
    {
        if (myNode->size != pos) //if intermediate node then check left sibling then right sibling and if not possible then do merge.
        {
            if (myNode->link[pos - 1]->size > MN)
                do_rsh(myNode, pos);
            else
            {
                if (myNode->link[pos + 1]->size > MN)
                    do_lsh(myNode, pos + 1);
                else
                    mergeNodes(myNode, pos);
            }
        }
        else //if rightmost child then check only left sibling
        {
            if (myNode->link[pos - 1]->size > MN) //left sibling has sufficient data, then perform shift
                do_rsh(myNode, pos);
            else
                mergeNodes(myNode, pos); //if left sibling has not sufficient data then merge.
        }
    }
}

/* delete val from the node */
bool findNodeToDelete(long long val, btreeNode *myNode)
{
    long long pos;
    bool flag = false;
    if (myNode)
    {
        if (val < myNode->val[1])
        {
            pos = 0;
            flag = false;
        }
        else
        {
            for (pos = myNode->size; (val < myNode->val[pos] && pos > 1); pos--)
                ;
            if (val == myNode->val[pos])
                flag = true;
            else
                flag = false;
        }
        if (flag)
        {
            if (myNode->link[pos - 1]) //check internal node or leaf node.
            {                          //Internal
                ino_suc(myNode, pos);
                flag = findNodeToDelete(myNode->val[pos], myNode->link[pos]);
                if (flag == false)
                    cout << "Data not present" << endl;
            }
            else //LEAF
                rm_val(myNode, pos);
        }
        else
            flag = findNodeToDelete(val, myNode->link[pos]);
        if (myNode->link[pos]) //checking if its child is exist or not. and if exist, checking for its count for adjusting.
            if (myNode->link[pos]->size < MN)
                adjst_node(myNode, pos);
    }
    return flag;
}

/* delete val from B-tree */
void del_node(long long val, btreeNode *myNode)
{
    btreeNode *tmp;
    if (!findNodeToDelete(val, myNode))
    {
        cout << "value not present" << endl;
        return;
    }
    else
    {
        if (myNode->size == 0)
        { //if count of root == 0 then adjust.
            tmp = myNode;
            myNode = myNode->link[0];
            free(tmp);
        }
    }
    root = myNode;
    return;
}

/* Searching if given value is prensent in tree or not */
void btree_search(long long val, btreeNode *myNode)
{
    long long pos;
    if (!myNode)
        return;
    if (val < myNode->val[1])
        pos = 0;
    else
    {
        for (pos = myNode->size; (val < myNode->val[pos] && pos > 1); (pos)--)
            ;
        if (val == myNode->val[pos])
        {
            cout << val << " is present in B-Tree" << endl;
            return;
        }
    }
    btree_search(val, myNode->link[pos]);
    return;
}

void inOrder(btreeNode *myNode)
{
    int i;
    if (myNode)
    {
        for (i = 0; i < myNode->size; i++)
        {
            inOrder(myNode->link[i]);
            cout << myNode->val[i + 1] << " ";
        }
        inOrder(myNode->link[i]);
    }
}