#include <SFML/Graphics.hpp>
#include <iostream>
#include <string> 
#include <sstream>

using namespace sf;

template <typename T>
std::string toString(T val) {
	std::ostringstream oss;
	oss << val;
	return oss.str();
}

class Field {
	static const int WIDTH = 70;
	static const int HEIGHT = 50;

	Vector2u _windowSize;

	Color _tempColor = Color(1, 1, 1);
	Color _landColor = Color(0, 168, 168);
	Color _seaColor = Color(0, 0, 0);
	Color _trackColor = Color(144, 18, 144);

public:
	RectangleShape _field[WIDTH][HEIGHT];

	int _distanceBetweenCells = 10;
	int _xOffset, _yOffset;

	Field(RenderWindow &renderWindow) {
		_windowSize = renderWindow.getSize();
		_xOffset = (_windowSize.x - WIDTH * _distanceBetweenCells) / 2;
		_yOffset = (_windowSize.y - HEIGHT * _distanceBetweenCells) / 2;
	};

	Vector2u GetWindowSize() {
		return _windowSize;
	}

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

	//void FillArea(int x, int y) {
	//	if (_field[(x - _xOffset) / 10][(y - _yOffset) / 10].getFillColor() != _seaColor ||
	//		_field[(x - _xOffset) / 10][(y - _yOffset) / 10].getFillColor() == _tempColor) {
	//		return;
	//	}

	//	_field[(x - _xOffset) / 10][(y - _yOffset) / 10].setFillColor(_tempColor);

	//	if (_field[(x + 10 - _xOffset) / 10][(y - _yOffset) / 10].getFillColor() == _seaColor) FillArea(x + 10, y);
	//	if (_field[(x - 10 - _xOffset) / 10][(y - _yOffset) / 10].getFillColor() == _seaColor) FillArea(x - 10, y);
	//	if (_field[(x - _xOffset) / 10][(y + 10 - _yOffset) / 10].getFillColor() == _seaColor) FillArea(x, y + 10);
	//	if (_field[(x - _xOffset) / 10][(y - 10 - _yOffset) / 10].getFillColor() == _seaColor) FillArea(x, y - 10);
	//}

	//void FillTrackArea() {
	//	//_currentSeaArea = 0;

	//	FillArea(_x, _y);

	//	for (int y = 0; y < GetHeight(); y += 10)
	//		for (int x = 0; x < GetWidth(); x += 10) {
	//			if (_field[x / 10][y / 10].getFillColor() == _trackColor || _field[x / 10][y / 10].getFillColor() == _seaColor) {
	//				_field[x / 10][y / 10].setFillColor(_landColor);
	//				//_score++;
	//			}
	//			if (_field[x / 10][y / 10].getFillColor() == _tempColor) {
	//				_field[x / 10][y / 10].setFillColor(_seaColor);
	//				//_currentSeaArea++;
	//			}
	//		}
	//}
};

//class SeaEnemy;

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
	//Xonix *_xonix;

public:
	SeaEnemy(Field *field/*, Xonix *xonix*/) {
		srand(time(NULL));
		_field = field;
		//_xonix = xonix;

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

		IsHitTrackOrXonix();

		_seaEnemy.setPosition(_x, _y);
	}

	int GetX() {
		return _x;
	}

	int GetY() {
		return _y;
	}

	float GetSeaPercent() {
		float seaArea = (_field->GetWidth() / 10 - 4) * (_field->GetHeight() / 10 - 4);
		float seaPercent = _currentSeaArea / seaArea * 100;
		if (seaPercent == 0)
			return 0;
		return 100 - seaPercent;
	}

	int GetScore() {
		return _score;
	}

	/*void FillArea(int x, int y) {
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
	}*/

	bool IsHitTrackOrXonix() {
		if (_field->_field[(_x + _dx - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10].getFillColor() == _trackColor)	return true;
		/*if (_x == _xonix->GetX() && _y == _xonix->GetY()) return true;
		if (_x + _dx == _xonix->GetX() && _y + _dy == _xonix->GetY()) return true;
		if (_x + _dx == _xonix->GetX() && _y == _xonix->GetY()) return true;
		if (_x == _xonix->GetX() && _y + _dy == _xonix->GetY()) return true;*/
		return false;
	}
};


