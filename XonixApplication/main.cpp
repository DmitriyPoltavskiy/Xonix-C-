#include <SFML/Graphics.hpp>
#include <iostream>
#include <string> 
#include <sstream>
#include <ctime>

using namespace sf;

template <typename T>
std::string toString(T val) {
	std::ostringstream oss;
	oss << val;
	return oss.str();
}


class Timer {
	int _startTime,
		_endTime,
		_delay = 60;

public:
	void Init() {
		_startTime = clock() / 1000;
		_endTime = _startTime + _delay;
	}

	bool TimeIsOver() {
		if (clock() / 1000 > _endTime)
			return true;
		return false;
	}

	int CurrentTime() {
		return _endTime - clock() / 1000;
	}
};

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

	void ClearTrack() {
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {
				if (_field[x][y].getFillColor() == _trackColor) {
					_field[x][y].setFillColor(_seaColor);
				}
			}
		}
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

class SeaEnemy;

class Xonix {
	int _x,
		_y,
		_direction;

	int _score,
		_currentSeaArea,
		_countLives = 3;

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

	std::vector<SeaEnemy*>* _seaEnemies;
public:
	Xonix(Field*, std::vector<SeaEnemy*>*);

	void Init();

	void Draw(RenderWindow&);

	void SetDirection();

	void Move(RenderWindow&);

	void FillArea(int, int);

	void FillTrackArea();

	void ResetScore();

	void ResetSeaArea();

	void SetInSea(bool);

	void ResetCountLives(int);

	void DecreaseLive();

	void IncreaseLive();

	int GetX();

	int GetY();

	bool XonixInSea();

	bool GetIsSelfCross();

	float GetSeaPercent();

	int GetScore();

	int GetCountLives();
};

class SeaEnemy {
	int _dx,
		_dy,
		_x,
		_y;

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

