// @auther : Rushit Jasani
// problem : implementation of stringBuilder in c++
#include <bits/stdc++.h>
using namespace std;
#define ll long long

typedef struct node
{
    const char *data;
    struct node *next;
} ll_node;

class stringBuilder
{
  public:
    ll total_size;
    ll_node *head;
    stringBuilder(const char *str)
    {
        ll_node *new_node = new ll_node;
        new_node->data = str;
        new_node->next = NULL;
        head = new_node;
        total_size = strlen(head->data);
    }
    stringBuilder()
    {
        total_size = 0;
        head = NULL;
    }
};

stringBuilder stringInitialize(const char *str)
{
    return stringBuilder(str);
}

stringBuilder stringAppend(stringBuilder x, stringBuilder y)
{
    stringBuilder s3;
    ll_node *tmp_head = x.head;
    ll_node *tmp = tmp_head, *prev;
    if (x.head != NULL)
    {
        ll_node *new_node = new ll_node;
        new_node->data = tmp->data;
        new_node->next = NULL;
        prev = tmp;
        tmp = tmp->next;
        while (tmp != NULL)
        {
            ll_node *n_1 = new ll_node;
            n_1->data = tmp->data;
            n_1->next = NULL;
            prev->next = n_1;
            prev = n_1;
            tmp = tmp->next;
        }
        tmp = y.head;
    }
    else if (y.head != NULL)
    {
        tmp_head = y.head;
        tmp = tmp_head;
        ll_node *new_node = new ll_node;
        new_node->data = tmp->data;
        new_node->next = NULL;
        prev = tmp;
        tmp = tmp->next;
    }
    while (tmp != NULL)
    {
        ll_node *n_1 = new ll_node;
        n_1->data = tmp->data;
        n_1->next = NULL;
        prev->next = n_1;
        prev = n_1;
        tmp = tmp->next;
    }
    s3.head = tmp_head;
    s3.total_size = x.total_size + y.total_size;
    return s3;
}

void print(stringBuilder s)
{

    ll_node *tmp = s.head;
    while (tmp != NULL)
    {
        cout << "=>[" << tmp->data << "]";
        tmp = tmp->next;
    }
    cout << endl;
}

ll findSubstring_kmp(const char *s, const char *pattern)
{
    ll str_len = strlen(s);         //length of string
    ll pat_len = strlen(pattern);   //length of pattern
    //handleing edge casaes.
    if (pat_len == 0)
        return -1;
    if (pat_len > str_len)
        return -1;

    vector<ll> partial_match_table(pat_len + 1, 0);
    for (ll i = 1; i < pat_len; i++)
    {
        ll j = partial_match_table[i + 1];
        while (j > 0 && pattern[i] != pattern[j])
            j = partial_match_table[j];
        if (j > 0 || pattern[i] == pattern[j])
            partial_match_table[i + 1] = j + 1;
    }
    ll i = 0, j = 0;
    for (; i < str_len; i++)
    {
        if (s[i] == pattern[j])
        {
            if (++j == pat_len)
                return (i - j + 1);
        }
        else if (j > 0)
        {
            j = partial_match_table[j];
            i--;
        }
    }
    return -1;
}

ll findSubstring(stringBuilder &s, const char *pattern)
{
    if (s.head == NULL)
        return -1;
    if (s.head->next != NULL)
    {
        ll newSize = s.total_size;
        char *str = new char[newSize];

        ll_node *prevhead = s.head;

        ll_node *newNode = new ll_node;
        newNode->next = NULL;
        s.head = newNode;

        strcpy(str, prevhead->data);

        ll_node *prev = prevhead;
        prevhead = prevhead->next;
        free(prev);
        while (prevhead != NULL)
        {
            prev = prevhead;
            strcat(str, prevhead->data);
            prevhead = prevhead->next;
            free(prev);
        }
        newNode->data = str;
    }
    return findSubstring_kmp(s.head->data, pattern);
}

int main()
{
    stringBuilder s1 = stringInitialize("abca");
    print(s1);
    stringBuilder s2 = stringInitialize("abab");
    print(s2);
    stringBuilder s3 = stringInitialize("rushit");
    print(s3);
    stringBuilder s4 = stringInitialize("darshan");
    print(s4);
    stringBuilder s5 = stringAppend(s1, s2);
    print(s5);
    stringBuilder s6, s7;
    s1 = stringAppend(s6, s7);
    print(s1);
    s2 = stringAppend(s1, s2);
    print(s2);
    s2 = stringAppend(s2, s5);
    print(s2);
    ll x = findSubstring(s2, "aabab");
    print(s2);
    cout << x << endl;
    ll y = findSubstring(s2, "orla");
    print(s2);
    cout << y << endl;
    return 0;
}