#include <bits/stdc++.h>
using namespace std;

int mod = 998244353;

void mul(int &x, int y) { x = 1ll * x * y % mod; }

int q_pow(int x, int k) { int res = 1; for (; k; k >>= 1, mul(x, x)) if (k & 1) mul(res, x); return res; }


const int N = 2e6 + 10;

int fac[N], inv[N];

void init() {
    fac[0] = inv[0] = 1;
    for (int i = 1; i < N; i++) fac[i] = 1ll * i * fac[i - 1] % mod;
    inv[N - 1] = q_pow(fac[N - 1], mod - 2);
    for (int i = N - 1; i; i--) inv[i - 1] = 1ll * inv[i] * i % mod;
}

int calc(int n, int m) {
    return 1ll * fac[n] * inv[m] % mod * inv[n - m] % mod;
}
