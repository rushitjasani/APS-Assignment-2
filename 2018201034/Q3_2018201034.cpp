// @auther : Rushit Jasani
// problem : nth_element()
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

void nth_element_my(vector<ll> &arr, ll k)
{
    ll low = 0;
    ll high = arr.size() - 1;
    ll pi = partition_my(arr, low, high);
    while (pi != k)
    {
        if (pi < k)
        {   
            low = pi+1;
            pi = partition_my(arr, low, high);
        }
        else if (pi > k)
        {
            high = pi-1;
            pi = partition_my(arr, low,high);
        }
    }
    printf("%lld\n", arr[k]);
}

int main()
{
    ll n, k;
    scanf("%lld", &n);
    scanf("%lld", &k);
    vector<ll> arr(n, 0);
    for (auto i = 0; i < n; i++)
        scanf("%lld", &arr[i]);
    vector<ll> arr1(arr.begin(), arr.end());
    
    if ((k > n) || k <= 0)
    {
        printf("-1\n");
        exit( EXIT_FAILURE );
    }

/////////////////////////////////////////////////////////////////////////////////////////////////////
    clock_t begin = clock();
    nth_element_my(arr, k-1 );
    clock_t end = clock();
    cout << "Time Elapsed: " << 1.0 * (end - begin) / CLOCKS_PER_SEC << " sec\n";
/////////////////////////////////////////////////////////////////////////////////////////////////////
    clock_t begin1 = clock();

    nth_element(arr1.begin(), arr1.begin() + k-1  , arr1.end());
    printf("%lld\n", arr1[k-1]);

    clock_t end1 = clock();
    cout << "Time Elapsed: " << 1.0 * (end1 - begin1) / CLOCKS_PER_SEC << " sec\n";
    return 0;
}