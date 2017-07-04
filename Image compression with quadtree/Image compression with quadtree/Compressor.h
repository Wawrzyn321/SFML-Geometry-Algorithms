#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include "Quadtree.h"

#define WHITE false
#define BLACK true

typedef QuadTree<bool> QT;
typedef sf::Rect<unsigned> RectUnsigned;

#include <iostream>
using namespace std;

struct Compressor {
private: 
	sf::Image image;
	unsigned width;
	unsigned height;
	QT *quadTree;
	unsigned numberOfNodes;
	unsigned realTreeSize;
	unsigned resolution = 1;
public:
	Compressor(int resolution = 1);
	Compressor(std::string path, int resolution = 1);

	void setResolution(unsigned resolution);

	void preprocess();

	void create();

	void loadFromCompressor(Compressor *comp);

	void saveImage(std::string path);

	void saveToFile(std::string name);

	void loadFrom4TreeFile(std::string name);

	unsigned getNodesNumber();

private:

	void check(RectUnsigned rect, QT **q);

	void checkBranch(RectUnsigned rect, QT **q);

	void loadFromTree(RectUnsigned rect, QT *q);

	bool checkColor(RectUnsigned rect, bool color);

	void fillColor(RectUnsigned rect, bool color);

};


#endif