#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

const int MAX_N = 150000;       // 最大商店数量
const int MAX_VAL = 1000000;    // 最大独特度值

// 事件结构体：用于表示扫描线中的事件
struct Event {
    int pos;   // 事件触发的位置（对应b1的位置）
    int l, r;  // 影响b2的区间[l, r]
    int type;  // 事件类型：+1表示添加贡献，-1表示移除贡献
};

// 线段树结构体：用于区间更新和最大值查询
struct SegmentTree {
    struct Node {
        int l, r;        // 节点代表的区间[l, r]
        int max_val;     // 区间最大值
        int idx;         // 最大值对应的位置（b2的值）
        int lazy;        // 懒惰标记，用于延迟更新
    };
    vector<Node> tree;   // 线段树节点数组
    int n;               // 线段树覆盖的总区间大小

    // 构造函数：初始化线段树
    SegmentTree(int size) {
        n = size;
        tree.resize(4 * n + 10);  // 分配4倍空间
        build(1, 1, n);           // 构建线段树
    }

    // 构建线段树
    void build(int id, int l, int r) {
        tree[id].l = l;
        tree[id].r = r;
        tree[id].lazy = 0;
        if (l == r) {
            tree[id].max_val = 0;  // 初始值设为0
            tree[id].idx = l;      // 记录位置
            return;
        }
        int mid = (l + r) / 2;
        build(id * 2, l, mid);       // 构建左子树
        build(id * 2 + 1, mid + 1, r); // 构建右子树
        push_up(id);                 // 更新父节点
    }

    // 更新父节点信息
    void push_up(int id) {
        // 选择左右子树中较大的值
        if (tree[id * 2].max_val >= tree[id * 2 + 1].max_val) {
            tree[id].max_val = tree[id * 2].max_val;
            tree[id].idx = tree[id * 2].idx;
        } else {
            tree[id].max_val = tree[id * 2 + 1].max_val;
            tree[id].idx = tree[id * 2 + 1].idx;
        }
    }

    // 下推懒惰标记
    void push_down(int id) {
        if (tree[id].lazy != 0) {
            int lazy_val = tree[id].lazy;
            // 更新左子树
            tree[id * 2].lazy += lazy_val;
            tree[id * 2].max_val += lazy_val;
            // 更新右子树
            tree[id * 2 + 1].lazy += lazy_val;
            tree[id * 2 + 1].max_val += lazy_val;
            tree[id].lazy = 0;  // 清除标记
        }
    }

    // 区间更新接口
    void update(int l, int r, int val) {
        update(1, l, r, val);
    }

    // 实际更新操作
    void update(int id, int L, int R, int val) {
        // 当前节点区间与更新区间无交集
        if (tree[id].l > R || tree[id].r < L) {
            return;
        }
        // 当前节点区间完全包含在更新区间内
        if (L <= tree[id].l && tree[id].r <= R) {
            tree[id].lazy += val;      // 更新懒惰标记
            tree[id].max_val += val;   // 更新节点值
            return;
        }
        push_down(id);  // 下推懒惰标记
        int mid = (tree[id].l + tree[id].r) / 2;
        if (L <= mid) {
            update(id * 2, L, R, val);  // 更新左子树
        }
        if (R > mid) {
            update(id * 2 + 1, L, R, val);  // 更新右子树
        }
        push_up(id);  // 更新父节点
    }

    // 区间查询接口
    pair<int, int> query(int l, int r) {
        return query(1, l, r);
    }

    // 实际查询操作
    pair<int, int> query(int id, int L, int R) {
        // 当前节点区间与查询区间无交集
        if (tree[id].l > R || tree[id].r < L) {
            return {-1000000000, 0};  // 返回极小值
        }
        // 当前节点区间完全包含在查询区间内
        if (L <= tree[id].l && tree[id].r <= R) {
            return {tree[id].max_val, tree[id].idx};
        }
        push_down(id);  // 下推懒惰标记
        int mid = (tree[id].l + tree[id].r) / 2;
        // 查询区间完全在左子树
        if (R <= mid) {
            return query(id * 2, L, R);
        } 
        // 查询区间完全在右子树
        else if (L > mid) {
            return query(id * 2 + 1, L, R);
        } 
        // 查询区间跨越左右子树
        else {
            pair<int, int> left_res = query(id * 2, L, R);
            pair<int, int> right_res = query(id * 2 + 1, L, R);
            // 返回左右子树中的最大值及其位置
            if (left_res.first >= right_res.first) {
                return left_res;
            } else {
                return right_res;
            }
        }
    }
};