#include <SFML/Graphics.hpp>
#include <iostream>
#include <string> 
#include <sstream>

//using namespace sf;


template <typename T>
std::string toString(T val) {
	std::ostringstream oss;
	oss << val;
	return oss.str();
}

class Field {
	static const int WIDTH = 70;
	static const int HEIGHT = 50;

	sf::Vector2u _windowSize;

	sf::Color _tempColor = sf::Color(1, 1, 1);
	sf::Color _landColor = sf::Color(0, 168, 168);
	sf::Color _seaColor = sf::Color(0, 0, 0);
	sf::Color _trackColor = sf::Color(144, 18, 144);

public:
	sf::RectangleShape _field[WIDTH][HEIGHT];

	int _distanceBetweenCells = 10;
	int _xOffset, _yOffset;

	Field(sf::RenderWindow &renderWindow) {
		_windowSize = renderWindow.getSize();
		_xOffset = (_windowSize.x - WIDTH * _distanceBetweenCells) / 2;
		_yOffset = (_windowSize.y - HEIGHT * _distanceBetweenCells) / 2;
	};

	sf::Vector2u GetWindowSize() {
		return _windowSize;
	}

	int GetWidth() {
		return WIDTH * _distanceBetweenCells;
	}

	int GetHeight() {
		return HEIGHT * _distanceBetweenCells;
	}

	sf::Color GetLandColor() {
		return _landColor;
	}

