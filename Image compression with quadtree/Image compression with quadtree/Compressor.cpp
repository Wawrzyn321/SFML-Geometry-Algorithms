#include "Compressor.h"

Compressor::Compressor(int resolution) {
	this->resolution = resolution;
}

Compressor::Compressor(std::string path, int resolution) {
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
				if (c.r > 127) {
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
	quadTree = new QT();
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
	RectUnsigned rect = RectUnsigned(0, 0, width, height);
	loadFromTree(rect, comp->quadTree);
}

unsigned Compressor::getNodesNumber() {
	return numberOfNodes;
}

void Compressor::saveImage(std::string path) {
	image.saveToFile(path);
}

void Compressor::saveToFile(std::string name)
{
	if (quadTree) {
		quadTree->saveToFile(name, max(width, height));
	}
}

void Compressor::loadFrom4TreeFile(std::string name) {
	int s;
	quadTree = *(QT::loadFrom4TreeFile(name, &s));
	cout << quadTree << " " << quadTree->p_ne << " " << quadTree->p_nw << " " << quadTree->p_se << " " << quadTree->p_sw << endl;
	width = height = s;
	image.create(width, height, sf::Color::White);
	RectUnsigned rect = RectUnsigned(0, 0, width, height);
	loadFromTree(rect, quadTree);
}

void Compressor::check(RectUnsigned rect, QT **q) {
	if (rect.width <= resolution || rect.height <= resolution) {
		(*q)->value = image.getPixel(rect.left, rect.top) == sf::Color::Black;
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

void Compressor::checkBranch(RectUnsigned rect, QT **q) {
	if (checkColor(rect, BLACK)) {
		//all black
		numberOfNodes++;
		*q = new QT(true);
	}
	else if (checkColor(rect, WHITE)) {
		//all white
		numberOfNodes++;
		*q = new QT(false);
	}
	else {
		//branch!
		numberOfNodes++;
		*q = new QT(false);
		check(rect, q);
	}
}

void Compressor::loadFromTree(RectUnsigned rect, QT * q)
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
	//cout << rect.left << " " << rect.top << " " << rect.width << " " << rect.height;
	for (unsigned i = rect.left; i < rect.left + rect.height; i++) {
		for (unsigned j = rect.top; j < rect.top + rect.width; j++) {
			if (color) {
				if (image.getPixel(i, j) == sf::Color::White) {
					//cout << " nie jest cala czarna"<< endl;
					return false;
				}
			}
			else {
				//cout << (int)image.getPixel(i, j).r << (int)image.getPixel(i, j).g << (int)image.getPixel(i, j).b << endl;
				if (image.getPixel(i, j) == sf::Color::Black) {
					//cout << " nie jest cala biala" << endl;
					return false;
				}
			}
		}
	}
	if (color) {
		//cout << " cala czarna" << endl;
	}
	else {
		//cout << " cala biala"<< endl;
	}
	return true;
}

void Compressor::fillColor(RectUnsigned rect, bool color) {
	for (unsigned i = rect.left; i < rect.left + rect.height; i++) {
		for (unsigned j = rect.top; j < rect.top + rect.width; j++) {
			if (color) {
				image.setPixel(i, j, sf::Color::Black);
			}
			else {
				image.setPixel(i, j, sf::Color::White);
			}
		}
	}
}
