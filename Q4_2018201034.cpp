// @auther : Rushit Jasani
// problem : implementation of unordered set
#include <bits/stdc++.h>
using namespace std;
#define BKT 5

template <class T, class U>
class Unordered_map
{
  public:
    long long prime_num = 31;
    long long bucket = BKT;
    class node
    {
      public:
        T key;
        U value;
        node *next;
    };
    node *my_map[BKT];

    Unordered_map()
    {
        for (long long i = 0; i < BKT; i++)
            my_map[i] = NULL;
    }

    long long calculateHash(T key)
    {
        ostringstream s1;
        s1 << key;
        string s = s1.str();
        long long hash_val = 0;
        long long mul = 1;
        for (long long i = 0; i < s.size(); i++)
        {
            hash_val += (s[i] * mul);
            hash_val %= bucket;
            mul *= prime_num;
        }
        return hash_val;
    }

    node *getNode(T key, U value)
    {
        node *x = new node;
        x->key = key;
        x->value = value;
        x->next = NULL;
        return x;
    }

    void insert(T key, U value)
    {
        long long index = calculateHash(key);
        node *head = my_map[index];
        if (head == NULL)
        {
            node *newNode = getNode(key, value);
            my_map[index] = newNode;
            return;
        }
        else if (head->next == NULL)
        {
            if (head->key == key)
                return;
            else
            {
                node *newNode = getNode(key, value);
                head->next = newNode;
                return;
            }
        }
        else
        {
            node *tmp = head;
            while (tmp->next != NULL)
            {
                if (tmp->key == key)
                {
                    return;
                }
                else
                {
                    tmp = tmp->next;
                }
            }
            if (tmp->key == key)
            {
                return;
            }
            node *newNode = getNode(key, value);
            tmp->next = newNode;
            return;
        }
        return;
    }

    pair<bool, U> search(T key)
    {
        long long index = calculateHash(key);
        node *head = my_map[index];
        pair<bool, U> ans;
        if (head == NULL)
        {
            ans.first = false;
            return ans;
        }
        else
        {
            node *tmp = head;
            while (tmp != NULL)
            {
                if (tmp->key == key)
                {
                    ans = {true, tmp->value};
                    return ans;
                }
                else
                {
                    tmp = tmp->next;
                }
            }
            ans.first = false;
            return ans;
        }
    }

    void delete_key(T key)
    {
        long long index = calculateHash(key);
        node *head = my_map[index];
        node *tmp = head, *prev = NULL;
        if (head == NULL)
        {
            cout << "No Data Found Related with key." << endl;
            return;
        }
        else
        {
            if (head->key == key)
            {
                tmp = head;
                head = head->next;
                my_map[index] = head;
                free(tmp);
                return;
            }
            while (tmp->next != NULL && tmp->key != key)
            {
                prev = tmp;
                tmp = tmp->next;
            }
            if (tmp->next == NULL && tmp->key != key)
            {
                cout << "No Data Found Related with key." << endl;
                return;
            }
            else
            {
                prev->next = tmp->next;
                free(tmp);
            }
        }
        return;
    }

    void print_all_LL()
    {
        for (long long i = 0; i < BKT; i++)
        {
            cout << endl
                 << i << " : ";
            node *st = my_map[i];
            while (st != NULL)
            {
                cout << "=>[" << st->key << "=" << st->value << "]";
                st = st->next;
            }
            cout << endl;
        }
    }
};

int main()
{
    ios_base::sync_with_stdio(false);
    int s;
    long long val;
    Unordered_map<int, int> x;
    while (1)
    {
        int c;
        cin >> c;
        switch (c)
        {
        case 1:
        {
            cin >> s;
            cin >> val;
            x.insert(s, val);
            break;
        }
        case 2:
        {
            cin >> s;
            pair<bool, int> ans = x.search(s);
            if (ans.first == true)
                cout << ans.second << endl;
            else
                cout << "Not Found" << endl;
            break;
        }
        case 3:
        {
            cin >> s;
            x.delete_key(s);
            break;
        }
        default:
        {
            fflush(0);
            continue;
        }
        }
        // x.print_all_LL();
        cout << endl;
    }
    return 0;
}