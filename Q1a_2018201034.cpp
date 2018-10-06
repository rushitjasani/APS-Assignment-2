#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define vll vector<ll>
#define pll pair<ll, ll>

int cmp_func(pair<ll, pll> a, pair<ll, pll> b)
{
    if (a.second.first == b.second.first)
        return (a.second.second < b.second.second);
    else
        return (a.second.first < b.second.first);
}

vll create_sfx_arr(string s)
{
    ll n = s.size();
    vector<pair<ll, pll>> sfx_val(n);
    vll index(n);
    for (ll i = 0; i < n; i++)
    {
        ll rank = s[i] - '0';
        ll next_rank = -1;
        if (i + 1 < n)
            next_rank = s[i + 1] - '0';
        sfx_val[i] = {i, {rank, next_rank}};
    }

    sort(sfx_val.begin(), sfx_val.end(), cmp_func);

    for (ll k = 4; k < (n << 1); k <<= 1)
    {

        ll r = 0;
        ll p_r = sfx_val[0].second.first;
        sfx_val[0].second.first = r;
        index[sfx_val[0].first] = 0;

        for (ll i = 1; i < n; i++)
        {
            if (sfx_val[i].second.first == p_r &&
                sfx_val[i].second.second == sfx_val[i - 1].second.second)
            {
                p_r = sfx_val[i].second.first;
                sfx_val[i].second.first = r;
            }
            else
            {
                p_r = sfx_val[i].second.first;
                sfx_val[i].second.first = ++r;
            }
            index[sfx_val[0].first] = i;
        }

        for (ll i = 0; i < n; i++)
        {
            ll nextindex = sfx_val[i].first + k / 2;
            if (nextindex < n)
                sfx_val[i].second.second = sfx_val[index[nextindex]].second.first;

            else
                sfx_val[i].second.second = -1;
        }
        sort(sfx_val.begin(), sfx_val.end(), cmp_func);
    }
    vll result;
    for(auto i: sfx_val ){
        result.push_back( i.first );
    }
    return result;
}

int main()
{
    string s;
    cin >> s;
    vll ans = create_sfx_arr(s);
    // for (auto i : ans)
    //     cout << i << " ";
    // cout << endl;

    ll smallest = ans[0];
    cout << s.substr(smallest) << s.substr(0,smallest);
    return 0;
}