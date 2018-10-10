// @auther : Rushit Jasani
// problem : B Tree in C++
#include <bits/stdc++.h>
using namespace std;

#define DEGREE 4
#define MX (DEGREE - 1)
#define MN ((MX) / 2)

typedef struct node_tree
{
    long long size;
    long long val[MX + 1];
    struct node_tree *link[MX + 1];
} btreeNode;

btreeNode *root;
btreeNode *createNewNode(long long, btreeNode *);
void btree_insert(long long);
bool findNodeToInsert(long long, long long *, btreeNode *, btreeNode **);
void split(long long, long long *, long long, btreeNode *, btreeNode *, btreeNode **);
void addVal(long long, long long, btreeNode *, btreeNode *);
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
        // cout << "\n1.INS\n2.SEARCH\n3.DELETE\n";
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
        cout << endl;
    }
}

void btree_insert(long long value)
{
    btreeNode *child;
    long long i;

    if (findNodeToInsert(value, &i, root, &child))
        root = createNewNode(i, child);
}

btreeNode *createNewNode(long long data, btreeNode *child)
{
    btreeNode *newNode = new btreeNode;
    newNode->size = 1;
    newNode->val[1] = data;
    newNode->link[0] = root;
    newNode->link[1] = child;
    
    return newNode;
}

bool findNodeToInsert(long long v, long long *p_v, btreeNode *node, btreeNode **link)
{
    if (node == NULL)
    {
        *p_v = v;
        *link = NULL;
        return true;
    }
    long long child;
    if (v < node->val[1])
        child = 0;
    else
    {
        child = node->size;
        while (v < node->val[child] && child > 1)
            child--;
        if (v == node->val[child])
            return false;
    }
    if (findNodeToInsert(v, p_v, node->link[child], link))
    {
        if ((node->size) < MX)
            addVal(*p_v, child, node, *link);
        else
        {
            split(*p_v, p_v, child, node, *link, link);
            return true;
        }
    }
    return false;
}

void addVal(long long val, long long pos, btreeNode *node, btreeNode *child)
{
    long long j = node->size;
    while (j > pos)
    {
        node->val[j + 1] = node->val[j];
        node->link[j + 1] = node->link[j];
        j--;
    }
    node->val[j + 1] = val;
    node->link[j + 1] = child;
    node->size++;
}

void split(long long val, long long *pval, long long pos, btreeNode *node, btreeNode *child, btreeNode **newNode)
{
    long long mid, j;
    if (pos > MN)
        mid = MN + 1;
    else
        mid = MN;

    *newNode = (btreeNode *)malloc(sizeof(btreeNode));
    j = mid + 1;
    while (j <= MX)
    {
        (*newNode)->val[j - mid] = node->val[j];
        (*newNode)->link[j - mid] = node->link[j];
        j++;
    }
    node->size = mid;
    (*newNode)->size = MX - mid;

    if (pos <= MN)
        addVal(val, pos, node, child);
    else
        addVal(val, pos - mid, *newNode, child);
    *pval = node->val[node->size];
    (*newNode)->link[0] = node->link[node->size];
    node->size--;
    return;
}

void rm_val(btreeNode *myNode, long long pos)
{
    long long i = pos + 1;
    while (i <= myNode->size)
    {
        myNode->val[i - 1] = myNode->val[i];
        myNode->link[i - 1] = myNode->link[i];
        i++;
    }
    myNode->size--;
}

void do_rsh(btreeNode *myNode, long long pos)
{
    btreeNode *x = myNode->link[pos];
    long long j = x->size;

    while (j > 0)
    {
        x->val[j + 1] = x->val[j];
        x->link[j + 1] = x->link[j];
        j--;
    }
    x->val[1] = myNode->val[pos];
    x->link[1] = x->link[0];
    x->size++;

    x = myNode->link[pos - 1];
    myNode->val[pos] = x->val[x->size];
    x->link[0] = myNode->link[pos - 1]->link[x->size];
    x->size--;
    return;
}

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

void mergeNodes(btreeNode *myNode, long long pos)
{
    long long j = 1;
    btreeNode *rt_child = myNode->link[pos];
    btreeNode *left_child = myNode->link[pos - 1];

    left_child->size++;
    left_child->val[left_child->size] = myNode->val[pos];

    while (j <= rt_child->size)
    {
        left_child->size++;
        left_child->val[left_child->size] = rt_child->val[j];
        left_child->link[left_child->size] = rt_child->link[j];
        j++;
    }
    j = pos;
    while (j < myNode->size)
    {
        myNode->val[j] = myNode->val[j + 1];
        myNode->link[j] = myNode->link[j + 1];
        j++;
    }
    myNode->size--;
    free(rt_child);
}

void adjst_node(btreeNode *myNode, long long pos)
{
    if (pos == 0)
    {
        if (myNode->link[1]->size > MN)
            do_lsh(myNode, 1);
        else
            mergeNodes(myNode, 1);
    }
    else
    {
        if (myNode->size != pos)
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
        else
        {
            if (myNode->link[pos - 1]->size > MN)
                do_rsh(myNode, pos);
            else
                mergeNodes(myNode, pos);
        }
    }
}

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
            pos = myNode->size;
            while (val < myNode->val[pos] && pos > 1)
                pos--;
            if (val == myNode->val[pos])
                flag = true;
            else
                flag = false;
        }
        if (flag)
        {
            if (myNode->link[pos])
            {
                btreeNode *tmp = myNode->link[pos];
                while (tmp->link[0])
                    tmp = tmp->link[0];
                myNode->val[pos] = tmp->val[1];
                flag = findNodeToDelete(myNode->val[pos], myNode->link[pos]);
                if (flag == false)
                    cout << "Data not present" << endl;
            }
            else
                rm_val(myNode, pos);
        }
        else
            flag = findNodeToDelete(val, myNode->link[pos]);
        if (myNode->link[pos])
            if (myNode->link[pos]->size < MN)
                adjst_node(myNode, pos);
    }
    return flag;
}

void del_node(long long val, btreeNode *myNode)
{
    btreeNode *tmp;
    if (!findNodeToDelete(val, myNode))
    {
        cout << "Not Found" << endl;
        return;
    }
    else
    {
        if (myNode->size == 0)
        {
            tmp = myNode;
            myNode = myNode->link[0];
            free(tmp);
        }
    }
    root = myNode;
    return;
}

void btree_search(long long val, btreeNode *myNode)
{

    if (myNode == NULL)
        return;
    long long pos;
    if (val < (myNode->val[1]))
        pos = 0;
    else
    {
        pos = myNode->size;
        while (val < myNode->val[pos] && pos > 1)
            pos--;
        if (val == myNode->val[pos])
        {
            cout << val << " is present" << endl;
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
    return;
}