#include <fstream>
#include "SegmentTree.hpp"

typedef unsigned long long ull;

inline ull sum(ull a, ull b) {
	return a + b;
}

int main() {
	auto in = std::ifstream("sum.in");
	auto out = std::ofstream("sum.out");

	ull n, k;
	in >> n >> k;
	auto data = std::vector<ull>(n, 0ull);
	auto sum_tree = SegmentTree::SegmentTree<ull>(data, sum, 0ull);

	sum_tree.verbose(true);
	for (; k > 0; --k) {
		char q;
		ull l, r, x;
		in >> q >> l >> r;
		if (q == 'A') {
			in >> x;
			sum_tree.assign(l - 1, r, x);
		}
		else if (q == 'Q')
			out << sum_tree.get(l - 1, r) << '\n';
	}
	system("pause");
}