class Xonix {
	int _x,
		_y,
		_direction;

	int _score,
		_currentSeaArea;

	bool _inSea;
	bool _isSelfCross;

	Color _landColor = Color(0, 168, 168);
	Color _trackColor = Color(144, 18, 144);
	Color _seaColor = Color(0, 0, 0);
	Color _tempColor = Color(1, 1, 1);

	Texture _xonixTexture;
	Sprite _xonix;
	String _pathToTexture = "Images/player_in_land.png";

	Field *_field;
	SeaEnemy *_seaEnemy;

public:
	Xonix(Field *field, SeaEnemy *seaEnemy) {
		_field = field;
		_seaEnemy = seaEnemy;
	};

	void Init() {
		_x = _field->_xOffset + (_field->GetWidth() / 2.);
		_y = _field->_yOffset;

		_inSea = false;

		_xonixTexture.loadFromFile(_pathToTexture);
		_xonix.setTexture(_xonixTexture);
		_xonix.setPosition(_x, _y);

		_direction = 0;
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
		int x, y;
		x = _x;
		y = _y;

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

		// Stop after filling
		if (_field->_field[(_x - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10]
			.getFillColor() == _field->GetLandColor() && _inSea) {
			_direction = 0;
		}
		// Track
		if (_field->_field[(x - _field->_xOffset) / 10][(y - _field->_yOffset) / 10]
			.getFillColor() == _field->GetSeaColor()) {

			_inSea = true;

			_field->_field[(x - _field->_xOffset) / 10][(y - _field->_yOffset) / 10]
				.setFillColor(_trackColor);

			_field->_field[(x - _field->_xOffset) / 10][(y - _field->_yOffset) / 10]
				.setSize(Vector2f(_field->_distanceBetweenCells, _field->_distanceBetweenCells));
		}

		if (_field->_field[(_x - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10].getFillColor() == _field->GetLandColor() && _inSea) {
			//fillAreaThread.launch();
			///////////////////////////////////////////////////
			FillTrackArea();
			///////////////////////////////////////////////////
			//_field->
			_inSea = false;
			//xonix->SetInSea(false);
		}

		// Self cross
		_isSelfCross = _field->_field[(_x - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10].getFillColor() == _trackColor;

		// Update position
		_xonix.setPosition(_x, _y);
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
		///////////////////////////////////////////////
		FillArea(_seaEnemy->GetX(), _seaEnemy->GetY());
		///////////////////////////////////////////////

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

	bool GetIsSelfCross() {
		return _isSelfCross;
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
	Xonix *_xonix;

	Color _seaColor = Color(0, 0, 0);
	Color _trackColor = Color(144, 18, 144);

public:
	LandEnemy(Field *field, Xonix *xonix) {
		_field = field;
		_xonix = xonix;

		_x = 1;
		_y = 1;
	}

	void Init() {
		srand(time(NULL));

		_dx = rand() % 2 == 0 ? -10 : 10;
		_dy = rand() % 2 == 0 ? -10 : 10;

		_x = (_field->GetWidth() / 2.) + _field->_xOffset;
		_y = _field->GetHeight() + _field->_yOffset;

		//std::cout << "_x: " << _x << "\t_y: " << _y << "\n";

		//_x = _field->_xOffset + (_field->GetWidth() / 2.);

		//while (true) {
		//	_x = rand() % (_field->GetWidth() / 10) + (_field->_xOffset / 10);
		//	_x *= 10;

		//	_y = rand() % (_field->GetHeight() / 10) + (_field->_yOffset / 10);
		//	_y *= 10;
		//	if (_x < (20 + _field->_xOffset) || _x > (_field->GetWidth() - 30 + _field->_xOffset) || _y < (20 + _field->_yOffset) /*|| _y > (_field->GetHeight() - 30 + _field->_yOffset)*/) {
		//		std::cout << "x:" << _x << "\t";
		//		std::cout << "y:" << _y << "\n";
		//		break;
		//	}
		//}

		_landEnemyTexture.loadFromFile(_pathToTexture);
		_landEnemy.setTexture(_landEnemyTexture);
		_landEnemy.setPosition(_x, _y);

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

		if (_field->_field[(_x + _dx - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10].getFillColor() == _field->GetSeaColor() || _field->_field[(_x + _dx - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10].getFillColor() == _trackColor) _dx = -_dx;
		if (_field->_field[(_x - _field->_xOffset) / 10][(_y + _dy - _field->_yOffset) / 10].getFillColor() == _field->GetSeaColor() || _field->_field[(_x - _field->_xOffset) / 10][(_y + _dy - _field->_yOffset) / 10].getFillColor() == _trackColor) _dy = -_dy;
	}

	void Move() {
		UpdateDirection();
		_x += _dx;
		_y += _dy;
		/*
		if (IsHitXonix()) {
		std::cout << "HIT\n";
		}*/

		_landEnemy.setPosition(_x, _y);
	}

	bool IsHitXonix() {
		//if (_x == _xonix->GetX() && _y == _xonix->GetY()) return true;
		if (_x + _dx == _xonix->GetX() && _y + _dy == _xonix->GetY()) return true;
		if (_x + _dx == _xonix->GetX() && _y == _xonix->GetY()) return true;
		if (_x == _xonix->GetX() && _y + _dy == _xonix->GetY()) return true;
		return false;
	}
};

class Info {
	Text _score;
	Text _lvl;
	Text _xn;
	Text _full;
	Font _font;
	String _pathToFont;
	Vector2i _position;
	Color _fontColor = Color(144, 18, 144);

	std::string _textScore;
	std::string _textLvl;
	std::string _textXn;
	std::string _textFull;

	Field *_field;
public:
	Info(Field *field) {
		_field = field;
	}

	void Init() {
		SetDefaulValue();
		//score += toString();

		_position.x = 50;
		_position.y = _field->GetWindowSize().y - 40;

		//float distanceX = 

		// init font
		_pathToFont = "Fonts/joystix-monospace.ttf";
		_font.loadFromFile(_pathToFont);

		// init score
		_score.setFont(_font);
		_score.setString(_textScore);
		_score.setCharacterSize(18);
		_score.setFillColor(Color::White);
		_score.setOutlineColor(_fontColor);
		_score.setOutlineThickness(2);
		_score.setPosition(_position.x, _position.y);
		_position.x += _field->GetWindowSize().x / 5;

		// init lvl
		_lvl.setFont(_font);
		_lvl.setString(_textLvl);
		_lvl.setCharacterSize(18);
		_lvl.setFillColor(Color::White);
		_lvl.setOutlineColor(_fontColor);
		_lvl.setOutlineThickness(2);
		_lvl.setPosition(_position.x, _position.y);
		_position.x += _field->GetWindowSize().x / 5;

		// init xn
		_xn.setFont(_font);
		_xn.setString(_textXn);
		_xn.setCharacterSize(18);
		_xn.setFillColor(Color::White);
		_xn.setOutlineColor(_fontColor);
		_xn.setOutlineThickness(2);
		_xn.setPosition(_position.x, _position.y);
		_position.x += _field->GetWindowSize().x / 5;

		// init full
		_full.setFont(_font);
		_full.setString(_textFull);
		_full.setCharacterSize(18);
		_full.setFillColor(Color::White);
		_full.setOutlineColor(_fontColor);
		_full.setOutlineThickness(2);
		_full.setPosition(_position.x, _position.y);
	}

	void SetDefaulValue() {
		_textScore = "Score: ";
		_textFull = "Full: ";
		_textLvl = "Lvl: ";
		_textXn = "Xn: ";
	}

	void Draw(RenderWindow &renderWindow) {
		renderWindow.draw(_score);
		renderWindow.draw(_lvl);
		renderWindow.draw(_xn);
		renderWindow.draw(_full);
	}

	void DrawScore(int score, RenderWindow &renderWindow) {
		std::string text = _textScore;
		text += toString(score);
		_score.setString(text);
		renderWindow.draw(_score);
	}

	void DrawFull(int full, RenderWindow &renderWindow) {
		std::string text = _textFull;
		text += toString(full);
		text += "%";
		_full.setString(text);
		renderWindow.draw(_full);
	}

	void DrawXn(int xn, RenderWindow &renderWindow) {
		std::string text = _textXn;
		text += toString(xn);
		_xn.setString(text);
		renderWindow.draw(_xn);
	}

	void DrawLevel(int level, RenderWindow &renderWindow) {
		std::string text = _textLvl;
		text += toString(level);
		_lvl.setString(text);
		renderWindow.draw(_lvl);
	}

	void Update() {
		//_score = _score
	}
};
enum GameStates {
	START_GAME,
	NEXT_LEVEL,
	PAUSE,
	PLAYING,
	GAME_OVER,
	LOST_LIVE,
	EMPTY_STATE
};

class StateManager {
	int _winLevelPercent = 60;
	int _level = 1;

	GameStates _gameStates = START_GAME;
	GameStates _prevState = EMPTY_STATE;

	Field* _field;
	Xonix* _xonix;
	SeaEnemy* _seaEnemy;
	LandEnemy *_landEnemy;

public:
	StateManager(Field *field, Xonix *xonix, SeaEnemy *seaEnemy, LandEnemy *landEnemy) {
		_field = field;
		_xonix = xonix;
		_seaEnemy = seaEnemy;
		_landEnemy = landEnemy;
	}

	GameStates GetState() {
		return _gameStates;
	}

	void SetState(GameStates state) {
		_gameStates = state;
	}

	void UpdateStates(RenderWindow &renderWindow) {
		// Next level
		if (_seaEnemy->GetSeaPercent() >= _winLevelPercent) {
			//_prevState = _gameStates;
			_gameStates = NEXT_LEVEL;
			_level++;

			_field->Init(renderWindow);
			_xonix->Init();
			_seaEnemy->Init();
			_landEnemy->Init();
			_xonix->FillTrackArea();
		}
		// Game over
		if (_xonix->GetIsSelfCross() || _seaEnemy->IsHitTrackOrXonix() || _landEnemy->IsHitXonix() && _gameStates != _prevState) {
			// Once enter to condition
			_gameStates = GAME_OVER;
			_prevState = _gameStates;
		}
	}

	int GetLevel() {
		return _level;
	}
};

int main() {
	RenderWindow window(VideoMode(800, 600), "Xonix");
	window.setFramerateLimit(30); // Temporary solution

	Field *field = new Field(window);
	SeaEnemy *seaEnemy = new SeaEnemy(field);
	Xonix *xonix = new Xonix(field, seaEnemy);
	LandEnemy *landEnemy = new LandEnemy(field, xonix);
	//Thread fillAreaThread(&SeaEnemy::FillTrackArea, seaEnemy);
	//Thread landEnemyPositionThread(&LandEnemy::Init, landEnemy);
	Info info(field);

	StateManager stateManager(field, xonix, seaEnemy, landEnemy);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
				window.close();

			xonix->SetDirection();
		}

		window.clear();

		// Update states
		stateManager.UpdateStates(window);

		// Init
		if (stateManager.GetState() == START_GAME) {
			stateManager.SetState(PLAYING);

			field->Init(window);
			xonix->Init();
			seaEnemy->Init();
			landEnemy->Init();
			info.Init();
		}


		// Draw
		info.DrawScore(seaEnemy->GetScore(), window);
		info.DrawFull(seaEnemy->GetSeaPercent(), window);
		info.DrawLevel(stateManager.GetLevel(), window);

		field->Draw(window);
		xonix->Draw(window);
		seaEnemy->Draw(window);
		landEnemy->Draw(window);

		// Move
		if (stateManager.GetState() != GAME_OVER && stateManager.GetState() != PAUSE) {
			xonix->Move(window);
			seaEnemy->Move();
			landEnemy->Move();
		}

		window.display();
	}

	return 0;
}