	sf::Color GetSeaColor() {
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

	void Init(sf::RenderWindow &renderWindow) {
		for (int x = 0; x < WIDTH; x++) {
			for (int y = 0; y < HEIGHT; y++) {
				_field[x][y].setSize(sf::Vector2f(_distanceBetweenCells, _distanceBetweenCells));

				if (x < 2 || x > WIDTH - 3 || y < 2 || y > HEIGHT - 3)
					_field[x][y].setFillColor(_landColor);
				else
					_field[x][y].setFillColor(_seaColor);

				_field[x][y]
					.setPosition(
						sf::Vector2f(x * _distanceBetweenCells + _xOffset,
							y * _distanceBetweenCells + _yOffset)
					);
			}
		}
	}

	void Draw(sf::RenderWindow &renderWindow) {
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

	sf::Color _landColor = sf::Color(0, 168, 168);
	sf::Color _trackColor = sf::Color(144, 18, 144);
	sf::Color _seaColor = sf::Color(0, 0, 0);
	sf::Color _tempColor = sf::Color(1, 1, 1);

	sf::Texture _xonixTexture;
	sf::Sprite _xonix;
	sf::String _pathToTexture = "Images/player_in_land.png";

	Field *_field;
	SeaEnemy *_seaEnemy;
	std::vector<SeaEnemy*> _seaEnemies;
public:
	Xonix(Field*, std::vector<SeaEnemy*>*);

	void Init();

	void Draw(sf::RenderWindow&);

	void SetDirection();

	void Move(sf::RenderWindow&);

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

	sf::Texture _seaEnemyTexture;
	sf::Sprite _seaEnemy;
	sf::String _pathToTexture = "Images/enemy_in_sea.png";

	sf::Color _landColor = sf::Color(0, 168, 168);
	sf::Color _trackColor = sf::Color(144, 18, 144);
	sf::Color _seaColor = sf::Color(0, 0, 0);
	sf::Color _tempColor = sf::Color(1, 1, 1);

	Field *_field;
	Xonix *_xonix;

	std::vector<SeaEnemy*>* _seaEnemies;

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

	void InitSeaEnemies(std::vector<SeaEnemy*>* seaEnemies) {
		_seaEnemies = seaEnemies;
	}

	void Draw(sf::RenderWindow &renderWindow) {
		//for (int i = 0; i < _seaEnemies->size(); i++) {
			//renderWindow.draw(_seaEnemies->at(i)->_seaEnemy);
		//}
	}

	void UpdateDirection() {
		std::cout << _y << "\n";
		if (_field->_field[(_x + _dx - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10]
			.getFillColor() == _landColor) _dx = -_dx;
		if (_field->_field[(_x - _field->_xOffset) / 10][(_y + _dy - _field->_yOffset) / 10]
			.getFillColor() == _landColor) _dy = -_dy;
	}

	//void InitSeaEnemies(std::vector<SeaEnemy*> seaEnemies) {
		//_seaEnemies = seaEnemies;
	//}

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

	void SetXonix(Xonix *xonix) {
		_xonix = xonix;
	}

	bool IsHitTrackOrXonix() {
		if (_field->_field[(_x - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10].getFillColor() == _trackColor)	return true;
		if (_x + _dx == _xonix->GetX() && _y + _dy == _xonix->GetY()) return true;
		return false;
	}

	bool EnemiesHitTrackOrXonix() {
		for (int i = 0; i < _seaEnemies->size(); i++)
			if (_seaEnemies->at(i)->IsHitTrackOrXonix())
				return true;
		return false;
	}
};


Xonix::Xonix(Field *field, std::vector<SeaEnemy*>* seaEnemies) {
	_field = field;
	_seaEnemies = *seaEnemies;
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

void Xonix::Draw(sf::RenderWindow &renderWindow) {
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		_direction = 1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		_direction = -1;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		_direction = -2;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
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

void Xonix::Move(sf::RenderWindow &renderWindow) {
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
		.getFillColor() == _landColor && _inSea) {
		_direction = 0;
	}
	// Track
	if (_field->_field[(x - _field->_xOffset) / 10][(y - _field->_yOffset) / 10]
		.getFillColor() == _field->GetSeaColor()) {

		_inSea = true;

		_field->_field[(x - _field->_xOffset) / 10][(y - _field->_yOffset) / 10]
			.setFillColor(_trackColor);

		_field->_field[(x - _field->_xOffset) / 10][(y - _field->_yOffset) / 10]
			.setSize(sf::Vector2f(_field->_distanceBetweenCells, _field->_distanceBetweenCells));
	}

	if (_field->_field[(_x - _field->_xOffset) / 10][(_y - _field->_yOffset) / 10].getFillColor() == _landColor && _inSea) {
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

	for (int i = 0; i < _seaEnemies.size(); i++) {
		FillArea(_seaEnemies.at(i)->GetX(), _seaEnemies.at(i)->GetY());
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
	float seaArea = (_field->GetWidth() / 6.) * (_field->GetHeight() / 6.);
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

	sf::Texture _landEnemyTexture;
	sf::Sprite _landEnemy;
	sf::String _pathToTexture = "Images/enemy_in_land.png";

	Field *_field;
	Xonix *_xonix;

	sf::Color _seaColor = sf::Color(0, 0, 0);
	sf::Color _trackColor = sf::Color(144, 18, 144);

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

	void Draw(sf::RenderWindow &renderWindow) {
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
	sf::Text _gameStart;
	sf::Text _gameOver;
	sf::Text _hint;
	sf::Text _score;
	sf::Text _lvl;
	sf::Text _xn;
	sf::Text _full;
	sf::Font _font;
	sf::String _pathToFont;
	sf::Vector2i _position;
	sf::Color _fontColor = sf::Color(144, 18, 144);

	std::string _textGameStart;
	std::string _textGameOver;
	std::string _textHint;
	std::string _textScore;
	std::string _textLvl;
	std::string _textXn;
	std::string _textFull;

	Field *_field;

	sf::FloatRect gameStart;
	sf::FloatRect gameOver;
	sf::FloatRect hint;
	sf::FloatRect scoreRect;
	sf::FloatRect lvlRect;
	sf::FloatRect xnRect;
	sf::FloatRect fullRect;

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
		_score.setFillColor(sf::Color::White);
		_score.setOutlineColor(_fontColor);
		_score.setOutlineThickness(2);

		// init lvl
		_lvl.setFont(_font);
		_lvl.setString(_textLvl);
		_lvl.setCharacterSize(18);
		_lvl.setFillColor(sf::Color::White);
		_lvl.setOutlineColor(_fontColor);
		_lvl.setOutlineThickness(2);

		// init xn
		_xn.setFont(_font);
		_xn.setString(_textXn);
		_xn.setCharacterSize(18);
		_xn.setFillColor(sf::Color::White);
		_xn.setOutlineColor(_fontColor);
		_xn.setOutlineThickness(2);

		// init full
		_full.setFont(_font);
		_full.setString(_textFull);
		_full.setCharacterSize(18);
		_full.setFillColor(sf::Color::White);
		_full.setOutlineColor(_fontColor);
		_full.setOutlineThickness(2);

		// init game start
		_textGameStart = "Start Game";
		_gameStart.setFont(_font);
		_gameStart.setString(_textGameStart);
		_gameStart.setCharacterSize(55);
		_gameStart.setFillColor(sf::Color::White);
		_gameStart.setOutlineColor(_fontColor);
		_gameStart.setOutlineThickness(4);
		gameStart = _gameStart.getLocalBounds();
		_gameStart.setOrigin(gameStart.width / 2.f, gameStart.height * 1.5f);
		_gameStart.setPosition(_field->GetWindowSize().x / 2.f, _field->GetWindowSize().y / 2.f);

		// init game over
		_textGameOver = "Game Over";
		_gameOver.setFont(_font);
		_gameOver.setString(_textGameOver);
		_gameOver.setCharacterSize(55);
		_gameOver.setFillColor(sf::Color::White);
		_gameOver.setOutlineColor(_fontColor);
		_gameOver.setOutlineThickness(4);
		gameOver = _gameOver.getLocalBounds();
		_gameOver.setOrigin(gameOver.width / 2.f, gameOver.height * 1.5f);
		_gameOver.setPosition(_field->GetWindowSize().x / 2.f, _field->GetWindowSize().y / 2.f);

		// init hint
		_textHint = "Press 'space' for start";
		_hint.setFont(_font);
		_hint.setString(_textHint);
		_hint.setCharacterSize(18);
		_hint.setFillColor(sf::Color::White);
		_hint.setOutlineColor(_fontColor);
		_hint.setOutlineThickness(2);
		hint = _hint.getLocalBounds();
		_hint.setOrigin(hint.width / 2.f, hint.height / 2.f);
		_hint.setPosition(_field->GetWindowSize().x / 2.f, (_field->GetWindowSize().y / 2.f) + hint.height);

	}

	void SetPosition(sf::RenderWindow &renderWindow) {
		scoreRect = _score.getLocalBounds();
		lvlRect = _lvl.getLocalBounds();
		xnRect = _xn.getLocalBounds();
		fullRect = _full.getLocalBounds();

		float countTextElement = 4;

		float windowWidth = _field->GetWindowSize().x;
		float windowHeight = _field->GetWindowSize().y;
		float fieldWidth = _field->GetWidth();
		float fieldHeight = _field->GetHeight();

		float offsetX = (windowWidth - fieldWidth) / 2.f;
		offsetX += scoreRect.width / 2.f;

		float offsetY = (windowHeight - fieldWidth) / 2.f;
		float positionTextY = windowHeight + (offsetY / 2.f);

		float widthText = scoreRect.width + lvlRect.width + xnRect.width + fullRect.width;

		float offsetText = (fieldWidth - widthText) / (countTextElement - 1);

		// <-- score
		float positionTextX = offsetX;
		_score.setPosition(positionTextX, positionTextY);
		// -->

		// <-- lvl
		positionTextX += (scoreRect.width / 2.f) + offsetText + (lvlRect.width / 2.f);
		_lvl.setPosition(positionTextX, positionTextY);
		// -->

		// <-- xn
		positionTextX += (lvlRect.width / 2.f) + offsetText + (xnRect.width / 2.f);
		_xn.setPosition(positionTextX, positionTextY);
		// -->

		// <-- xn
		positionTextX += (xnRect.width / 2.f) + offsetText + (fullRect.width / 2.f);
		_full.setPosition(positionTextX, positionTextY);
		// -->
	}

	void SetDefaulValue() {
		_textScore = "Score: ";
		_textFull = "Full: ";
		_textLvl = "Lvl: ";
		_textXn = "Xn: ";
	}

	void DrawGameStart(sf::RenderWindow &renderWindow) {
		renderWindow.draw(_gameStart);
	}

	void DrawGameOver(sf::RenderWindow &renderWindow) {
		renderWindow.draw(_gameOver);
	}

	void DrawHint(sf::RenderWindow &renderWindow) {
		renderWindow.draw(_hint);
	}

	void DrawScore(int score, sf::RenderWindow &renderWindow) {
		std::string text = _textScore;
		text += toString(score);
		_score.setString(text);
		scoreRect = _score.getLocalBounds();
		_score.setOrigin(scoreRect.width / 2, scoreRect.height / 2);
		renderWindow.draw(_score);
	}

	void DrawFull(int full, sf::RenderWindow &renderWindow) {
		std::string text = _textFull;
		text += toString(full);
		text += "%";
		_full.setString(text);
		scoreRect = _full.getLocalBounds();
		_full.setOrigin(scoreRect.width / 2, scoreRect.height / 2);
		renderWindow.draw(_full);
	}

	void DrawXn(int xn, sf::RenderWindow &renderWindow) {
		std::string text = _textXn;
		text += toString(xn);
		_xn.setString(text);
		scoreRect = _xn.getLocalBounds();
		_xn.setOrigin(scoreRect.width / 2, scoreRect.height / 2);
		renderWindow.draw(_xn);
	}

	void DrawLevel(int level, sf::RenderWindow &renderWindow) {
		std::string text = _textLvl;
		text += toString(level);
		_lvl.setString(text);
		scoreRect = _lvl.getLocalBounds();
		_lvl.setOrigin(scoreRect.width / 2, scoreRect.height / 2);
		renderWindow.draw(_lvl);
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
	int _winLevelPercent = 60;
	int _level = 1;

	GameStates _gameStates;
	GameStates _prevState = EMPTY_STATE;

	Field* _field;
	Xonix* _xonix;
	SeaEnemy* _seaEnemy;
	std::vector<SeaEnemy*>* _seaEnemies;
	LandEnemy *_landEnemy;

public:
	StateManager(Field *field, Xonix *xonix, SeaEnemy *seaEnemy, std::vector<SeaEnemy*>* seaEnemies, LandEnemy *landEnemy) {
		_field = field;
		_xonix = xonix;
		_seaEnemy = seaEnemy;
		_seaEnemies = seaEnemies;
		_landEnemy = landEnemy;
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

	void UpdateStates(sf::RenderWindow &renderWindow) {
		// Next level
		if (_xonix->GetSeaPercent() >= _winLevelPercent) {
			_gameStates = NEXT_LEVEL;
			_level++;
			_xonix->IncreaseLive();

			_field->Init(renderWindow);
			_xonix->Init();
			//_seaEnemy->Init();

			_seaEnemies->push_back(&SeaEnemy(_field));
			_seaEnemy->InitSeaEnemies(_seaEnemies);

			_landEnemy->Init();
			_xonix->FillTrackArea();
			_gameStates = PLAYING;
		}
		// Game over
		if (_xonix->GetIsSelfCross() || 
			//_seaEnemy->IsHitTrackOrXonix() || 
			//_seaEnemy->EnemiesHitTrackOrXonix() ||
			_landEnemy->IsHitXonix() &&
			_xonix->GetCountLives() >= 1 &&
			_gameStates != _prevState) {
			
			// Once enter to condition
			_gameStates = LOST_LIVE;
			_prevState = _gameStates;
			
			_xonix->DecreaseLive();
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
	sf::RenderWindow window(sf::VideoMode(800, 600), "Xonix");
	window.setFramerateLimit(30); // Temporary solution

	Field *field = new Field(window);
	///////////////////////////////////////////////////////////////////
	std::vector<SeaEnemy*>* seaEnemies = new std::vector<SeaEnemy*>();

	SeaEnemy *seaEnemy = new SeaEnemy(field);
	//sleep(milliseconds(1000));0
	//SeaEnemy *seaEnemy2 = new SeaEnemy(field);

	seaEnemies->push_back(seaEnemy);
	//seaEnemy->InitSeaEnemies(seaEnemies);
	//seaEnemies.push_back(seaEnemy2);

	//////////////////////////////////////////////////////////////////
	Xonix *xonix = new Xonix(field, seaEnemies);
	//seaEnemy->SetXonix(xonix);
	for (int i = 0; i < seaEnemies->size(); i++) {
		seaEnemies->at(i)->SetXonix(xonix);
	}
	//seaEnemy->SetXonix(xonix);
	//seaEnemy2->SetXonix(xonix);



	LandEnemy *landEnemy = new LandEnemy(field, xonix);
	Info info(field);

	StateManager stateManager(field, xonix, seaEnemy, seaEnemies, landEnemy);
	stateManager.SetState(INIT_DEPENDENCIES);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				window.close();

			if (stateManager.GetState() == PLAYING)
				xonix->SetDirection();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && stateManager.GetState() == START_GAME)
				stateManager.SetState(PLAYING);

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && stateManager.GetState() == PLAYING) {
				for (int i = 0; i < seaEnemies->size(); i++) {
					std::cout << "(" << i + 1 << "): X: " << seaEnemies->at(i)->GetX() << "\t";
					std::cout << "(" << i + 1 << "): Y: " << seaEnemies->at(i)->GetY() << "\n\n";
					std::cout << "(" << i + 1 << "): X: " << seaEnemies->at(i)->GetX() << "\t";
					std::cout << "(" << i + 1 << "): Y: " << seaEnemies->at(i)->GetY() << "\n\n";
					seaEnemies->at(i)->SetXonix(xonix);
				}
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && stateManager.GetState() == GAME_OVER) {
				field->Init(window);

				for (int i = 0; i < seaEnemies->size(); i++) {
					seaEnemies->at(i)->Init();
				}
				//seaEnemy->Init();
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
			for (int i = 0; i < seaEnemies->size(); i++) {
				seaEnemies->at(i)->Init();
			}
			//seaEnemy->Init();
			landEnemy->Init();
			info.Init();

			stateManager.SetState(START_GAME);
		}

		// Start game
		if (stateManager.GetState() == START_GAME) {
			info.DrawGameStart(window);
			info.DrawHint(window);
		}

		// Draw entities
		if (stateManager.GetState() != START_GAME && 
			stateManager.GetState() != INIT_DEPENDENCIES &&
			stateManager.GetState() != GAME_OVER) {
			field->Draw(window);
			xonix->Draw(window);
			for (int i = 0; i < seaEnemies->size(); i++) {
				seaEnemies->at(i)->Draw(window);
			}
			//seaEnemy->Draw(window);
			landEnemy->Draw(window);
		}

		// Draw text info
		if (stateManager.GetState() != START_GAME && 
			stateManager.GetState() != INIT_DEPENDENCIES &&
			stateManager.GetState() != GAME_OVER) {
			info.DrawScore(xonix->GetScore(), window);
			info.DrawFull(xonix->GetSeaPercent(), window);
			info.DrawXn(xonix->GetCountLives(), window);
			info.DrawLevel(stateManager.GetLevel(), window);
			info.SetPosition(window);
		}

		// Lost live
		if (stateManager.GetState() == LOST_LIVE) {
			xonix->Init();
			field->ClearTrack();
			landEnemy->Init();
			sf::sleep(sf::milliseconds(1000));
			stateManager.SetState(PLAYING);
		}

		// Move
		if (stateManager.GetState() != START_GAME &&
			stateManager.GetState() != INIT_DEPENDENCIES &&
			stateManager.GetState() != GAME_OVER) {

			xonix->Move(window);
			//seaEnemy->Move();
			/*for (int i = 0; i < seaEnemies->size(); i++) {
				seaEnemies->at(i)->Move();
			}*/
			landEnemy->Move();
		}

		// Game Over
		if (stateManager.GetState() == GAME_OVER) {
			info.DrawGameOver(window);
			info.DrawHint(window);
			xonix->ResetCountLives(3);
			xonix->ResetScore();
			xonix->ResetSeaArea();
			stateManager.ResetLevel();
		}
		window.display();
	}

	return 0;
}