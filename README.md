# Durak Game Robot

Эта статья описывает простую реализацию игрового робота Durak.
Этот робот состоит из 9 частей, это:

1. Реализация карты - `card.h`
2. Реализация правил игры - `rule.h`
3. Динамический калькулятор веса для карт и ситуаций - `ranker.h`
4. Управление рекордером и игроком - `counter.h`
5. Реализация плеера - `iplayer.h`
6. Реализация алгоритма дерева - `tree.h` &` data.h`
7. Создайте дерево прогнозов, используя алгоритм связующего дерева - `thinker.h`
8. Проанализируйте дерево прогноза - `thinker.h`
9. Другое - `player.h` &` translation.h`

Необходимо пояснить, что в `header.h` записаны некоторые константы программы и предварительно объявлены все классы, структуры, которые могут использоваться в программе; в каждом заголовочном файле, кроме того, который будет содержать заголовочный файл Чтобы избежать ошибок компиляции.

## Часть 1 - Реализация карты

Эта часть реализована с помощью `card.h` в исходном коде. Ниже приведено краткое описание:

### Class `iCard`

```
class iCard {
private:
	std::string _suit;
	std::string _rank;

public:
	iCard(std::string, std::string);
	std::string suit(void) const;
	std::string rank(void) const;
	static int compare(const iCard*, const iCard*);
	bool operator>(const iCard*&);
	bool operator<(const iCard*&);
	bool operator>=(const iCard*&);
	bool operator<=(const iCard*&);
	bool operator==(const iCard*&);
	friend std::ostream& operator<<(std::ostream&, const iCard&);
};
```

`iCard` - это класс карт, используемый программой для внутренних целей и имеющий две частные переменные типа` std :: string`: `_suit` и` _rank`, которые соответствуют масти и номерам карт.
Класс инициализируется передачей двух стандартных строк и предоставляет два общедоступных интерфейса: "suit" и "rank" для получения номеров мастей и колод колоды.

Классы поддерживают `<>> = <= ==` перегрузкой операторов, они сравнивают `_rank` текущей карты с` _rank` цели при выполнении сравнения.
В то же время класс предоставляет статический общедоступный интерфейс `Compare (const iCard *, const iCard *)` для сравнения карт между экземплярами, не являющимися экземплярами: когда карта первого параметра велика, интерфейс будет Возвращает положительное число, отрицательное число, когда второе большое, и возвращает `0`, когда два одинаковы.

Класс реализует поддержку печати для информации о карте, перегружая выходной оператор `<<`.

### Class `CardManager`

```
class CardManager {
private:
	std::vector<iCard*> all;
	std::unordered_map<std::pair<std::string, std::string>, iCard*> library;

public:
	CardManager(void);
	~CardManager(void);
	std::vector<iCard*> getall(void);
	iCard* get(std::string&, std::string&);
	iCard* get(std::pair<std::string, std::string>&);
};
```

При создании экземпляра класса `CardManager` класс создаст все карты в памяти кучи и сохранит все указатели во внутренней переменной` all`, которая будет использоваться для освобождения памяти при уничтожении экземпляра.
Класс предоставляет два перегруженных `get` интерфейса для получения указателей внутри карты.

Поскольку `std :: unordered_map` требуется в реализации` CardManager` с `std :: pair <std :: string, std :: string>` в качестве ключа, то здесь реализовано расширение `hash` пользовательского типа. :

```
// Specialized hash support for pair<string, string>
namespace std {
	template<>
	class hash<std::pair<std::string, std::string>> {
	public:
		std::size_t operator()(const std::pair<std::string, std::string>& obj) const {
			std::hash<std::string> first;
			std::hash<std::string> second;
			return first(obj.first) + second(obj.second);
		}
	};
}
```

## Часть 2 - Реализация правил игры

Из-за различий в правилах между атакой и защитой у нас есть два различных интерфейса в `rule.h` для расчета карт, которые можно использовать.
Два открытых интерфейса в `rule.h` имеют одинаковую структуру параметров:`std ::vector <iCard *>& desk, std::vector <iCard *>& inhand`, где `desk` относится к рабочему столу В настоящее время есть карты, и «вдох» относится к руке игрока.

### Атака

Согласно правилам игры, атакующий должен найти карту с номером на текущем рабочем столе в качестве атакующей руки, поэтому сначала нужно посчитать число, появившееся за текущим столом.
Настройте частный интерфейс в классе `shown`, чтобы вернуть серийный номер карты, которая когда-либо появлялась. Обратите внимание, что интерфейс возвращает тип `std :: unordered_set <std :: string>`, потому что хранение карты в карте осуществляется стандартным типом строки, и здесь нам нужно часто находить строку Внутри этой последовательности использование хеш-таблицы в возвращаемом значении может помочь повысить производительность.

```
std::vector<iCard*> Rule::attack(std::vector<iCard*>& desk, std::vector<iCard*>& inhand) {
	
	// If inhand is empty
	if (inhand.size() == 0) return std::vector<iCard*>();

	// When the offense board's desktop is empty
	// you can play every cards.
	if (desk.size() == 0) return inhand;

	// Otherwise, you can only play cards that you have ever shown.
	std::vector<iCard*> possible;
	std::unordered_set<std::string> shown = this->shown(desk);
	std::unordered_set<std::string>::iterator notexist = shown.end();

	for (auto& card : inhand) {
	
		// Deal with existed card
		const std::string rank = card->rank();
		if (shown.find(rank) != notexist)
			possible.push_back(card);
	}

	return possible;
}
```

