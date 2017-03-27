#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class Field {
	static const int WIDTH = 80;
	static const int HEIGHT = 60;

	Vector2u _windowSize;

	Color _landColor = Color(0, 168, 168);
	Color _seaColor = Color(0, 0, 0);
public:
	RectangleShape _field[WIDTH * HEIGHT];

	int _distanceBetweenCells = 10;
	int _xOffset, _yOffset;

	Field() {}

	Field(RenderWindow &renderWindow) {
		_windowSize = renderWindow.getSize();
		_xOffset = (_windowSize.x - WIDTH * _distanceBetweenCells) / 2;
		_yOffset = (_windowSize.y - HEIGHT * _distanceBetweenCells) / 2;
	};

	int GetWidth() {
		return WIDTH * _distanceBetweenCells;
	}

	int GetHeight() {
		return HEIGHT * _distanceBetweenCells;
	}

	Color GetLandColor() {
		return _landColor;
	}

	Color GetSeaColor() {
		return _seaColor;
	}

	void DrawField(RenderWindow &renderWindow) {
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {

				_field[x * HEIGHT + y].setSize(Vector2f(_distanceBetweenCells, _distanceBetweenCells));

				if (x < 2 || x > WIDTH - 3 || y < 2 || y > HEIGHT - 3)
					_field[x * HEIGHT + y].setFillColor(_landColor);
				else
					_field[x * HEIGHT + y].setFillColor(_seaColor);

				_field[x * HEIGHT + y]
					.setPosition(
						Vector2f(x * _distanceBetweenCells + _xOffset, 
								 y * _distanceBetweenCells + _yOffset)
					);


				renderWindow.draw(_field[x * HEIGHT + y]);
			}
		}
	}
};

class Xonix {
	int _x,
		_y,
		_direction;

	bool _inSea,
		_inLand;

	Color _trackColor = Color(144, 18, 144);

	Texture _xonixTexture;
	Sprite _xonix;
	String _pathToTexture = "Images/player_in_land.png";

	Field *_field;
	RenderWindow _window;

public:
	Xonix(Field *field) {
		_x = field->_xOffset + (field->GetWidth() / 2.);
		_y = field->_yOffset;

		_inSea = false;
		_inLand = true;

		_xonixTexture.loadFromFile(_pathToTexture);
		_xonix.setTexture(_xonixTexture);
		_xonix.setPosition(_x, _y);

		_field = field;

		_direction = 0;
	};

	void InitXonix(RenderWindow &renderWindow) {
		_xonix.setScale(_field->_distanceBetweenCells / (float)_xonixTexture.getSize().x, _field->_distanceBetweenCells / (float)_xonixTexture.getSize().x);
		renderWindow.draw(_xonix);
	}

	void SetDirection() {
		if (Keyboard::isKeyPressed(Keyboard::Right))
			_direction = 1;
		if (Keyboard::isKeyPressed(Keyboard::Left))
			_direction = -1;
		if (Keyboard::isKeyPressed(Keyboard::Up))
			_direction = -2;
		if (Keyboard::isKeyPressed(Keyboard::Down))
			_direction = 2;
	}

	void Move(RenderWindow &renderWindow) {
		// Calculate position
		switch (_direction) {
		case 1:
			_x += _field->_distanceBetweenCells;
			break;
		case -1:
			_x -= _field->_distanceBetweenCells;
			break;
		case 2:
			_y += _field->_distanceBetweenCells;
			break;
		case -2:
			_y -= _field->_distanceBetweenCells;
			break;
		default:
			break;
		}

		// Fixed borders
		if (_x < _field->_xOffset) {
			_direction = 0;
			_x = _field->_xOffset;
		}
		if (_x > _field->_xOffset + _field->GetWidth() - _field->_distanceBetweenCells) {
			_direction = 0;
			_x = _field->_xOffset + _field->GetWidth() - _field->_distanceBetweenCells;
		}
		if (_y < _field->_yOffset) {
			_direction = 0;
			_y = _field->_yOffset;
		}
		if (_y > _field->_yOffset + _field->GetHeight() - _field->_distanceBetweenCells) {
			_direction = 0;
			_y = _field->_yOffset + _field->GetHeight() - _field->_distanceBetweenCells;
		}

		// Update position
		_xonix.setPosition(_x, _y);
		std::cout << "\n_x:  " << _x;
		std::cout << "\t_y:  " << _y;


		if (_field->_field[(_x / 10) * 60 + (_y / 10)].getFillColor() == _field->GetSeaColor()) {
			_field->_field[(_x / 10) * 60 + (_y / 10)].setFillColor(_trackColor);
			_field->_field[(_x / 10) * 60 + (_y / 10)].setSize(Vector2f(_field->_distanceBetweenCells, _field->_distanceBetweenCells));
			renderWindow.draw(_field->_field[(_x / 10) * 60 + (_y / 10)]);
		}
	}
};


int main() {
	RenderWindow window(VideoMode(800, 600), "Xonix");
	window.setFramerateLimit(30); // Temporary solution

	Field *field = new Field(window);
	Xonix xonix(field);
	
	/*
	#include <vector>*/

	while (window.isOpen())
	{
		/*std::vector<Xonix*> dd;
		dd.push_back(&xonix);
*/
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
				window.close();
			
			xonix.SetDirection();
		}


		window.clear();


		field->DrawField(window);

		xonix.InitXonix(window);
		xonix.Move(window);

		window.display();
	}

	return 0;
}
