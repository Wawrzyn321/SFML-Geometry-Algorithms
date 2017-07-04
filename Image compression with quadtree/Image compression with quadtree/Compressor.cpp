#include "Compressor.h"

Compressor::Compressor(unsigned resolution) {
	if (resolution >= 1) {
		this->resolution = resolution;
	}
}

Compressor::Compressor(string path, unsigned resolution) {
	if (!image.loadFromFile(path)) {
		system("pause");
		exit(1);
	}
	width = image.getSize().x;
	height = image.getSize().y;
	numberOfNodes = 0;
}

void Compressor::setResolution(unsigned resolution) {
	if (resolution >= 1) {
		this->resolution = resolution;
	}
}

void Compressor::preprocess() {
	sf::Color c;
	for (unsigned i = 0; i < width; i++) {
		for (unsigned j = 0; j < height; j++) {
			c = image.getPixel(i, j);
			if (c != sf::Color::Black && c != sf::Color::White) {
				if (c.r > 127 && c.g > 127 && c.b > 127) {
					image.setPixel(i, j, sf::Color::White);
				}
				else {
					image.setPixel(i, j, sf::Color::Black);
				}
			}
		}
	}
}

void Compressor::create() {
	quadTree = new QTBool();
	numberOfNodes++;
	RectUnsigned r = RectUnsigned(0, 0, width, height);
	check(r, &quadTree);
}

void Compressor::loadFromCompressor(Compressor * comp)
{
	width = comp->width;
	height = comp->height;
	numberOfNodes = comp->numberOfNodes;

	image.create(width, height, sf::Color::White);

	loadFromTree(RectUnsigned(0, 0, width, height), comp->quadTree);
}

unsigned Compressor::getNodesNumber() {
	return numberOfNodes;
}

unsigned Compressor::getMaxTreeHeight()
{
	return quadTree ? quadTree->maxHeight() : 0;
}

void Compressor::saveImage(string path) {
	image.saveToFile(path);
}

void Compressor::saveToFile(string name)
{
	if (quadTree) {
		quadTree->saveToFile(name, max(width, height));
	}
}

void Compressor::loadFrom4TreeFile(string name) {
	int s;
	quadTree = *(QTBool::loadFrom4TreeFile(name, &s));
	width = height = s;
	image.create(width, height, sf::Color::White);
	RectUnsigned rect = RectUnsigned(0, 0, width, height);
	loadFromTree(rect, quadTree);
}

void Compressor::check(RectUnsigned rect, QTBool **q) {
	if (rect.width <= resolution || rect.height <= resolution) {
		if (isFloatRectGood(rect)) {
			(*q)->value = image.getPixel(rect.left, rect.top) == sf::Color::Black;
		}
		return;
	}
	else {
		unsigned halfWidth = rect.width / 2;
		unsigned halfHeight = rect.height / 2;
		checkBranch(RectUnsigned(rect.left + halfWidth, rect.top, halfWidth, halfHeight), &(*q)->p_ne);
		checkBranch(RectUnsigned(rect.left + halfWidth, rect.top + halfWidth, halfWidth, halfHeight), &(*q)->p_se);
		checkBranch(RectUnsigned(rect.left, rect.top + halfWidth, halfWidth, halfHeight), &(*q)->p_sw);
		checkBranch(RectUnsigned(rect.left, rect.top, halfWidth, halfHeight), &(*q)->p_nw);
	}
}

void Compressor::checkBranch(RectUnsigned rect, QTBool **q) {
	if (checkColor(rect, BLACK)) {
		//all black
		numberOfNodes++;
		*q = new QTBool(true);
	}
	else if (checkColor(rect, WHITE)) {
		//all white
		numberOfNodes++;
		*q = new QTBool(false);
	}
	else {
		//split
		numberOfNodes++;
		*q = new QTBool(false);
		check(rect, q);
	}
}

void Compressor::loadFromTree(RectUnsigned rect, QTBool * q)
{
	if (q == NULL) return;
	if (q->value) {
		fillColor(rect, BLACK);
	}
	else {
		if (q->allNodesAreNull()) {
			return;
		}
		else {
			unsigned halfWidth = rect.width / 2;
			unsigned halfHeight = rect.height / 2;
			loadFromTree(RectUnsigned(rect.left + halfWidth, rect.top, halfWidth, halfHeight), q->p_ne);
			loadFromTree(RectUnsigned(rect.left + halfWidth, rect.top + halfWidth, halfWidth, halfHeight), q->p_se);
			loadFromTree(RectUnsigned(rect.left, rect.top + halfWidth, halfWidth, halfHeight), q->p_sw);
			loadFromTree(RectUnsigned(rect.left, rect.top, halfWidth, halfHeight), q->p_nw);
		}
	}

}

bool Compressor::checkColor(RectUnsigned rect, bool color) {
	if (!isFloatRectGood(rect)) {
		return false;
	}
	for (unsigned i = rect.left; i < rect.left + rect.height; i++) {
		for (unsigned j = rect.top; j < rect.top + rect.width; j++) {
			if (color) {
				if (image.getPixel(i, j) == sf::Color::White) {
					return false;
				}
			}
			else {
				if (image.getPixel(i, j) == sf::Color::Black) {
					return false;
				}
			}
		}
	}
	return true;
}

void Compressor::fillColor(RectUnsigned rect, bool color) {
	if (!isFloatRectGood(rect)) {
		return;
	}
	for (unsigned i = rect.left; i < rect.left + rect.height; i++) {
		for (unsigned j = rect.top; j < rect.top + rect.width; j++) {
			image.setPixel(i,j, color ? sf::Color::Black : sf::Color::White);
		}
	}
}

bool Compressor::isFloatRectGood(RectUnsigned & rect)
{
	if (rect.left < 0 || rect.top < 0)  return false;
	if (rect.left + rect.width >= image.getSize().x)  return false;
	if (rect.top+ rect.height>= image.getSize().y) return false;
	return true;
}
