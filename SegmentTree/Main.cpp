#include "SegmentTree.hpp"
#include <io.h>
#include <fcntl.h>

int main() {
	auto data = std::vector<int>{ 124, 123, 124, -1, 20, 30, 40, 199, -10, 100, 100, 231, -123, 30123, -129 };
	auto min_tree = SegmentTree::SegmentTree<int>(
		data,
		[](int a, int b) { return std::min(a, b); },
		std::numeric_limits<int>::max()
	);
	_setmode(_fileno(stdout), _O_U16TEXT);
	min_tree.print(std::wcout);
	min_tree.update(0, 7, 0xbaadf00d);
	std::wcout << std::endl << min_tree.get(3, 4) << std::endl;
	min_tree.print(std::wcout);
	system("pause");
}