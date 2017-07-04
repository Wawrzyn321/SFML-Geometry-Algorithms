#include "Compressor.h"

/*
WHITE FALSE
BLACK TRUE
*/


int main() {
	sf::RenderWindow window;
	window.create(sf::VideoMode(400, 300), "ICwQ", sf::Style::Default);
	window.close();

	const string filename = "guppy.jpg";

	Compressor c = Compressor(filename);
	c.preprocess();
	c.create();
	cout << "nodes number: "<<c.getNodesNumber() << endl;
	c.saveToFile("TREE");
	cout << "saved to file TREE.4tree" << endl;

	Compressor loader = Compressor(2);
	//loader.loadFromFile(&c);
	cout << "loading from TREE.4tree at resolution = " << 2 << endl;
	loader.loadFrom4TreeFile("TREE.4tree");
	loader.setResolution(4);
	loader.saveImage("ORIGINAL_" + filename);
	cout << "loaded image saved to file ORIGINAL_" << filename <<" with resolution 4"<< endl;

	system("pause");
	return 0;
}