Когда рука пуста, это означает, что нам нужно вернуть пустую последовательность, а когда на столе нет карт, нам нужно вернуть все текущие руки (потому что мы можем разыграть любую руку в это время).

### Оборона

Правила защиты немного сложнее: игрок может разыграть большую карту, чем атакующая сторона, или любого туза, и когда атакующий использует туза для атаки, защитная сторона должна использовать туза большего размера, чем атакующий. Защищать, в противном случае объявить оборонительный отказ.
Здесь нам нужно определить, является ли карта козырем, поэтому мы определяем частный интерфейс `istrump` в классе, который используется для определения того, является ли указанная карта козырем.
В то же время, мы также должны получить последнюю карту на столе в качестве карты для защиты. Здесь, поскольку `vector` упорядочен, каждая из наших операций` push_back` используется для вытягивания карты в таблицу, поэтому, когда вам нужно извлечь последнюю карту, вы можете передать `std::vector<iCard*>::back` реализация. Эта часть программного кода реализована в `Rule::last`.

```
std::vector<iCard*> Rule::defend(std::vector<iCard*>& desk, std::vector<iCard*>& inhand) {

	// If inhand or desk is empty
	if (inhand.size() == 0) return inhand;

	// In defend module, you can play card with bigger or trump card
	std::vector<iCard*> possible;
	const iCard* last = this->last(desk);

	std::string trump_suit = trump->suit();
	std::string last_suit = last->suit();
	bool defend_trump = (last_suit == trump_suit);

	for (auto& card : inhand) {

		std::string card_suit = card->suit();

		// When in the trump defensive mode, 
		// the current card needs to be greater than the last;
		if (defend_trump == true) {

			// If it is general card
			if (card_suit != trump_suit)
				continue;

			if (card->operator>(last))
				possible.push_back(card);

			continue;
		}

		// Otherwise, if the current card is a trump card, 
		// we can defend successfully;
		if (card_suit == trump_suit) {
			possible.push_back(card);
			continue;
		}

		// When both cards are not ace, 
		// check if the two cards are the same suit;
		if (card_suit == last_suit) {

			// Defensive success when the same suit 
			// and the current card is larger
			if (card->operator>(last))
				possible.push_back(card);

		}

	}

	return possible;
}
```

Булева переменная `defnd_trump` в коде используется для указания того, является ли карта, которую программа должна защищать в настоящее время, козырной картой для другой защитной логики.

## Часть 3 - Ranker: Калькулятор динамического веса

Будь то в наступательном или оборонительном процессе, мы можем столкнуться с ситуациями, когда доступно несколько карт. В настоящее время нам необходимо измерить важность карты с нашей стороны, с помощью класса Ranker мы можем вычислить важность руки, указанной в главе в текущей ситуации, и кратко описать факторы расчета веса для любой карты:

1. Чем больше номер карты, тем больше ее вес
2. Если это козырная карта, ее вес важнее, чем средняя карта.
3. Если в руке повторяется тот же номерной знак этой карты, это может быть полезно в следующем раунде атаки, и его вес увеличивается.
4. Когда игра находится на поздней стадии, карта с правильным весом относительно важнее для нас, и ее вес увеличивается.

Исходя из вышеизложенных соображений, чтобы отделить программу, мы определяем четыре вышеуказанных правила как четыре открытых интерфейса, реализованных в `ranker.h`:

```
class Ranker {
private:
	Counter<global::players>* _counter;
	const iCard* _trump;

public:
	static double zipvalue(double);
	static double absolute(const iCard* card);

	Ranker(Counter<global::players>*, const iCard*);
	double progress(void);
	double trump(const iCard* card);
	double repeat(iPlayer*, const iCard*);
};
```

### `Ranker::progress` - Рассчитать ход игры

```
// Calc for progress coefficent
double Ranker::progress(void) {
	double all = global::total;
	double left = this->_counter->unknown().size();
	if (left == 0) left = 1;
	double progress = all / left;
	return this->zipvalue(progress);
}
```

В этом разделе мы читаем количество позиционных карт в игре - `left`, которое делит начальное количество карт в игре (по умолчанию 52, определено в` global :: total`), и получается прогресс в игре. Соответствующий фактор (это значение должно быть больше, чем `1`).
Следует отметить, что когда игра только началась, значение может быть очень большим (например, `42/2 = 21`), нам нужно сжать его в диапазоне` [1, 2] `, используйте здесь` Ranker :: zipvalue` интерфейс:

```
// Scale the function value field to [1,2]
double Ranker::zipvalue(double value) {
	return atan(value) / (0.5 * M_PI) + 1; 
}
```

Этот интерфейс использует математическую функцию для сжатия произвольно больших значений в интервале `[1, 2]`. Изображение функции выглядит следующим образом:
![zipvalue](images/zipvalue.png)

