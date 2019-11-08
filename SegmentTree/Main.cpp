#include "SegmentTree.hpp"
#include <fstream>

int main() {
	auto in = std::ifstream("sum.in");
	auto out = std::ofstream("sum.out");

	int n, k;
	in >> n >> k;

	auto data = std::vector<long long>(n, 0);
	auto sum_tree = SegmentTree::SegmentTree<long long>(
		data,
		[](long long a, long long b) { return a + b; },
		0ll
	);
	sum_tree.verbose(true);

	for (; k > 0; --k) {
		char q;
		int l, r, x;
		in >> q >> l >> r;
		if (q == 'A') {
			in >> x;
			sum_tree.update(l - 1, r, x);
		}
		else if (q == 'Q')
			out << sum_tree.get(l - 1, r) << '\n';
	}
	system("pause");
}	