	bool IsHitTrackOrXonix(Xonix *xonix) {
		if (_field->_field[(_x - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10].getFillColor() == _trackColor)	return true;
		if (_x + _dx == xonix->GetX() && _y + _dy == xonix->GetY() && 
			_field->_field[(xonix->GetX() - _field->_xOffset) / 10][(xonix->GetY() - _field->_yOffset) / 10]
				.getFillColor() != _landColor) {
			return true;
		}
		return false;
	}
};


Xonix::Xonix(Field *field, std::vector<SeaEnemy*>* seaEnemies) {
	_field = field;
	_seaEnemies = seaEnemies;
}

void Xonix::Init() {
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

void Xonix::Draw(RenderWindow &renderWindow) {
	renderWindow.draw(_xonix);
}

int Xonix::GetCountLives() {
	return _countLives;
}

void Xonix::ResetSeaArea() {
	_currentSeaArea = 0;
}

void Xonix::ResetScore() {
	_score = 0;
}

void Xonix::SetDirection() {
	if (Keyboard::isKeyPressed(Keyboard::Right))
		_direction = 1;
	if (Keyboard::isKeyPressed(Keyboard::Left))
		_direction = -1;
	if (Keyboard::isKeyPressed(Keyboard::Up))
		_direction = -2;
	if (Keyboard::isKeyPressed(Keyboard::Down))
		_direction = 2;
}

void Xonix::ResetCountLives(int count) {
	_countLives = count;
}

void Xonix::DecreaseLive() {
	_countLives--;
}

void Xonix::IncreaseLive() {
	_countLives++;
}

void Xonix::Move(RenderWindow &renderWindow) {
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
		FillTrackArea();
		_inSea = false;
	}

	// Self cross
	_isSelfCross = _field->_field[(_x - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10].getFillColor() == _trackColor;

	// Update position
	_xonix.setPosition(_x, _y);
}

void Xonix::FillArea(int x, int y) {
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

void Xonix::FillTrackArea() {
	_currentSeaArea = 0;
	for (int i = 0; i < _seaEnemies->size(); i++) {
		FillArea(_seaEnemies->at(i)->GetX(), _seaEnemies->at(i)->GetY());
	}

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

void Xonix::SetInSea(bool inSea) {
	_inSea = inSea;
}

int Xonix::GetX() {
	return _x;
}

int Xonix::GetY() {
	return _y;
}

bool Xonix::XonixInSea() {
	return _inSea;
}

bool Xonix::GetIsSelfCross() {
	return _isSelfCross;
}

float Xonix::GetSeaPercent() {
	float seaArea = (_field->GetWidth() / 10 - 4) * (_field->GetHeight() / 10 - 4);
	float seaPercent = _currentSeaArea / seaArea * 100;
	if (seaPercent == 0)
		return 0;
	return 100 - seaPercent;
}

int Xonix::GetScore() {
	return _score;
}

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

		_landEnemy.setPosition(_x, _y);
	}

	bool IsHitXonix() {
		if (_x == _xonix->GetX() && _y == _xonix->GetY()) return true;
		return false;
	}
};

class Info {
	Text _gameStart;
	Text _gameOver;
	Text _hint;
	Text _score;
	Text _lvl;
	Text _xn;
	Text _full;
	Text _time;
	Font _font;
	String _pathToFont;
	Vector2i _position;
	Color _fontColor = Color(144, 18, 144);

	std::string _textGameStart;
	std::string _textGameOver;
	std::string _textHint;
	std::string _textScore;
	std::string _textLvl;
	std::string _textXn;
	std::string _textFull;
	std::string _textTime;

	Field *_field;

	FloatRect gameStart;
	FloatRect gameOver;
	FloatRect hint;
	FloatRect scoreRect;
	FloatRect lvlRect;
	FloatRect xnRect;
	FloatRect fullRect;
	FloatRect timeRect;

public:
	Info(Field *field) {
		_field = field;
	}

	void Init() {
		SetDefaulValue();

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

		// init lvl
		_lvl.setFont(_font);
		_lvl.setString(_textLvl);
		_lvl.setCharacterSize(18);
		_lvl.setFillColor(Color::White);
		_lvl.setOutlineColor(_fontColor);
		_lvl.setOutlineThickness(2);

		// init xn
		_xn.setFont(_font);
		_xn.setString(_textXn);
		_xn.setCharacterSize(18);
		_xn.setFillColor(Color::White);
		_xn.setOutlineColor(_fontColor);
		_xn.setOutlineThickness(2);

		// init full
		_full.setFont(_font);
		_full.setString(_textFull);
		_full.setCharacterSize(18);
		_full.setFillColor(Color::White);
		_full.setOutlineColor(_fontColor);
		_full.setOutlineThickness(2);

		// init timer
		_time.setFont(_font);
		_time.setString(_textTime);
		_time.setCharacterSize(18);
		_time.setFillColor(Color::White);
		_time.setOutlineColor(_fontColor);
		_time.setOutlineThickness(2);

		// init game start
		_textGameStart = "Start Game";
		_gameStart.setFont(_font);
		_gameStart.setString(_textGameStart);
		_gameStart.setCharacterSize(55);
		_gameStart.setFillColor(Color::White);
		_gameStart.setOutlineColor(_fontColor);
		_gameStart.setOutlineThickness(4);
		gameStart = _gameStart.getLocalBounds();
		_gameStart.setOrigin(gameStart.width / 2, gameStart.height * 1.5);
		_gameStart.setPosition(_field->GetWindowSize().x / 2, _field->GetWindowSize().y / 2);

		// init game over
		_textGameOver = "Game Over";
		_gameOver.setFont(_font);
		_gameOver.setString(_textGameOver);
		_gameOver.setCharacterSize(55);
		_gameOver.setFillColor(Color::White);
		_gameOver.setOutlineColor(_fontColor);
		_gameOver.setOutlineThickness(4);
		gameOver = _gameOver.getLocalBounds();
		_gameOver.setOrigin(gameOver.width / 2, gameOver.height * 1.5);
		_gameOver.setPosition(_field->GetWindowSize().x / 2, _field->GetWindowSize().y / 2);

		// init hint
		_textHint = "Press 'space' for start";
		_hint.setFont(_font);
		_hint.setString(_textHint);
		_hint.setCharacterSize(18);
		_hint.setFillColor(Color::White);
		_hint.setOutlineColor(_fontColor);
		_hint.setOutlineThickness(2);
		hint = _hint.getLocalBounds();
		_hint.setOrigin(hint.width / 2, hint.height / 2);
		_hint.setPosition(_field->GetWindowSize().x / 2, (_field->GetWindowSize().y / 2) + hint.height);

	}

	void SetPosition(RenderWindow &renderWindow) {
		scoreRect = _score.getLocalBounds();
		lvlRect = _lvl.getLocalBounds();
		xnRect = _xn.getLocalBounds();
		fullRect = _full.getLocalBounds();
		timeRect = _time.getLocalBounds();

		float countTextElement = 5;

		float windowWidth = _field->GetWindowSize().x;
		float windowHeight = _field->GetWindowSize().y;
		float fieldWidth = _field->GetWidth();
		float fieldHeight = _field->GetHeight();

		float offsetX = (windowWidth - fieldWidth) / 2.;
		offsetX += scoreRect.width / 2.;

		float offsetY = (windowHeight - fieldWidth) / 2.;
		float positionTextY = windowHeight + (offsetY / 2.);

		float widthText = scoreRect.width + lvlRect.width + xnRect.width + fullRect.width + timeRect.width;
		std::cout << "widthText: " << widthText << "\n";

		float offsetText = (fieldWidth - widthText) / (countTextElement - 1);
		std::cout << "offsetText: " << offsetText << "\n\n";

		// <-- score
		float positionTextX = offsetX;
		_score.setPosition(positionTextX, positionTextY);
		// -->

		// <-- lvl
		positionTextX += (scoreRect.width / 2.) + offsetText + (lvlRect.width / 2.);
		_lvl.setPosition(positionTextX, positionTextY);
		// -->

		// <-- xn
		positionTextX += (lvlRect.width / 2.) + offsetText + (xnRect.width / 2.);
		_xn.setPosition(positionTextX, positionTextY);
		// -->

		// <-- xn
		positionTextX += (xnRect.width / 2.) + offsetText + (fullRect.width / 2.);
		_full.setPosition(positionTextX, positionTextY);
		// -->

		// <-- time
		positionTextX += (fullRect.width / 2.) + offsetText + (timeRect.width / 2.);
		_time.setPosition(positionTextX, positionTextY);
		// -->
	}

	void SetDefaulValue() {
		_textScore = "Score: ";
		_textFull = "Full: ";
		_textLvl = "Lvl: ";
		_textXn = "Xn: ";
		_textTime = "Time: ";
	}

	void DrawGameStart(RenderWindow &renderWindow) {
		renderWindow.draw(_gameStart);
	}

	void DrawGameOver(RenderWindow &renderWindow) {
		renderWindow.draw(_gameOver);
	}

	void DrawHint(RenderWindow &renderWindow) {
		renderWindow.draw(_hint);
	}

	void DrawScore(int score, RenderWindow &renderWindow) {
		std::string text = _textScore;
		text += toString(score);
		_score.setString(text);
		scoreRect = _score.getLocalBounds();
		_score.setOrigin(scoreRect.width / 2, scoreRect.height / 2);
		renderWindow.draw(_score);
	}

	void DrawFull(int full, RenderWindow &renderWindow) {
		std::string text = _textFull;
		text += toString(full);
		text += "%";
		_full.setString(text);
		scoreRect = _full.getLocalBounds();
		_full.setOrigin(scoreRect.width / 2, scoreRect.height / 2);
		renderWindow.draw(_full);
	}

	void DrawXn(int xn, RenderWindow &renderWindow) {
		std::string text = _textXn;
		text += toString(xn);
		_xn.setString(text);
		scoreRect = _xn.getLocalBounds();
		_xn.setOrigin(scoreRect.width / 2, scoreRect.height / 2);
		renderWindow.draw(_xn);
	}

	void DrawLevel(int level, RenderWindow &renderWindow) {
		std::string text = _textLvl;
		text += toString(level);
		_lvl.setString(text);
		scoreRect = _lvl.getLocalBounds();
		_lvl.setOrigin(scoreRect.width / 2, scoreRect.height / 2);
		renderWindow.draw(_lvl);
	}

	void DrawTime(int time, RenderWindow &renderWindow) {
		std::string text = _textTime;
		text += toString(time);
		_time.setString(text);
		scoreRect = _time.getLocalBounds();
		_time.setOrigin(scoreRect.width / 2, scoreRect.height / 2);
		renderWindow.draw(_time);
	}
};

enum GameStates {
	INIT_DEPENDENCIES,
	START_GAME,
	NEXT_LEVEL,
	PAUSE,
	PLAYING,
	LOST_LIVE,
	GAME_OVER,
	EMPTY_STATE
};

class StateManager {
	int _winLevelPercent = 52;
	int _level = 1;

	GameStates _gameStates;
	GameStates _prevState = EMPTY_STATE;

	Field *_field;
	Xonix *_xonix;
	LandEnemy *_landEnemy;
	Timer *_timer;

	std::vector<SeaEnemy*>* _seaEnemies;

public:
	StateManager(Field *field, Xonix *xonix, std::vector<SeaEnemy*> *seaEnemies, LandEnemy *landEnemy, Timer *timer) {
		_field = field;
		_xonix = xonix;
		_seaEnemies = seaEnemies;
		_landEnemy = landEnemy;
		_timer = timer;
	}

	GameStates GetState() {
		return _gameStates;
	}

	void ResetLevel() {
		_level = 1;
	}

	void SetState(GameStates state) {
		_gameStates = state;
	}

	bool SeaEnemiesIsHitTrackOrXonix() {
		for (int i = 0; i < _seaEnemies->size(); i++) 
			if (_seaEnemies->at(i)->IsHitTrackOrXonix(_xonix))
				return true;
		return false;
	}

	void UpdateStates(RenderWindow &renderWindow) {
		// Next level
		if (_xonix->GetSeaPercent() >= _winLevelPercent) {
			_level++;
			_xonix->IncreaseLive();
			_xonix->Init();
			_field->Init(renderWindow);
			_landEnemy->Init();

			for (int i = 0; i < _seaEnemies->size(); i++) {
				_seaEnemies->at(i)->Init();
			}

			_xonix->FillTrackArea();
			_gameStates = NEXT_LEVEL;
		}
		// Game over
		if (_xonix->GetIsSelfCross() ||
			SeaEnemiesIsHitTrackOrXonix() ||
			_timer->TimeIsOver() ||
			_landEnemy->IsHitXonix() &&
			_xonix->GetCountLives() >= 1 &&
			_gameStates != _prevState) {

			// Once enter to condition
			_gameStates = LOST_LIVE;
			_prevState = _gameStates;

			_xonix->DecreaseLive();
			_timer->Init();
		}
		if (_xonix->GetCountLives() <= 0 && _gameStates != _prevState) {
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

	std::vector<SeaEnemy*> seaEnemies;
	seaEnemies.push_back(new SeaEnemy(field));
	
	Xonix *xonix = new Xonix(field, &seaEnemies);
	LandEnemy *landEnemy = new LandEnemy(field, xonix);
	
	Timer *timer = new Timer();
	Info info(field);
	StateManager stateManager(field, xonix, &seaEnemies, landEnemy, timer);
	stateManager.SetState(INIT_DEPENDENCIES);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
				window.close();

			if (stateManager.GetState() == PLAYING)
				xonix->SetDirection();

			if (Keyboard::isKeyPressed(Keyboard::Space) && stateManager.GetState() == START_GAME)
				stateManager.SetState(PLAYING);

			if (Keyboard::isKeyPressed(Keyboard::Space) && stateManager.GetState() == GAME_OVER) {
				field->Init(window);

				for (int i = 0; i < seaEnemies.size(); i++) {
					seaEnemies[i]->Init();
				}

				landEnemy->Init();
				stateManager.SetState(PLAYING);
			}

		}

		window.clear();

		// Update states
		stateManager.UpdateStates(window);

		// Init
		if (stateManager.GetState() == INIT_DEPENDENCIES) {
			field->Init(window);
			xonix->Init();
			landEnemy->Init();
			info.Init();
			timer->Init();

			for (int i = 0; i < seaEnemies.size(); i++) {
				seaEnemies[i]->Init();
			}

			stateManager.SetState(START_GAME);
		}

		// Draw entities
		if (stateManager.GetState() != START_GAME &&
			stateManager.GetState() != INIT_DEPENDENCIES &&
			stateManager.GetState() != GAME_OVER) {
			field->Draw(window);
			xonix->Draw(window);
			landEnemy->Draw(window);

			for (int i = 0; i < seaEnemies.size(); i++) {
				seaEnemies[i]->Draw(window);
			}
		}

		// Start game
		if (stateManager.GetState() == START_GAME) {
			info.DrawGameStart(window);
			info.DrawHint(window);
		}

		// Draw text info
		if (stateManager.GetState() != START_GAME &&
			stateManager.GetState() != INIT_DEPENDENCIES &&
			stateManager.GetState() != GAME_OVER) {
			info.DrawScore(xonix->GetScore(), window);
			info.DrawFull(xonix->GetSeaPercent(), window);
			info.DrawXn(xonix->GetCountLives(), window);
			info.DrawLevel(stateManager.GetLevel(), window);
			info.DrawTime(timer->CurrentTime(), window);
			info.SetPosition(window);
		}

		// Lost live
		if (stateManager.GetState() == LOST_LIVE) {
			xonix->Init();
			field->ClearTrack();
			landEnemy->Init();
			sleep(milliseconds(1000));
			stateManager.SetState(PLAYING);
		}

		// Move
		if (stateManager.GetState() != START_GAME &&
			stateManager.GetState() != INIT_DEPENDENCIES &&
			stateManager.GetState() != GAME_OVER) {

			xonix->Move(window);
			landEnemy->Move();

			for (int i = 0; i < seaEnemies.size(); i++) {
				seaEnemies[i]->Move();
			}
		}

		// Next level
		if (stateManager.GetState() == NEXT_LEVEL) {
			seaEnemies.push_back(new SeaEnemy(field));
			timer->Init();
			for (int i = seaEnemies.size() - 1; i < seaEnemies.size(); i++) {
				seaEnemies[i]->Init();
			}
			stateManager.SetState(PLAYING);
		}

		// Game Over
		if (stateManager.GetState() == GAME_OVER) {
			info.DrawGameOver(window);
			info.DrawHint(window);
			xonix->ResetCountLives(3);
			xonix->ResetScore();
			xonix->ResetSeaArea();

			seaEnemies.clear();
			seaEnemies.push_back(new SeaEnemy(field));

			timer->Init();

			stateManager.ResetLevel();
		}
		window.display();
	}

	return 0;
}