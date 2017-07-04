#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include <SFML/Graphics.hpp>
#include <fstream>

class Compressor;

template <class T>
class QuadTree {
	friend class Compressor;
	//private:
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
		//cout << p_ne << " " << p_se << " " << p_sw << " " << p_nw << endl;
		if (allNodesAreNull()) {
			return 1;
		}
		else {
			return max(max(maxHeight(p_ne, l + 1), maxHeight(p_se, l + 1)),
				max(maxHeight(p_sw, l + 1), maxHeight(p_nw, l + 1)));
		}
	}

	void saveToFile(std::string name, int size) {
		std::fstream f(name + ".4tree", std::ios::out);
		f << '[';
		f << size;
		f << ']';
		saveToFile(this, f);
		f.close();
	}

	static QuadTree<bool>** loadFrom4TreeFile(std::string name, int *size) {
		std::fstream f(name, std::ios::in);

		if (f.bad()) {
			//cout << "BAD" << endl;
			return NULL;
		}

		char c;
		f >> c;
		f >> *size;
		f >> c;

		std::string s;
		f >> s;
		f.close();
		//cout << s << endl;
		QT* quadTree = new QT(false);

		int it = 0;

		//cout << s << endl;
		loadFrom4TreeFile(&quadTree, s, &it);

		return &quadTree;
	}

private:
	static unsigned maxHeight(QuadTree<T> *qt, int l) {
		if (!qt) {
			return l;
		}
		if (!qt->p_ne && !qt->p_se && !qt->p_sw && !qt->p_nw) {
			return l + 1;
		}
		else {
			return max(max(maxHeight(qt->p_ne, l + 1), maxHeight(qt->p_se, l + 1)),
				max(maxHeight(qt->p_sw, l + 1), maxHeight(qt->p_nw, l + 1)));
		}
	}

	bool allNodesAreNull() {
		return !p_ne && !p_se && !p_sw && !p_nw;
	}

	static void saveToFile(QuadTree<bool> *qt, std::fstream &f) {
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

	static void loadFrom4TreeFile(QuadTree<bool> **qt, std::string &s, int *it) {
		if (*it == s.size()) {
			return;
		}

		char c = s[*it];

		if (c == '1') {
			//cout << "[" << *it << "] < " << c << s[*it] << " >" << *qt << "(main) na true, to samo" << endl;
			(*qt)->value = true;
		}
		else if (c == '0') {
			//cout << "[" << *it << "] < " << c << s[*it] << " >" << *qt << "(main) na false, to samo" << endl;
			(*qt)->value = false;
			if (s[*it + 1] == '(') {
				(*it) = (*it) + 2;

				c = s[*it];
				//cout << "p: " << *it << endl;
				   //p_ne
				if (s[(*it) + 1] != '0' && s[(*it) + 1] != '1') {
					(*qt)->p_ne = new QuadTree();
					loadFrom4TreeFile(&(*qt)->p_ne, s, it);
				}
				else {
					if (c == '1') {
						(*qt)->p_ne = new QuadTree(true);
						//cout << "[" << *it << "] < " << c << s[*it] << " >" << (*qt)->p_ne << "p_ne na true, to samo" << endl;
					}
					else if (c == '0') {
						(*qt)->p_ne = new QuadTree(false);
						//cout << "[" << *it << "] < " << c << s[*it] << " >" << (*qt)->p_ne << "p_ne na false, to samo" << endl;
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
						//cout << "[" << *it << "] < " << c << s[*it] << " >" << (*qt)->p_se << "p_se na true, to samo" << endl;
					}
					else if (c == '0') {
						(*qt)->p_se = new QuadTree(false);
						//cout << "[" << *it << "] < " << c << s[*it] << " >" << (*qt)->p_se << "p_se na false, to samo" << endl;
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
						//cout << "[" << *it << "] < " << c << s[*it] << " >" << (*qt)->p_sw << "p_sw na true, to samo" << endl;
					}
					else if (c == '0') {
						(*qt)->p_sw = new QuadTree(false);
						//cout << "[" << *it << "] < " << c << s[*it] << " >" << (*qt)->p_sw << "p_sw na false, to samo" << endl;
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
						//cout << "[" << *it << "] < " << c << s[*it] << " >" << (*qt)->p_nw << "p_nw na true, to samo" << endl;
					}
					else if (c == '0') {
						(*qt)->p_nw = new QuadTree(false);
						//cout << "[" << *it << "] < " << c << s[*it] << " >" << (*qt)->p_nw << "p_nw na false, to samo" << endl;
						if (s[*it] == '(') {
							loadFrom4TreeFile(&(*qt)->p_sw, s, it);
						}
					}
				}
				(*it)++;
			}
			else {
				return;
			}
		}
		else if (c == ')') {
			(*it)++;
		}
		else {
			//cout << "FILE CORRUPTED bo < " << c << " >" << endl;
		}



		/*
		if (c == '1') {
		 //cout << "[" << *it << "] < " << c<<s[*it] << " >"<<*qt<<" na true, cofaj" << endl;
			(*qt)->value = true;
			return;
		}
		else if(c=='0'){
		 //cout << "[" << *it << "] < " << c<<s[*it] << " >" << *qt << " na false, to samo" << endl;
			(*qt)->value = false;
			loadFrom4TreeFile(qt, s, it);
		}
		else if(c==')'){
		 //cout << "[" << *it << "] < " << c<<s[*it] << " > it++, zamykajacy zwykly, cofaj" << endl;
			return;
		}
		else if(c=='('){
			if (s[*it] == ')') {
			 //cout << "[" << *it << "] < " << c<<s[*it] << " > zamykajacy po (, cofaj" << endl;
				return;
			}
			else {
			 //cout << "[" << *it << "] < " << c<<s[*it] << " >, rozdzielamy po (, dalej" << endl;
				//p_ne
				(*qt)->p_ne = new QuadTree(false);
				loadFrom4TreeFile(&(*qt)->p_ne, s, it);
				//p_se
				(*qt)->p_se = new QuadTree(false);
				loadFrom4TreeFile(&(*qt)->p_se, s, it);
				//p_sw
				(*qt)->p_sw = new QuadTree(false);
				loadFrom4TreeFile(&(*qt)->p_sw, s, it);
				//p_nw
				(*qt)->p_nw = new QuadTree(false);
				loadFrom4TreeFile(&(*qt)->p_nw, s, it);
			}
		}
		else {
		 //cout << "BAD CHAR ["<<c<<"]" << endl;
		}
		*/
	}

};


#endif