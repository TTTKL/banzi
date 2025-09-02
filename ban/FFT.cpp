//https://ac.nowcoder.com/acm/contest/108305/J

#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
#include <algorithm>
#include <string>
using namespace std;

const double PI = acos(-1);

// FFT函数：快速傅里叶变换
void fft(vector<complex<double>>& a, int n, int op) {
    // 位反转重排
    for (int i = 0, j = 0; i < n; i++) {
        if (i < j) swap(a[i], a[j]);
        for (int k = n >> 1; (j ^= k) < k; k >>= 1);
    }
    // 蝶形运算
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * op;
        complex<double> wn(cos(ang), sin(ang)); // 旋转因子
        for (int i = 0; i < n; i += len) {
            complex<double> w(1, 0);
            for (int j = 0; j < len / 2; j++) {
                complex<double> u = a[i + j];
                complex<double> v = w * a[i + j + len / 2];
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wn; // 更新旋转因子
            }
        }
    }
    // 逆变换需缩放
    if (op == -1) {
        for (int i = 0; i < n; i++) {
            a[i] /= n;
        }
    }
}

// 卷积计算：使用FFT或暴力法（小规模数据）
vector<int> convolve(vector<int>& a, vector<int>& b) {
    if (a.empty() || b.empty()) {
        return vector<int>();
    }
    int n1 = a.size(), n2 = b.size();
    int res_size = n1 + n2 - 1;

    // 小规模数据使用暴力卷积
    if (n1 <= 128 || n2 <= 128) {
        vector<int> c(res_size, 0);
        for (int i = 0; i < n1; i++) {
            for (int j = 0; j < n2; j++) {
                c[i + j] += a[i] * b[j];
            }
        }
        return c;
    }

    // 大规模数据使用FFT
    int n = 1;
    while (n < res_size) n <<= 1; // 扩展到2的幂
    vector<complex<double>> fa(n), fb(n);
    // 填充数据
    for (int i = 0; i < n1; i++) fa[i] = a[i];
    for (int i = 0; i < n2; i++) fb[i] = b[i];

    // FFT计算
    fft(fa, n, 1);   // 正变换
    fft(fb, n, 1);
    for (int i = 0; i < n; i++) fa[i] *= fb[i]; // 点乘
    fft(fa, n, -1);  // 逆变换

    // 取整并返回结果
    vector<int> c(res_size);
    for (int i = 0; i < res_size; i++) {
        c[i] = round(fa[i].real());
    }
    return c;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    int T;
    cin >> T;
    while (T--) {
        string A, B;
        cin >> A >> B;
        // 反转字符串：低位在索引0
        reverse(A.begin(), A.end());
        reverse(B.begin(), B.end());

        // 分离实部和虚部
        vector<int> A_real, A_img, B_real, B_img;
        for (int i = 0; i < A.size(); i++) {
            if (i % 2 == 0) A_real.push_back(A[i] - '0'); // 实部：偶数位
            else A_img.push_back(A[i] - '0');             // 虚部：奇数位
        }
        for (int i = 0; i < B.size(); i++) {
            if (i % 2 == 0) B_real.push_back(B[i] - '0');
            else B_img.push_back(B[i] - '0');
        }

        // 计算四个卷积
        vector<int> rr = convolve(A_real, B_real); // 实*实
        vector<int> ii = convolve(A_img, B_img);   // 虚*虚
        vector<int> ri = convolve(A_real, B_img);  // 实*虚
        vector<int> ir = convolve(A_img, B_real);  // 虚*实

        // 构造实部多项式：rr - 2*ii
        int len_real = max(rr.size(), ii.size());
        vector<int> real_poly(len_real, 0);
        for (int i = 0; i < rr.size(); i++) real_poly[i] = rr[i];
        for (int i = 0; i < ii.size(); i++) real_poly[i] -= 2 * ii[i];

        // 构造虚部多项式：ri + ir
        int len_img = max(ri.size(), ir.size());
        vector<int> img_poly(len_img, 0);
        for (int i = 0; i < ri.size(); i++) img_poly[i] = ri[i];
        for (int i = 0; i < ir.size(); i++) img_poly[i] += ir[i];

        // 合并多项式：实部在偶数位，虚部在奇数位
        int max_index = 2 * max(len_real, len_img) + 1000; // 确保足够大
        vector<int> d(max_index, 0);
        for (int i = 0; i < len_real; i++) d[2 * i] = real_poly[i]; // 实部
        for (int i = 0; i < len_img; i++) d[2 * i + 1] = img_poly[i]; // 虚部

        // 进位调整：从低位到高位
        for (int k = 0; k < max_index; k++) {
            if (k >= max_index - 2) break; // 防止越界
            int ck = d[k];
            // 调整到0或1：ck mod 2
            int r = ck % 2;
            if (r < 0) r += 2; // 确保非负
            r %= 2;
            // 计算进位量（基的递推关系）
            int carry = (r - ck) / 2;
            d[k] = r; // 当前位保留0或1
            d[k + 2] += carry; // 进位到高位
        }

        // 寻找最高非零位
        int last_non_zero = max_index - 1;
        while (last_non_zero >= 0 && d[last_non_zero] == 0) {
            last_non_zero--;
        }

        // 输出结果
        if (last_non_zero < 0) {
            cout << "0\n"; // 全零情况
        } else {
            string ans;
            // 从高位到低位生成字符串
            for (int i = last_non_zero; i >= 0; i--) {
                ans += ('0' + d[i]);
            }
            cout << ans << '\n';
        }
    }
    return 0;
}