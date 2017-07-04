#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <fstream>
#include <iostream>
using std::max;
using std::cout;
using std::endl;
using std::string;
using std::fstream;

class Compressor;

template <class T>
class QuadTree {
	friend class Compressor;
private:
	T value;
	QuadTree *p_ne;
	QuadTree *p_se;
	QuadTree *p_sw;
	QuadTree *p_nw;
public:

	QuadTree(T val = false) {
		p_ne = p_se = p_sw = p_nw = NULL;
		value = val;
	}

	unsigned maxHeight(int l = 0) {
		if (allNodesAreNull()) {
			return 1;
		}
		else {
			return max(max(maxHeight(p_ne, l), maxHeight(p_se, l)),
				max(maxHeight(p_sw, l), maxHeight(p_nw, l))) + 1;
		}
	}

	void saveToFile(string name, int size) {
		fstream f(name + ".4tree", std::ios::out);
		f << '[' << size << ']';
		saveToFile(this, f);
		f.close();
	}

	static QuadTree<bool>** loadFrom4TreeFile(string name, int *size) {
		fstream f(name, std::ios::in);

		if (f.bad()) {
			cout << "QuadTree::loadFrom4TreeFile: Cannot open file " << name << endl;
			system("pause");
			return NULL;
		}

		char redundantCharBuffer;
		string stringBuffer;
		f >> redundantCharBuffer >> *size >> redundantCharBuffer  >> stringBuffer;
		f.close();

		QTBool* quadTree = new QTBool(false);

		int it = 0;

		loadFrom4TreeFile(&quadTree, stringBuffer, &it);

		return &quadTree;
	}

private:
	static unsigned maxHeight(QuadTree<T> *qt, int l) {
		if (!qt) {
			return l;
		}
		if (qt->allNodesAreNull()) {
			return l + 1;
		}
		else {
			return max(max(maxHeight(qt->p_ne, l + 1), maxHeight(qt->p_se, l + 1)),
				max(maxHeight(qt->p_sw, l + 1), maxHeight(qt->p_nw, l + 1)));
		}
	}

	inline bool allNodesAreNull() {
		return !p_ne && !p_se && !p_sw && !p_nw;
	}

	static void saveToFile(QuadTree<bool> *qt, fstream &f) {
		if (!qt) return;

		f << qt->value;
		if (!qt->allNodesAreNull()) {
			//if(!qt->value){
			f << '(';
			saveToFile(qt->p_ne, f);
			saveToFile(qt->p_se, f);
			saveToFile(qt->p_sw, f);
			saveToFile(qt->p_nw, f);
			f << ')';
		}
	}

	static void loadFrom4TreeFile(QuadTree<bool> **qt, string &s, int *it) {
		if (*it == s.size()) {
			return;
		}

		char c = s[*it];

		if (c == '1') {
			(*qt)->value = true;
		}
		else if (c == '0') {
			(*qt)->value = false;
			if (s[*it + 1] == '(') {
				(*it) = (*it) + 2;

				c = s[*it];
				   //p_ne
				if (s[(*it) + 1] != '0' && s[(*it) + 1] != '1') {
					(*qt)->p_ne = new QuadTree();
					loadFrom4TreeFile(&(*qt)->p_ne, s, it);
				}
				else {
					if (c == '1') {
						(*qt)->p_ne = new QuadTree(true);
					}
					else if (c == '0') {
						(*qt)->p_ne = new QuadTree(false);
						if (s[*it] == '(') {
							loadFrom4TreeFile(&(*qt)->p_ne, s, it);
						}
					}
				}
				(*it)++;
				c = s[(*it)];

				//p_se
				if (s[(*it) + 1] != '0' && s[(*it) + 1] != '1') {
					(*qt)->p_se = new QuadTree();
					loadFrom4TreeFile(&(*qt)->p_se, s, it);
				}
				else {
					if (c == '1') {
						(*qt)->p_se = new QuadTree(true);
					}
					else if (c == '0') {
						(*qt)->p_se = new QuadTree(false);
						if (s[*it] == '(') {
							loadFrom4TreeFile(&(*qt)->p_se, s, it);
						}
					}
				}
				(*it)++;
				c = s[(*it)];
				//p_sw
				if (s[(*it) + 1] != '0' && s[(*it) + 1] != '1') {
					(*qt)->p_sw = new QuadTree();
					loadFrom4TreeFile(&(*qt)->p_sw, s, it);
				}
				else {
					if (c == '1') {
						(*qt)->p_sw = new QuadTree(true);
					}
					else if (c == '0') {
						(*qt)->p_sw = new QuadTree(false);
						if (s[*it] == '(') {
							loadFrom4TreeFile(&(*qt)->p_sw, s, it);
						}
					}
				}
				(*it)++;
				c = s[(*it)];

				//p_nw
				if (s[(*it) + 1] != '0' && s[(*it) + 1] != '1') {
					(*qt)->p_nw = new QuadTree();
					loadFrom4TreeFile(&(*qt)->p_nw, s, it);
				}
				else {
					if (c == '1') {
						(*qt)->p_nw = new QuadTree(true);
					}
					else if (c == '0') {
						(*qt)->p_nw = new QuadTree(false);
						if (s[*it] == '(') {
							loadFrom4TreeFile(&(*qt)->p_sw, s, it);
						}
					}
				}
				(*it)++;
			}
		}
		else if (c == ')') {
			(*it)++;
		}
		else {
			cout << "Quadtree::loadFrom4TreeFile: file is corrupted!" << endl;
			system("pause");
			return;
		}

	}

};


#endif