Его функция определяется следующим образом: ![\frac{\arctan{x}}{0.5\cdot\pi} + 1](http://latex.codecogs.com/gif.latex?\\frac{\\arctan{x}}{0.5\\cdot\\pi}+1)

### `Ranker::trump`

Интерфейс реализован следующим образом:

```
// Add weight to the trump card
double Ranker::trump(const iCard* card) {
	if (card->suit() == this->_trump->suit())
		return global::trump_factor;
	return 1.0;
}
```

Возвращает пользовательский параметр `global :: trump_factor`, когда считается, что карта является козырем, в противном случае возвращает` 1`

### `Ranker::absolute`

Этот интерфейс используется для вычисления значений веса, основанных исключительно на торговой марке, умножьте марку на значение пользовательского параметра `global :: unsstrained_factor`, чтобы получить:

```
// Calc absolute rank for card
double Ranker::absolute(const iCard* card) {
	std::string rank = card->rank();
	double raw = global::iranks[rank];
	return global::unconstrained_factor * raw;
}
```

### `Ranker::repeat`

Интерфейс считает количество повторяющихся карт в руке и умножает его на пользовательский параметр `global :: repetition_factor`, чтобы получить:

```
double Ranker::repeat(iPlayer* player, const iCard* target) {
	int count = 0;
	std::string rank = target->rank();
	std::vector<iCard*>& inhand = player->hand();
	for (auto& card : inhand)
		if (card->rank() == rank)
			count++;

	return global::repetition_factor * count;
}
```

## Часть 4 - Управление карточным игроком и геймером

Эта часть определена в `counter.h`, потому что необходимо выполнить * симуляцию удержания * в ситуации в более поздней реализации, поэтому мы не записываем информацию о руке игрока в плеере` iplayer.h`, а вместо этого Введите несколько частных свойств класса `Counter`:

```
template <int number>
class Counter {
private:
	std::vector<iCard*> _desk; // cards in desk
	std::vector<iCard*> _unknown; // card we don't know temproary

private:
	CardManager* manager;
	std::array<iPlayer*, number> players; // all players
	std::array<size_t, number> count; // count of every one's cards
	std::array<std::vector<iCard*>, number> inhand; // cards for everyone

public:
	Counter(CardManager&, iCard*);
	Counter(const Counter<global::players>& counter);

	// Moveout one card from a specified vector
	static void moveout(std::vector<iCard*>& set, iCard*& target);

public:
	int join(iPlayer* player); // join a player
	iPlayer* player(int index) const; // return player
	iPlayer* player(std::string name) const; // return player

public:
	void grab(iPlayer*);
	void replenish(iPlayer*, int);
	void hit(iPlayer*, iCard*);
	void get(iPlayer*, iCard*);
	void hit(iPlayer*, std::string&, std::string&);
	void get(iPlayer*, std::string&, std::string&);

public:
	void clear(void); // clear desk
	std::vector<iCard*>& desk(void); // return desk
	std::vector<iCard*>& unknown(void); // return unknown
	std::vector<iCard*>& hand(iPlayer*); // return inhands for player
	int left(iPlayer*); // return number of remaining cards for player
};
```

Закрытые переменные в классе записывают следующую информацию:

- `std::vector<iCard*> _desk`: карта за текущим столом
- `std::vector<iCard*> _unknown`: временно неизвестная карта (стопка карт)
- `CardManager manager`: описанный выше менеджер карт, обеспечивающий хранение карты инициализации
- `std::array<iPlayer*, number> player`: записать все указатели игроков
- `std::array<size_t, number> count`: записать количество игроков в соответствующей позиции
- `std::array<std::vector<iCard*>, number> inhand;`: записать последовательность рук игрока в соответствующей позиции

Публичное описание метода класса:

- `int Counter::join`: присоединиться к игроку
- `iPlayer Counter::player(int index) const`: поиск игроков по локации
- `iPlayer Counter::player(std::stirng name) const`: поиск игроков по имени
- `Counter::grab`: игрок признался и взял все карты на столе.
- `Counter::replenish`: игрок берет карту, но не знает информацию о карте, которую он получил.
- `Counter::get`: игрок берет карту и записывает информацию о карте.
- `Counter::hit`: игрок играет карту
- `Counter::clear`: очистить рабочий стол
- `Counter::desk`: возвращает ссылку на рабочий стол
- `Counter::unknown`: возвращает ссылку на кучу неизвестных карт
- `Counter::hand`: возвращает ссылку на руку игрока
- `Counter::left`: возвращает количество рук игрока

Следует отметить, что хотя возвращаемое значение количества рук в указанном выше интерфейсе может не быть `0`, в ссылке на руку игрока может быть возвращена пустая последовательность, потому что в некоторых случаях мы не знаем о враге. Информация в руках партии, но известно количество карт противника.

Класс `Counter` - это шаблонный класс, который требует заданного количества игроков во время инициализации, что указано в `global::players`.
Реализация вышеупомянутого интерфейса относительно проста, реализована в `counter.cpp`.

## Часть 5 - Реализация проигрывателя

Индивид игрока реализован в `iplayer.h` и определяется следующим образом:

```
class iPlayer {
private:
	int _index; // our index
	bool _we; // its our player
	bool _turn; // our turn or not
	bool _status; // attack or defend
	std::string _name;

	Rule* analyser;
	Ranker* ranker;
	Counter<global::players>* counter;

public:
	int total(void); // return total count of our cards
	std::vector<iCard*>& desk(void); // return ref for desk cards
	std::vector<iCard*>& hand(void); // return ref for inhand's cards
	std::vector<iCard*>& unknown(void); // return ref for unknown cards

public:
	iPlayer(bool we, bool turn, bool status, std::string name);
	bool us(void) const; // return if it's our player
	bool turn(void) const; // return if it's our turn
	bool status(void) const; // return status for player
	int index(void) const; // return index
	std::string name(void) const; // return name
	void set_index(int);
	void set_counter(Counter<global::players>* counter); // reset counter for think
	void set_analyser(Rule* analyser); // set analyser
	void set_ranker(Ranker* ranker); // set ranker

public:
	void grab(void); // grab all cards
	void replenish(void); // get some cards we dont know
	void get(iCard*); // get one card
	void hit(iCard*); // hit one card
	void get(std::string&, std::string&); // get one card
	void hit(std::string&, std::string&); // hit one card

public:
	std::vector<iCard*> analysis(void); // analysis we could play
	double posibility(const iCard* card); // return for one card's posibility
	double rank(const iCard* card); // return for one card's rank

public:
	void transform(void); // takein/out our turn's control
	void toggle(void); // change our attack/defend mode
};
```

Значение представления закрытой переменной класса следующее:

- `_index`: номер игрока в классе` Counter`
- `_we`: этот игрок принадлежит известному игроку (полностью ли известна информация о руке)
- `_turn`: это очередь игрока атаковать и защищаться?
- `_status`: игрок находится в состоянии атаки / обороны
- `_name`: имя игрока

Реализация большинства вышеперечисленных интерфейсов очень проста, вы можете ссылаться на код в `iplayer.cpp` и комментарии к коду.
Необходимо объяснить это следующие три интерфейса:

- `std :: vector <iCard *> iPlayer :: analysis (void)`: этот интерфейс анализирует карту, которую игрок может сыграть в данный момент, вызывая экземпляр, сохраненный в плеере, с помощью класса `Rule`, описанного выше.
- `double iPlayer :: rank (const iCard * card)`: этот интерфейс вычисляет вес карты путем вызова экземпляра класса Ranker, описанного выше.
- `double iPlayer :: posibility (const iCard * card)`: этот интерфейс вычисляет вероятность карты в руке текущего игрока, рассчитанную следующим образом:
	1. Когда игрок принадлежит известному игроку (смотрите приватную переменную `_we`), верните` 1`, если указанная карта есть в нашей записи рук, в противном случае верните `0`
	2. Если игрок не принадлежит известному игроку, но указанная карта находится в записи руки, верните `1`
	3. Если игрок не принадлежит известному игроку, но указанная карта отсутствует в неизвестной карте и записи о руке, это означает, что карта больше не присутствует, принадлежит к сбросу и возвращает `0`
	4. Если игрок не принадлежит известному игроку и целевая карта обнаружена в неизвестной карте, рассчитайте следующее:
		1. `let a = unknown.size ()` - вычислить размер неизвестной стопки
		2. `let b = respnd.size ()` - подсчитать количество известных рук
		3. `let c = total` - получить общее количество рук
		4. Вероятность: ![\frac{c - b}{a + c -b}](http://latex.codecogs.com/gif.latex?\\frac{c-b}{a+c-b})

Принцип таков:

Задача эквивалентна извлечению m из n объектов и нахождению указанного объекта, что может быть известно из знаний теории вероятностей:
![\frac{C_{m-1}^{n-1}}{C_m^n} = m / n = \frac{c - b}{a + c - b}](http://latex.codecogs.com/gif.latex?\\frac{C_{m-1}^{n-1}}{C_m^n}=m/n=\\frac{c-b}{a+c-b})

## Часть 6 - Реализация алгоритма дерева

Алгоритм дерева используется, чтобы играть роль в последующем прогнозировании ситуации.Дерево состоит из трех частей: данных (`Data`), узла (` Node`) и дерева (`Tree`), которые представлены ниже.

### Класс данных

Класс данных определен в `data.h`, и его состав очень прост:

```
struct Data {
	double rank; double possibility; iCard* card;
	Data(iCard* card, double rank, double possibility);
};
typedef struct Data Data;
```

Где `rank` представляет вес карты, представленной текущим узлом,`possibility`используется для указания возможности появления описания узла, `card` - карта, которой должен управлять узел.

### Класс узла

Класс узла определяется следующим образом:

```
/* Tree (data structure) support */
class Node {
private:
	int _degree;
	Data* data;
	Node* _father = nullptr;
	std::vector<Node*> children;

public:
	Node(void);
	Node(const Data& data);
	~Node(void);

	std::vector<Node*>::const_iterator begin(void) const;
	std::vector<Node*>::const_iterator end(void) const;

public:
	Data* get(void) const;
	int degree(void) const;
	Node* father(void) const;

	void add(Node*& child);
	void add(const Data& data);
	void remove(const int index);

	Node* operator[] (int index);
	const Node* operator[] (int index) const;
};
```

Описание приватной переменной:

- `_degree`: степень узла
- `data`: указатель класса узла` Data`
- `_father`: адрес родительского узла
- `children`: список дочерних узлов

Поскольку нам нужно использовать новую функцию `C++ 11` - цикл, основанный на диапазоне, для обхода дочерних элементов узла, класс реализует методы` begin` и `end`.

Класс узла реализует поддержку индексирования путем перегрузки оператора `[]`.
Вызов деструктора класса узла попытается удалить все дочерние узлы из памяти кучи.

### Класс дерева (структура данных)

Класс дерева определяется следующим образом:

```
/*
	Tree support
	bool exist(Node* target); // Find if specified node in tree
	std::vector<Node*> path(const Node*); // Find path for specified node
*/
class Tree {
private:
	Node* root;

public:
	Tree(Node*);
	~Tree(void);
	bool exist(const Node*);
	std::stack<Node*> search(const Node*);
	static std::stack<Node*> path(const Node*);

	// Give an iterator that generates a root node to a leaf node
	class Parser {
	private:
		bool running = true;
		DFSTraverse traverser;

	public:
		Parser(Node*);
		bool status(void) const;
		std::stack<Node*> yield(void);
	};

	Tree::Parser leaves(void);
};
```

Среди них `Tree :: Parser` использует алгоритм` DFS` для реализации обходчика пути от корневого узла ко всем конечным узлам (описано ниже). Его использование заключается в следующем:

```
Tree::Parser parser = tree.leaves();
while (parser.status()):{
	std::stack<Node*> path = parser.yield();
	...
}
```

Введение в публичные методы:

1. `Tree(Node*)` - конструктор класса для хранения корневого узла
2. `~Tree(Node*)` - деструктор класса. Следует отметить, что деструктор попытается удалить корневой узел `root`, поэтому корневой узел должен храниться в памяти кучи. Согласно приведенному выше определению класса `Node`, это будет рекурсивно удалять все узлы из памяти кучи.
3. `bool exist(const Node*)` - используйте алгоритм `BFS`, чтобы найти указанный узел в дереве, верните` true` после нахождения целевого узла, в противном случае верните `false`
4. `std::stack<Node*> search(const Node*)` - Статическая функция, поскольку каждый узел хранит информацию о родительском узле, при вызове этой функции будет пытаться пройти все родительские узлы и поместить родительский узел в стек. И вернемся в стек
5. `std::stack<Node*> search(const Node*)` - объединить вызов двух вышеупомянутых функций: функция попытается найти целевой узел в дереве и вернет информацию о пути узла после его поиска, в противном случае она возвратит ноль. стек

Древовидная диаграмма:

![tree](images/tree.png)

## `BFS` и `DFS` алгоритм и класс `Traverse`

### Алгоритм `BFS`

`BFS` -`Breadth-first search`

Алгоритм `BFS` внутренне поддерживает очередь, которая рекурсивно пересекает 
все узлы дерева, используя функцию « первым пришел - первым вышел »(FIFO). Процесс прохождения выглядит следующим образом:

1. Сначала добавьте корневой узел в очередь задач.
2. Возьмите узел из очереди и добавьте все его дочерние узлы в очередь задач.
3. Повторяйте шаг 2, пока очередь не станет пустой

Поскольку очередь - `std::queue` является" первым вошел, первым обслужен ", это всегда пересекает дерево в иерархическом порядке, как показано на следующей диаграмме:

![bfs](images/bfs.png)

### Алгоритм `DFS` 

`DFS` -`Depth-first search`

Алгоритм `DFS` в основном такой же, как и ` BFS`, за исключением того, что он поддерживает стек внутри. В отличие от очереди: стек расширен (FILO), поэтому вышеупомянутый метод обхода будет проходить в соответствии с глубиной. Следующим образом:

![dfs](images/dfs.png)

### `Traverse` класс

Поскольку программа должна реализовывать как `BFS`, так и` DFS`, а большая часть ее кода одинакова, мы сначала реализуем ту же часть базового класса: `Traverse`, который определяется следующим образом:

```
/* Traversing support */
template <typename Contaniner>
class Traverse {
protected:
	Node* current;
	bool running = true;
	Contaniner unvisited;

public:
	// Prevent the compiler from generating a default constructor
	Traverse(void) { this->current = nullptr; }

	typedef Node* data;
	Traverse(Node* root) {
		this->unvisited.push(root);
		this->current = root;
	}

	bool status(void) const {
		return this->running;
	}

	virtual void next(void) = 0;

	data get(void) const {
		return this->current;
	}

	data yield(void) {
		try {
			this->next();
			data current = this->current;
			return current;
		}
		catch (std::out_of_range) {
			this->running = false;
			return nullptr;
		}
	}
};
```

Это шаблонный класс, который использует `Container` для указания контейнера данных (реализуйте` BFS` при реализации `DFS`,` Queue` при `Stack`). В некоторых методах интерфейс между `Stack` и` Queue` отличается, поэтому мы устанавливаем интерфейс `next`, который извлекает данные в чисто виртуальную функцию, наследуя и реализуя их в подклассе:

```
/*
	Traversing the tree using the BFS algorithm
	Traverse all nodes of the tree using the queue's first-in,
	first-out (FIFO) feature level.
*/
class BFSTraverse : public Traverse<std::queue<Node*>> {
	using Traverse::Traverse;
	void next(void) {
		// If queue is empty
		if (!this->unvisited.size())
			throw std::out_of_range("traverse ended.");

		this->current = this->unvisited.front();
		this->unvisited.pop();

		// Traversing the child nodes to the queue
		if (!this->current->degree())
			return;
		for (auto child : *(this->current))
			this->unvisited.push(child);
	}
};

/*
	Traversing the tree using the DFS algorithm
	The principle is the same as above, but the stack is LIFO
*/
class DFSTraverse : public Traverse<std::stack<Node*>> {
	using Traverse::Traverse;
	void next(void) {
		// If list if empty
		if (!this->unvisited.size())
			throw std::out_of_range("traverse ended.");

		this->current = this->unvisited.top();
		this->unvisited.pop();

		// Traversing the child nodes to the queue
		if (!this->current->degree())
			return;
		for (auto child : *(this->current))
			this->unvisited.push(child);
	}
};
```

Это позволяет реализовать два разных обходчика узлов дерева путем создания экземпляров классов `BFSTraverse` и` DFSTraverse` соответственно.
Базовый алгоритм обслуживания дерева может быть реализован путем объединения вышеуказанных узлов и реализаций дерева.

## Часть 7 - Предиктор ситуации, использующий алгоритм связующего дерева

Выше мы реализовали алгоритм дерева, класс игрока, класс правил и вычисления вероятности и веса, а затем мы объединили все вышеперечисленные части и реализовали простой предиктор ситуации, используя алгоритм связующего дерева.
Прежде всего, нам нужно сохранить как минимум информацию, необходимую для генерации каждого узла:

1. Состояние текущего узла - нападение или защита
2. Глубина текущего узла - `layer`
3. Карта, которой будет управлять текущий узел - `card`
4. Родительский узел текущего узла - `father`
5. Текущая ситуация с узлом - `situation`

Таким образом, мы получаем следующую структуру узла предсказания - `thinker.h`:

```
// Node for prediction, using in task queue
struct Prediction {
	bool mode; // attack or defense
	int layer; // current prediction's layer
	iCard* card; // operate card
	Node* father; // father node
	Counter<global::players>* situation; // layer
};
```

Следует отметить, что каждый из наших узлов должен сделать полную копию класса `Counter` текущей ситуации, чтобы выполнить операцию прогнозирования, не влияя на ход исходной игры; для этого требуется, чтобы класс `Counter` поддерживал структуру копирования, и Полностью скопируйте все данные в конструкторе копирования. Эта часть была реализована в классе `Counter` выше и не будет описана снова.

Далее нам нужно использовать указанный выше узел прогнозирования для рекурсивной генерации дерева, соответствующего структуре `Data`, которая определена в классе` TreeMaker`:

```
class TreeMaker {
private:
	Node* root = nullptr; // root node
	int maxlayer = global::depth;
	iPlayer* attacker; iPlayer* defender;

private:
	std::queue<Prediction*> tasks; // tasks queue
	static Node* complex(iCard*& card, iPlayer*& player);
	std::vector<Counter<global::players>*> trashbin;

public:
	TreeMaker(bool mode, iCard* rootcard, iPlayer* attacker, 
		iPlayer* defender, Counter<global::players>* situation);
	~TreeMaker(void);

public:
	Tree* make(void); // remember to delete tree after use!
};
```

Как видите, класс хранит очередь задач (`tasks`), игрока-атакующего, игрока-защитника и корневой узел.
Вот реализация связующего дерева:

```
/* Using BFS algorithm to generate pretree */
Tree* TreeMaker::make(void) {
	// Produce with tasks queue
	while (!this->tasks.empty()) {

		// Get prediction job
		Prediction* prediction = this->tasks.front();
		bool mode = prediction->mode;
		int layer = prediction->layer;
		iCard* card = prediction->card;
		Node* father = prediction->father;
		Counter<global::players>* situation = prediction->situation;
		this->tasks.pop();

		// Get player
		iPlayer* player; iPlayer* passive;
		if (mode == global::attack) {
			player = this->attacker;
			passive = this->defender;
		}
		else {
			player = this->defender;
			passive = this->attacker;
		}

		// Make node and insert to father while not root
		Node* child;
		if (father == nullptr) {
			child = this->root;
		}
		else {
			child = this->complex(card, player);
			father->add(child);
		}

		// Make predictions and add new tasks
		{
			this->attacker->set_counter(situation);
			this->defender->set_counter(situation);
			player->hit(card);

			/*
				If current prediction layer miner global::depth,
				analyze possible cards in the current situation;
				and also exchange offense and defense.

				Otherwise, stop prediction.
			*/
			if (layer < this->maxlayer)
				for (auto& card : passive->analysis()) {
					Prediction* prediction = new Prediction;
					prediction->card = card;
					prediction->mode = !mode;
					prediction->layer = layer + 1;
					prediction->father = child;
					prediction->situation = \
						new Counter<global::players>(*situation);
					this->trashbin.push_back(prediction->situation);
					this->tasks.push(prediction);
				}
		}

		// Release memeory
		delete prediction;
	}

	Tree* tree = new Tree(this->root);
	return tree;
}
```

Мы сделали следующее:

1. Сначала мы получаем необходимую нам информацию, включая карту операции, текущий статус наступления и количество предсказанных слоев.
2. После этого мы оцениваем текущий статус атакующего и защитника и определяем игрока (`player`)
3. Ниже мы используем интерфейс `complex` класса (описан ниже), чтобы преобразовать вышеуказанную информацию в узел `Node*` и добавить ее в указанный родительский узел.
4. Затем введите процесс создания новой ветви прогноза:
	1. Мы устанавливаем `situation` атакующего и защитника в текущую скопированную ситуацию через интерфейс `set_counter`, чтобы не мешать другим предсказаниям ветвления.
	2. Операция, которую мы собираемся выполнить - `player.hit (card)`
	3. После этого мы определяем, достигла ли текущая прогнозируемая глубина заданной максимальной глубины `global :: deep`. Если она достигнута, прогнозирование новой ветви не будет выполнено. В противном случае будут выполнены следующие операции.
	4. Поскольку вышеприведенным игроком `player` манипулировали, ему нужен компаньон `passive`, чтобы ответить на него. Мы можем пройти через все его карты и сгенерировать новую для каждой возможности. Предсказать ветку и добавить ее в очередь задач,  *и добавить копию текущей ситуации в корзину для мусора класса (`trashbin`)*
5. Вернитесь к шагу 1

Следует отметить, что для предотвращения выталкивания наши узлы прогнозирования (`Predction`), копия ситуации (`Counter`) и узлы дерева (`Node`) работают в куче памяти, поэтому после завершения использования Нужно вовремя освободить память.

В `TreeMaker::trashbin` все копии ситуации в вышеописанном процессе прогнозирования сохраняются и все они высвобождаются в деструкторе класса:

```
TreeMaker::~TreeMaker(void) {
	// release all Counter's memory
	for (auto& counter : this->trashbin)
		delete counter;
	this->trashbin.clear();
}
```

### `complex` конвертер

В классе `TreeMaker` есть `complex`  преобразователь, который отвечает за преобразование результирующей информации `prediction` в узел `Node`, который реализован следующим образом:

```
Node* TreeMaker::complex(iCard*& card, iPlayer*& player) {
	// This function use card to create Node in ***heap***
	double rank, possibility;
	if (card != nullptr) {
		rank = player->rank(card) * global::rank_factor;
		possibility = player->posibility(card) * global::possibility_factor;
	} else {
		rank = 0.0;
		possibility = 0.0;
	}
	Data data(card, rank, possibility);
	Node* node = new Node(data);
	return node;
}
```

Этот интерфейс вызывает вычисление весов и возможностей указанной карты указанного игрока и создает новый узел `Node` в памяти кучи, сохраняя и возвращая рассчитанные веса, возможности и возвраты.
Вызывая интерфейс `make` класса ` TreeMaker`, мы можем сгенерировать дерево предсказания ветвления для текущей операции, которое мы затем проанализируем.

![predction](images/predction.png)

## Часть 8 - Дерево анализа

Мы просто предсказали текущую ситуацию с помощью класса `TreeMaker` и сгенерировали дерево прогнозирования с несколькими форками со всеми возможными ветвями. Теперь давайте проанализируем дерево и заставим работать последнюю руку.

Ниже приведено определение класса анализа:

```
class Thinker {
private:
	iPlayer* we;
	iPlayer* enemy;
	Counter<global::players>* situation;

private:
	Tree* maketree(bool status, iCard* card);
	static double ranksum(bool starting, std::stack<Node*>& path);
	static iCard* byrank(iPlayer*& player);
	static void showpath(std::stack<Node*> path, double rank);

public:
	Thinker(iPlayer* we, iPlayer* enemy,
		Counter<global::players>* situation);

public:
	iCard* attack(void);
	iCard* attack(int);
	iCard* defend(iCard* action);
};
```

Объяснение частной функции:

- `maketree(bool status, iCard* card)` - вызвать `TreeMaker` для данной карты действий и наступления события, чтобы сгенерировать дерево и вернуться
- `static double ranksum(bool starting, std::stack<Node*>& path)` - сумма вычисленных путей для заданного начального состояния и заданного пути (подробно описано ниже)
- `static iCard* byrank(iPlayer*& player)` - дает лучший выбор на основе веса всех возможных карт без прогноза
- `static void showpath(std::stack<Node*> path, double rank)` - вывести сумму заданного пути и веса узлов

### `ranksum` функция

Его реализация заключается в следующем:

```
/* 
	Calculate the weight of the path and 
	the sum of the probabilities;
	The probability value is in the interval[0, 1],
	multiplied by the weight, and the path sum is obtained.

	Record our weight loss as a negative number and 
	the enemy's record as a positive number. 
	The larger the calculated result value, the better.
*/
double Thinker::ranksum(bool starting, std::stack<Node*>& opath) {
	// make a copy for path
	double sum = 0; bool status = starting;
	std::stack<Node*> path(opath);

	while (!path.empty()) {
		Node* node = path.top();
		Data* data = node->get();
		double loss = data->possibility * data->rank;

		if (status)
			sum -= loss;
		else
			sum += loss;

		path.pop();
		status = !status;
	}
	
	return sum;
}
```

Можно видеть, что эта функция вычисляет сумму веса и вероятности пути: ее значение вероятности умножается на вес в интервале `[0, 1]`, и все узлы последовательно суммируются для получения общей суммы пути.
А когда исходное состояние наступательное: наш вес записывается как отрицательное значение, которое используется для обозначения веса, потерянного за эту атаку, а вес противника записывается как положительное значение, указывающее, что враг потерял, чтобы защитить наши карты. вес. Таким образом, значение, возвращаемое функцией, может указывать на плюсы и минусы пути, и чем больше значение, тем лучше для нас.

### Атака и защита

В классе определены три наступательных и оборонительных интерфейса:

```
iCard* attack(void);
iCard* attack(int);
iCard* defend(iCard* action);
```

Наши идеи заключаются в следующем:

Когда мы нападаем и на столе нет карт, мы не делаем прогнозы, а разыгрываем только самую маленькую карту в соответствии с весом: поскольку в это время слишком много возможностей прогнозирования, точность прогнозирования ветвлений будет снижаться;

Когда мы оскорбительны и у нас на столе есть карта, чтобы делать прогнозы, и давать общий вес и самый большой, это лучше для нас;
Когда мы проводим оборонительные раунды, основанные на атакующей карте противника, минимальный вес лучше для нас;

Его интерфейс реализован следующим образом:

```
iCard* Thinker::attack(void) {
	return this->byrank(this->we); 
}

iCard* Thinker::attack(int) {
	// record all ways could be
	std::map<double, iCard*> record;

	std::vector<iCard*> list = this->we->analysis();
	// if no cards can be play, return nullptr
	if (list.empty()) return nullptr;

	for (auto& card : list) {
		Tree* tree = this->maketree(global::attack, card);
		Tree::Parser parser = tree->leaves();

		while (parser.status()) {
			std::stack<Node*> path = parser.yield();
			if (path.empty()) continue;

			double rank = this->ranksum(global::attack, path);
			this->showpath(path, rank);
			record[rank] = path.top()->get()->card;
		}

		delete tree;
	}

	// Return the branch with the highest weight
	auto it = record.end(); it--;
	return it->second;
}
```

Здесь мы используем `std::map` для хранения соответствия между значением веса и картой, потому что суть`map` - это красно-черное дерево, минимальное значение которого хранится в корне, а максимальное значение хранится в листовом узле, соответственно, используя `std::map<double, iCard*>::begin()/end()`доступно.

## Часть 9 - Другое

### Конвертер карт

Класс `iCard`, который мы реализуем внутренне, должен быть преобразован в класс` Card` и обратно, поэтому следующие статические функции определены в `translator.h`:

```
// translate card* to icard*
static iCard* icard(CardManager* manager, const Card* card) {
	if ((card == Dealer::GetNocard())
		||
		(card == Dealer::GetPas())
		||
		(card == nullptr)
		) {
		return nullptr;
	}
	std::string suit = Dealer::SuitName(card);
	std::string rank = Dealer::RankName(card);
	iCard* target = manager->get(suit, rank);
	return target;
}
```

Эта функция ищет соответствующий указатель `iCard` в`CardManager` и возвращает его, запрашивая атрибуты `suit` и`rank` класса `Card`.

### `Player` класс

Определение класса игрока выглядит следующим образом:

```
class Player : public PlayerAbstract {

private:
	// Mapping table between external card and internal card
	iPlayer* weinenemy;
	std::unordered_map<iCard*, Card*> cardmap;
	bool ending = false;

protected:
	iCard* trump = nullptr;
	Rule* analyser = nullptr;
	Ranker* ranker = nullptr;
	CardManager* manager = nullptr;
	Counter<global::players>* counter = nullptr;
	iPlayer* enemy = nullptr;
	Thinker* thinker = nullptr;

	iPlayer* we;
	
public:
	Player(const char* name, CardManager* manager);
	~Player(void);
	void YouTurn(bool);
	void PutCard(void);
	void TakeCards(void);
	void GetHeadTrick(void);
	void TakeOneCard(Card*& nc);
	void ShowCards(void);
	bool INeedCard(void);
	int GetCardNum(void);

public:
	void setwe(iPlayer* enemy); // Get our player's address
	iPlayer* getenemy(void) const; // Set the address of the imaginary enemy
};
```

Два объекта `iPlayer` генерируются в каждом классе игрока, один хранится как `we`, а другой как `enemy`, это принадлежит игроку и соответствующему воображаемому врагу.

Выполняя операцию (`TakeCards`,` GetHeadTrick`, `TakeOneCard`,` PutCard`), мы должны уведомить воображаемого врага противника, чтобы обновить соответствующее состояние, таким образом достигая взаимного предсказания с обеих сторон.
Поэтому вам нужно сохранить объект класса `iPlayer* weinenemy` в классе. Когда бы мы ни выполняли связанные операции с изменениями ресурса, мы должны делать это одновременно:

```
// Take one card
void Player::TakeOneCard(Card*& nc) {
	iCard* target = icard(this->manager, nc);

	// While there is no card for us
	if (target == nullptr) {
		this->ending = true;
		return;
	}

	this->cardmap[target] = nc;
	this->we->get(target);

	// Notify enemy that we get one card
	this->weinenemy->replenish();
}
```

Последняя строка `this->weinenemy->replenish()` должна информировать воображаемого врага противника о том, что мы получили руку, но не сообщать другой стороне, какую карту мы получили.

Конструктор в классе `Player` должен передавать открытый класс`CardManager`, поскольку вся информация об адресе объекта `iCard*` является общедоступной и не обязательно должна быть закрытым членом класса; когда класс инициализируется извне Передача объектов класса может сэкономить память.

`void Player::set(iPlayer*врага)` - вызовите этот интерфейс, чтобы установить воображаемого врага противника самостоятельно

`iPlayer* getenemy(void) const` - вызовите этот интерфейс, чтобы вернуть своего воображаемого врага для уведомления

Поскольку все компоненты инициализируются в памяти кучи в конструкторе класса, их необходимо освободить в деструкторе класса:

```
Player::~Player(void) {
	// Release all created resources
	delete this->analyser;
	delete this->ranker;
	delete this->counter;
	delete this->we;
	delete this->enemy;
	delete this->thinker;
}
```

Кроме того, после отправки карты в колоде, если вы по-прежнему объявляете `INeedCard`, она будет продолжать выдавать карту в` Dealer::GetPas() `объекту, что приведет к внутренней ошибке. Нам нужно установить флаг (`Player::ending`) и объявить, что карта больше не нужна, когда мы обнаружим, что карта недоступна:

```
// Return if we need cards
bool Player::INeedCard(void) {
	// If there is no card for us - ending status
	if (ending) return false;

	if (this->we->total() < global::fullcard)
		return true;
	return false;
}
...
// Take one card
void Player::TakeOneCard(Card*& nc) {
	iCard* target = icard(this->manager, nc);

	// While there is no card for us
	if (target == nullptr) {
		this->ending = true;
		return;
	}

	this->cardmap[target] = nc;
	this->we->get(target);

	// Notify enemy that we get one card
	this->weinenemy->replenish();
}
```

При инициализации игры, из-за вышеуказанных характеристик, нам нужно выполнить еще два шага, чтобы инициализировать `CardManager` и установить воображаемого врага:

```
Card* trump = Dealer::GetTrump();
CardManager manager;
std::cout << "Trump: " << *icard(&manager, trump) << std::endl;
Card* temp;
Player p1("Konstantin", &manager);
Player p2("Grigory", &manager);
	
p1.setwe(p2.getenemy()); 
p2.setwe(p1.getenemy());
```

## Резюме

В соответствии с приведенным выше алгоритмом мы можем динамически анализировать текущую ситуацию, чтобы получить относительно разумный план атаки / защиты, и, регулируя параметры настройки в `header.h`, мы можем динамически изменять поведение анализатора.

Ниже приведен снимок экрана с результатом самообороны в 100 странах и частью прогнозирования, когда прогнозируемая глубина равна `3`:

![processing](images/processing.png)

Результаты противостояния:

![result](images/result.png)