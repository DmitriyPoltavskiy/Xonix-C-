#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

class Field {
	static const int WIDTH = 70;
	static const int HEIGHT = 50;

	Vector2u _windowSize;

	Color _landColor = Color(0, 168, 168);
	Color _seaColor = Color(0, 0, 0);
public:
	RectangleShape _field[WIDTH][HEIGHT];

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

	void Init(RenderWindow &renderWindow) {
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {

				_field[x][y].setSize(Vector2f(_distanceBetweenCells, _distanceBetweenCells));

				if (x < 2 || x > WIDTH - 3 || y < 2 || y > HEIGHT - 3)
					_field[x][y].setFillColor(_landColor);
				else
					_field[x][y].setFillColor(_seaColor);

				_field[x][y]
					.setPosition(
						Vector2f(x * _distanceBetweenCells + _xOffset, 
								 y * _distanceBetweenCells + _yOffset)
					);
				renderWindow.draw(_field[x][y]);
			}
		}
	}

	void Draw(RenderWindow &renderWindow) {
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {
				renderWindow.draw(_field[x][y]);
			}
		}
	}
};

class Xonix {
	int _x,
		_y,
		_direction;

	bool _inSea;

	Color _trackColor = Color(144, 18, 144);

	Texture _xonixTexture;
	Sprite _xonix;
	String _pathToTexture = "Images/player_in_land.png";

	Field *_field;

public:
	Xonix(Field *field) {
		_field = field;
		
		_x = _field->_xOffset + (field->GetWidth() / 2.);
		_y = _field->_yOffset;

		_inSea = false;

		_xonixTexture.loadFromFile(_pathToTexture);
		_xonix.setTexture(_xonixTexture);
		_xonix.setPosition(_x, _y);

		_direction = 0;
	};

	void Init() {
		_xonix.setScale(_field->_distanceBetweenCells / (float)_xonixTexture.getSize().x, 
						_field->_distanceBetweenCells / (float)_xonixTexture.getSize().y);
	}

