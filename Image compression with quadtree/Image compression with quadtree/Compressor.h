#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <SFML/Graphics.hpp>
#include "Quadtree.h"

#define WHITE false
#define BLACK true

typedef QuadTree<bool> QTBool;
typedef sf::Rect<unsigned> RectUnsigned;

class Compressor {
private: 
	sf::Image image;
	unsigned width;
	unsigned height;
	QTBool *quadTree;
	unsigned numberOfNodes;
	unsigned realTreeSize;
	unsigned resolution = 1;
public:
	Compressor(unsigned resolution = 1);
	Compressor(string path, unsigned resolution = 1);

	void setResolution(unsigned resolution);

	void preprocess();

	void create();

	void loadFromCompressor(Compressor *comp);

	void saveImage(string path);

	void saveToFile(string name);

	void loadFrom4TreeFile(string name);

	unsigned getNodesNumber();

	unsigned getMaxTreeHeight();

private:

	void check(RectUnsigned rect, QTBool **q);

	void checkBranch(RectUnsigned rect, QTBool **q);

	void loadFromTree(RectUnsigned rect, QTBool *q);

	bool checkColor(RectUnsigned rect, bool color);

	void fillColor(RectUnsigned rect, bool color);

	bool isFloatRectGood(RectUnsigned &rect);
};


#endif