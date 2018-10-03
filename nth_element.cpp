#include <bits/stdc++.h>
using namespace std;
#define ll long long

ll partition_my(vector<ll> &arr, ll low, ll high)
{

    ll rand_num = low + (rand()) % (high - low + 1);
    swap(arr[rand_num], arr[low]);

    ll i = low + 1;
    ll piv = arr[low];
    for (ll j = low + 1; j <= high; j++)
    {
        if (arr[j] < piv)
        {
            swap(arr[i], arr[j]);
            i++;
        }
    }
    swap(arr[low], arr[i - 1]);
    return i - 1;
}

void nth_element(vector<ll> &arr, ll k)
{
    ll low = 0;
    ll high = arr.size() - 1;
    ll pi = partition_my(arr, low, high);
    while (pi != k)
    {
        if (pi < k)
        {
            pi = partition_my(arr, pi + 1, high);
        }
        else if (pi > k)
        {
            pi = partition_my(arr, low, pi - 1);
        }
    }
    printf("%lld\n", arr[k]);
}

int main()
{
    ll n, k;
    scanf("%lld", &n);
    vector<ll> arr(n, 0);
    for (auto i = 0; i < n; i++)
        scanf("%lld", &arr[i]);
    while(1)
    {
        scanf("%lld", &k);
        if((k > n) || k <= 0 ){
            printf("-1\n");
            continue;
        }
        nth_element(arr, k - 1);
    }
    return 0;
}