	void Draw(RenderWindow &renderWindow) {
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
		if (_direction == 1) _x += _field->_distanceBetweenCells;
		if (_direction == -1) _x -= _field->_distanceBetweenCells;
		if (_direction == 2) _y += _field->_distanceBetweenCells;
		if (_direction == -2) _y -= _field->_distanceBetweenCells;

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

		// Track
		if (_field->_field[(_x - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10]
			.getFillColor() == _field->GetLandColor() && _inSea) {
			_direction = 0;
		}
		if (_field->_field[(_x - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10]
			.getFillColor() == _field->GetSeaColor()) {

			_inSea = true;

			_field->_field[(_x - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10]
				.setFillColor(_trackColor);

			_field->_field[(_x - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10]
				.setSize(Vector2f(_field->_distanceBetweenCells, _field->_distanceBetweenCells));
		}

		// Update position
		_xonix.setPosition(_x, _y);
	}

	void SetInSea(bool inSea) {
		_inSea = inSea;
	}

	int GetX() {
		return _x;
	}

	int GetY() {
		return _y;
	}

	bool XonixInSea() {
		return _inSea;
	}
};

class SeaEnemy {
	int _dx,
		_dy,
		_x,
		_y;

	int _score,
		_currentSeaArea;

	Texture _seaEnemyTexture;
	Sprite _seaEnemy;
	String _pathToTexture = "Images/enemy_in_sea.png";

	Color _landColor = Color(0, 168, 168);
	Color _trackColor = Color(144, 18, 144);
	Color _seaColor = Color(0, 0, 0);
	Color _tempColor = Color(1, 1, 1);

	Field *_field;

public:
	SeaEnemy(Field *field) {
		srand(time(NULL));
		_field = field;

		_dx = rand() % 2 == 0 ? -10 : 10;
		_dy = rand() % 2 == 0 ? -10 : 10;

		_x = ((2 + rand() % (_field->GetWidth() / 10 - 3)) * 10) + _field->_xOffset;
		_y = ((2 + rand() % (_field->GetHeight() / 10 - 3)) * 10) + _field->_yOffset;

		_seaEnemyTexture.loadFromFile(_pathToTexture);
		_seaEnemy.setTexture(_seaEnemyTexture);
		_seaEnemy.setPosition(_x, _y);
	}

	void Init() {
		_seaEnemy.setScale(_field->_distanceBetweenCells / (float)_seaEnemyTexture.getSize().x, 
						   _field->_distanceBetweenCells / (float)_seaEnemyTexture.getSize().y);
	}

	void Draw(RenderWindow &renderWindow) {
		renderWindow.draw(_seaEnemy);
	}

	void UpdateDirection() {
		if (_field->_field[(_x + _dx - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10]
			.getFillColor() == _field->GetLandColor()) _dx = -_dx;
		if (_field->_field[(_x - _field->_xOffset) / 10][(_y + _dy - _field->_yOffset) / 10]
			.getFillColor() == _field->GetLandColor()) _dy = -_dy;
	}

	void Move() {
		UpdateDirection();
		_x += _dx;
		_y += _dy;

		_seaEnemy.setPosition(_x, _y);
	}

	int GetX() {
		return _x;
	}

	int GetY() {
		return _y;
	}

	void FillArea(int x, int y) {
		if (_field->_field[(x - _field->_xOffset) / 10][(y - _field->_yOffset) / 10].getFillColor() != _seaColor ||
			_field->_field[(x - _field->_xOffset) / 10][(y - _field->_yOffset) / 10].getFillColor() == _tempColor) {
			return;
		}

		_field->_field[(x - _field->_xOffset) / 10][(y - _field->_yOffset) / 10].setFillColor(_tempColor);

		if (_field->_field[(x + 10 - _field->_xOffset) / 10][(y - _field->_yOffset) / 10].getFillColor() == _seaColor) FillArea(x + 10, y);
		if (_field->_field[(x - 10 - _field->_xOffset) / 10][(y - _field->_yOffset) / 10].getFillColor() == _seaColor) FillArea(x - 10, y);
		if (_field->_field[(x - _field->_xOffset) / 10][(y + 10 - _field->_yOffset) / 10].getFillColor() == _seaColor) FillArea(x, y + 10);
		if (_field->_field[(x - _field->_xOffset) / 10][(y - 10 - _field->_yOffset) / 10].getFillColor() == _seaColor) FillArea(x, y - 10);
	}

	void FillTrackArea() {
		_currentSeaArea = 0;

		FillArea(_x, _y);

		for (int y = 0; y < _field->GetHeight(); y += 10)
			for (int x = 0; x < _field->GetWidth(); x += 10) {
				if (_field->_field[x / 10][y / 10].getFillColor() == _trackColor || _field->_field[x / 10][y / 10].getFillColor() == _seaColor) {
					_field->_field[x / 10][y / 10].setFillColor(_landColor);
					_score++;
				}
				if (_field->_field[x / 10][y / 10].getFillColor() == _tempColor) {
					_field->_field[x / 10][y / 10].setFillColor(_seaColor);
					_currentSeaArea++;
				}
			}
	}
};

class LandEnemy {
	int _dx,
		_dy,
		_x,
		_y;

	Texture _landEnemyTexture;
	Sprite _landEnemy;
	String _pathToTexture = "Images/enemy_in_land.png";

	Field *_field;

	Color _trackColor = Color(144, 18, 144);

public:
	LandEnemy(Field *field) {
		srand(time(NULL));
		_field = field;

		_dx = rand() % 2 == 0 ? -10 : 10;
		_dy = -10;

		do {
			_x = ((0 + rand() % (_field->GetWidth() / 10 - 1)) * 10) + _field->_xOffset;
			_y = ((((_field->GetHeight() / 10) - 3) + rand() % ((_field->GetHeight() / 10) - 1)) * 10) + _field->_yOffset;
		}
		while (_field->_field[(_x - _field->_xOffset) / 10, (_y - _field->_yOffset) / 10]->getFillColor() == _field->GetSeaColor());


		_landEnemyTexture.loadFromFile(_pathToTexture);
		_landEnemy.setTexture(_landEnemyTexture);
		_landEnemy.setPosition(_x, _y);
	}

	void Init() {
		_landEnemy.setScale(_field->_distanceBetweenCells / (float)_landEnemyTexture.getSize().x,
			_field->_distanceBetweenCells / (float)_landEnemyTexture.getSize().y);
	}

	void Draw(RenderWindow &renderWindow) {
		renderWindow.draw(_landEnemy);
	}

	void UpdateDirection() {
		if (_x < _field->_xOffset) _x = _field->_xOffset;
		if (_x > _field->_xOffset + _field->GetWidth() - _field->_distanceBetweenCells) 
			_x = _field->_xOffset + _field->GetWidth() - _field->_distanceBetweenCells;
		if (_y < _field->_yOffset) _y = _field->_yOffset;
		if (_y > _field->_yOffset + _field->GetHeight() - _field->_distanceBetweenCells)
			_y = _field->_yOffset + _field->GetHeight() - _field->_distanceBetweenCells;

		if (_x <= _field->_xOffset || _x >= (_field->GetWidth() - 10 + _field->_xOffset)) _dx = -_dx;
		if (_y <= +_field->_yOffset || _y >= (_field->GetHeight() - 10 + _field->_yOffset)) _dy = -_dy;

		if (_field->_field[(_x + _dx - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10].getFillColor() == _field->GetSeaColor()/* || _field->_field[(_x + _dx - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10].getFillColor() == _trackColor*/) _dx = -_dx;
		if (_field->_field[(_x - _field->_xOffset) / 10][(_y + _dy - _field->_yOffset) / 10].getFillColor() == _field->GetSeaColor()/* || _field->_field[(_x - _field->_xOffset) / 10][(_y + _dy - _field->_yOffset) / 10].getFillColor() == _trackColor*/) _dy = -_dy;
	}

	void Move() {
		UpdateDirection();
		_x += _dx;
		_y += _dy;

		_landEnemy.setPosition(_x, _y);
	}
};


int main() {
	RenderWindow window(VideoMode(800, 600), "Xonix");
	window.setFramerateLimit(30); // Temporary solution

	Field *field = new Field(window);
	Xonix xonix(field);
	SeaEnemy seaEnemy(field);
	LandEnemy landEnemy(field);
	
	bool start = false;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
				window.close();
			
			xonix.SetDirection();
		}


		window.clear();

		// Init
		if (!start) {
			start = true;
			field->Init(window);
			xonix.Init();
			seaEnemy.Init();
			landEnemy.Init();
		}

		// Conditions
		if (field->_field[(xonix.GetX() - field->_xOffset) / 10][(xonix.GetY() - field->_yOffset) / 10].getFillColor() == field->GetLandColor() && xonix.XonixInSea()) {
			seaEnemy.FillTrackArea();
			xonix.SetInSea(false);
		}

		// Draw
		field->Draw(window);
		xonix.Draw(window);
		seaEnemy.Draw(window);
		landEnemy.Draw(window);

		// Move
		xonix.Move(window);
		seaEnemy.Move();
		landEnemy.Move();

		window.display();
	}

	return 0;
}
