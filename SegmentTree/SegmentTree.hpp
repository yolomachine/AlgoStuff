﻿/**
 *  @file SegmentTree.hpp
 *  
 *  @brief Segment tree implementation
 *  
 *  @author Alexander Gomeniuk
 *  Contact: gomeniuk.aa@gmail.com
 */

#pragma once
#include <cstddef>
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <io.h>
#include <fcntl.h>

namespace SegmentTree {
	template <typename T>
	class SegmentTree {
	public:
		SegmentTree(const std::vector<T>& data, std::function<T(T, T)> functor, T neutral_element) :
			_tree(4 * data.size(), neutral_element),
			_inconsistency(4 * data.size(), false),
			_functor(functor),
			_neutral_element(neutral_element),
			_verbose(false),
			_mode(_O_U16TEXT),
			verbosity_wostream(std::wcout) {
			build(data, 0, 0, data.size());
		}

		~SegmentTree() {}

		T get(int ql, int qr);
		T assign(int pos, T val);
		T assign(int ql, int qr, T val);

		void print();
		void print(std::wostream& os);
		void verbose(bool val);

		std::wostream& verbosity_wostream;

	protected:
		void print(std::wostream& os, int i, int l, int r, std::vector<std::pair<int, bool>> margins);
		void build(const std::vector<T>& data, int i, int l, int r);
		void push(int i, int l, int r);
		T get(int i, int l, int r, int ql, int qr);
		T assign(int i, int l, int r, int pos, T val);
		T assign(int i, int l, int r, int ql, int qr, T val);

		std::vector<T> _tree;
		std::vector<bool> _inconsistency;
		std::function<T(T, T)> _functor;
		T _neutral_element;
		bool _verbose;
		int _mode;
	};

	template<typename T>
	inline void SegmentTree<T>::verbose(bool val) {
		if (_verbose != val) {
			_verbose = val;
			_mode = _setmode(_fileno(stdout), _mode);
		}
	}

	template<typename T>
	inline void SegmentTree<T>::print() {
		print(verbosity_wostream);
	}

	template<typename T>
	inline void SegmentTree<T>::print(std::wostream& os) {
		print(os, 0, 0, _tree.size() / 4, std::vector<std::pair<int, bool>>(0));
	}

	template<typename T>
	inline void SegmentTree<T>::print(std::wostream& os, int i, int l, int r, std::vector<std::pair<int, bool>> margins) {
		for (auto j = 0; j < margins.size(); ++j) {
			if (j != 0) {
				auto tmp = i;
				for (auto k = j; k < margins.size(); ++k) {
					tmp = (tmp % 2 == 0) ? (tmp - 2) / 2 : (tmp - 1) / 2;
				}
				if (tmp % 2 == 1) {
					os << L'│';
				}
				else
					os << L' ';
			}
			else
				os << L' ';
			for (auto k = 0; k < (margins[j].first - margins[j].second) - 1; ++k)
				os << L" ";
		};
		auto range = L'[' + std::to_wstring(l) + L',' + std::to_wstring(r) + L"): ";
		os << ((r - l == 1 || i % 2 == 0) ? L'└' : L'├') << L'─' << range << std::to_wstring(_tree[i]) << std::endl;

		if (r - l == 1)
			return;

		int m = (l + r) / 2;
		margins.push_back({ std::to_string(_tree[i]).length() + 2 + range.length(), (r - l) != 1 });
		print(os, 2 * i + 1, l, m, margins);
		margins.pop_back();
		margins.push_back({ std::to_string(_tree[i]).length() + 2 + range.length(), (r - l) != 1 });
		print(os, 2 * i + 2, m, r, margins);
		margins.pop_back();
	}

	template<typename T>
	inline T SegmentTree<T>::get(int ql, int qr) {
		if (ql > qr)
			std::swap(ql, qr);
		auto res = get(0, 0, _tree.size() / 4, ql, qr);
		if (_verbose) {
			verbosity_wostream << L"Get: [" << ql << L',' << qr << L"): " << res << std::endl;
			print();
			verbosity_wostream << std::endl << std::endl;
		}
		return res;
	}

	template<typename T>
	inline T SegmentTree<T>::assign(int pos, T val) {
		auto res = assign(0, 0, _tree.size() / 4, pos, val);
		if (_verbose) {
			verbosity_wostream << L"Update " << pos << L' ' << val << std::endl;
			print();
			verbosity_wostream << std::endl;
		}
		return res;
	}

	template<typename T>
	inline T SegmentTree<T>::assign(int ql, int qr, T val) {
		auto res = assign(0, 0, _tree.size() / 4, ql, qr, val);
		if (_verbose) {
			verbosity_wostream << L"Update [" << ql << L',' << qr << L"): " << val << std::endl;
			print(verbosity_wostream);
			verbosity_wostream << std::endl;
		}
		return res;
	}

	template<typename T>
	inline void SegmentTree<T>::push(int i, int l, int r) {
		if (r - l <= 1)
			return;
		if (_inconsistency[i]) {
			_tree[2 * i + 1] = _tree[2 * i + 2] = _tree[i];
			_inconsistency[2 * i + 1] = _inconsistency[2 * i + 2] = true;
			_inconsistency[i] = false;
			push(2 * i + 1, l, (l + r) / 2);
			push(2 * i + 2, (l + r) / 2, r);
			_tree[i] = _functor(_tree[2 * i + 1], _tree[2 * i + 2]);
		}
	}

	template<typename T>
	inline void SegmentTree<T>::build(const std::vector<T>& data, int i, int l, int r) {
		if (l == r)
			return;

		if (r - l == 1)
			_tree[i] = data[l];
		else {
			int m = (l + r) / 2;
			build(data, 2 * i + 1, l, m);
			build(data, 2 * i + 2, m, r);
			_tree[i] = _functor(_tree[2 * i + 1], _tree[2 * i + 2]);
		}
	}

	template<typename T>
	inline T SegmentTree<T>::get(int i, int l, int r, int ql, int qr) {
		if (r <= ql || l >= qr)
			return _neutral_element;

		if (ql <= l && qr >= r) {
			if (_inconsistency[i] && r - l > 1)
				push(i, l, r);
			return _tree[i];
		}

		push(i, l, r);
		int m = (l + r) / 2;
		return _functor(get(2 * i + 1, l, m, ql, qr), get(2 * i + 2, m, r, ql, qr));
	}

	template<typename T>
	inline T SegmentTree<T>::assign(int i, int l, int r, int pos, T val) {
		if (pos < l || pos >= r) {
			if (_inconsistency[i] && r - l > 1)
				push(i, l, r);
			return _tree[i];
		}

		if (r - l == 1 && pos == l)
			return _tree[i] = val;

		push(i, l, r);
		int m = (l + r) / 2;
		return _tree[i] = _functor(assign(2 * i + 1, l, m, pos, val), assign(2 * i + 2, m, r, pos, val));
	}

	template<typename T>
	inline T SegmentTree<T>::assign(int i, int l, int r, int ql, int qr, T val) {
		if (r <= ql || l >= qr) {
			if (_inconsistency[i] && r - l > 1)
				push(i, l, r);
			return _tree[i];
		}

		if (ql <= l && qr >= r) {
			_inconsistency[i] = true;
			_tree[i] = val;
			if (r - l > 1)
				push(i, l, r);
			return _tree[i];
		}

		push(i, l, r);
		int m = (l + r) / 2;
		return _tree[i] = _functor(assign(2 * i + 1, l, m, ql, qr, val), assign(2 * i + 2, m, r, ql, qr, val));
	}
}