#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
using std::fstream; using std::ifstream; using std::ofstream;

#include <ncurses.h>
#include <unistd.h>	//usleep(microseconds);
#include <sys/types.h>
#include <pwd.h>	//To get home directory

int countA;
int countP;
int countG;
#define MAXlen 10
class Vector
{
	public:
		Vector() : x(0), y(0){}
		Vector(int x, int y) {setX(x); setY(y);}

		void setX(int x){this->x = x;}
		int getX() const {return this->x;}
		void setY(int y){this->y = y;}
		int getY() const {return this->y;}

		Vector& operator=(const Vector& vec)
		{setX(vec.getX()); setY(vec.getY()); return *this;}
		Vector& operator+=(const Vector& vec)
		{setX(getX()+vec.getX()); setY(getY()+vec.getY()); return *this;}
		Vector operator+(const Vector& vec)
		{Vector v(getX(),getY()); return v+=vec; }
		Vector& operator-=(const Vector& vec)
		{setX(getX()-vec.getX()); setY(getY()-vec.getY()); return *this;}
		Vector operator-(const Vector& vec)
		{Vector v(getX(),getY()); return v-=vec;}
		bool operator==(const Vector& vec)
		{return getX()==vec.getX() && getY()==vec.getY();}
		bool operator!=(const Vector& vec)
		{return !operator==(vec);}
			
	private:
		int x,y;
};

class Snake
{
	private:
		std::vector<Vector> body;
		Vector apple;
		Vector poison; // poison 추가
		Vector direction;
		int points;
		int level;
		int best;
		const int height;
		const int width;
		bool exit;
		int speed;
		char* table;
	public:
		Snake(int _height, int _width, int _best)
		: best(_best), height(_height), width(_width)
		{	getApple(); 
			getPoison();
			body.push_back(Vector(width/2, height/2)); 
			setDirection(0); 
			exit=false; 
			speed=80; 
			level=1;
			points=0;
			srand(time(NULL));
			table = new char[height*width];
		}
		~Snake()
		{delete [] table;}

		void getApple()
		{	
			countA++;
			int x = rand()%width;
			int y = rand()%height;
			apple = Vector(x,y);
			for(unsigned int i=0;i<body.size();++i)
				if(apple==body[i])getApple();
		}
		void getPoison()  // poison 위치 설정
		{
			countP++;
			int x = rand()%width;
			int y = rand()%height;
			poison = Vector(x,y);
			for(unsigned int i=0;i<body.size();++i)
				if(poison==body[i])getPoison();
		}

		void setDirection(int d)
		{
			switch(d)
			{
				case 0:direction = Vector(0, -1); break;
				case 1:direction = Vector(1, 0); break;
				case 2:direction = Vector(0, 1); break;
				case 3:direction = Vector(-1, 0); break;
			}
		}
		
		void makeMove()
		{
			exit = false;
			if(body[0].getX()<0 || body[0].getX()>=width)
				exit = true;
			else if(body[0].getX()==0)
			{	if(direction==Vector(-1,0))
					exit = true;
			}
			else if(body[0].getX()==(width-1))
			{	if(direction==Vector(1,0))
					exit = true;
			}
			
			if(body[0].getY()<0 || body[0].getY()>=height)
				exit = true;
			else if(body[0].getY()==0)
			{
				if(direction==Vector(0,-1))
					exit = true;
			}
			else if(body[0].getY()==(height-1))
			{
				if(direction==Vector(0,1))
					exit = true;
			}

			if(!exit)body[0]+=direction;

			for(unsigned int i = 1; i<body.size(); ++i)
				if(body[i]==body[0]){exit = true; break;}
		}
		void bodyMove()
		{
			for(unsigned int i=body.size()-1; i>0; --i)
				snakeSwap(i);
		}
		void checkForApple()
		{
			if(body[0]==apple)
			{
				points++;
				if(points>best)best = points;
				getApple();
				snakeSwap(body.size());
			}
		}
		void checkForPoison() // 길이 줄이고 포인트 감소
		{
			if(body[0]==poison)
			{
				points--;
				getPoison();
				snakeSwap2(body.size());
			}
		}
		void checkPoints()
		{
			switch(points){
				case 3: level = 2; speed = 98; break;
				case 5: level = 3; speed = 85; break;
				case 7: level = 4; speed = 80; break;
				case 9: level = 5; speed = 60; break;
				case 50: level = 6; speed = 80; break;
				case 60: level = 7; speed = 65; break;
				case 70: level = 8; speed = 50; break;
				case 80: level = 9; speed = 40; break;
				case 90: level = 10; speed = 30; break;
				case 100: level = 999; speed = 15; break;
			}
		}
		void snakeSwap(uint i)
		{
			if(i >= body.size())
				body.push_back(body.back());
			else
				body[i] = body[i - 1];
		}

		void snakeSwap2(uint i)
		{
			if(i >= body.size())
				body.pop_back();
			else
				body[i] = body[i - 1];
		}
		int getPoints() {return points;}
		int getLevel() {return level;}
		int getBest() {return best;}
		bool getExit() {return exit;}
		void setExit(bool e) {exit = e;}
		int getSpeed() {return speed;}
		int getHeight() {return height;}
		int getWidth() {return width;}
		char* getTable()
		{
			std::memset(table, ' ', height*width);

			table[body[0].getY()*width+body[0].getX()] = 'h';
			for(unsigned int i=1; i<body.size(); ++i)
				table[body[i].getY()*width+body[i].getX()] = 'b';
			table[apple.getY()*width+apple.getX()] = 'a';
			table[poison.getY()*width+poison.getX()] = 'x';  //poison


			return table;
		}
		int getDirection()
		{
			if(direction.getX()==1)
				return 1;
			else if(direction.getX()==-1)
				return 3;
			else if(direction.getY()==-1)
				return 0;
			else 
				return 2;
		}
};

//------------------------------------
int getBest();
void writeBest(int best);
bool writeEndAndGetInput();
void printScore(WINDOW*, int, int, int);
void printMission(WINDOW*, int, int, int);
void draw(WINDOW*, Snake&, char*, int, int);
void proccesInput(WINDOW*, Snake&, int);
int main()
{
	countA = 0;
	countP = 0;
	countG = 0;
	initscr();
	noecho();
	cbreak();
	int x,y;
	// getmaxyx(stdscr, y, x);
	x = 100;
	y = 30;
	int best = getBest();
	//render frame
	WINDOW *win = newwin(y, x-41, 0, 0); //height, width, startY, startX
	start_color();
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);
	wbkgd(win, COLOR_PAIR(1));
    wattron(win, COLOR_PAIR(1));
	wborder(win, '|','|','-','-','+','+','+','+');
	nodelay(win, TRUE);
	keypad(win, TRUE);

	WINDOW *score = newwin(y/2, x-60, 0,x-37);
	wbkgd(score, COLOR_PAIR(1));
    wattron(score, COLOR_PAIR(1));
	wborder(score, '|','|','-','-','+','+','+','+');
	wrefresh(score);
	nodelay(score, TRUE);

	WINDOW *mission = newwin(y/2, x-60, y/2,x-37);
	wbkgd(mission, COLOR_PAIR(1));
    wattron(mission, COLOR_PAIR(1));
	wborder(mission, '|','|','-','-','+','+','+','+');
	wrefresh(mission);
	nodelay(mission, TRUE);

	
	//game loop
	do
	{
		Snake snake(y, x-41, best);
		refresh();
		wrefresh(win);
		printScore(score, 0, 1, best);
		printMission(mission,0,1,best);
		countA = 0;
		countP = 0;
		countG = 0;
	
		while(!snake.getExit())
		{
			char *tbl = snake.getTable();
			//process data
			//draw
			draw(win, snake, tbl, snake.getHeight(), snake.getWidth());
			
			printScore(score, snake.getPoints(), snake.getLevel(), snake.getBest());
			printMission(mission, snake.getPoints(), snake.getLevel(), snake.getBest());
			
			int input = wgetch(win);
			proccesInput(win, snake, input);
			if(snake.getExit())break;

			snake.bodyMove();
			snake.makeMove();
			snake.checkForApple();
			snake.checkForPoison();
			snake.checkPoints();

			//-----[ SLEEP ]-------
			#ifdef WIN32
			Sleep(snake.getSpeed());
			#else
			usleep(snake.getSpeed() * 1000);
			#endif
			//---------------------
		}	
		if(snake.getBest()>best){writeBest(snake.getBest()); best = snake.getBest();}
	}while(writeEndAndGetInput());
	
	delwin(score);
	delwin(win);
	delwin(mission);
	endwin();
}
std::string getFile()
{
	#ifdef WIN32
	std::string home = "%appdata%/.md.snake\0";
	#else
	const char *homedir;

	if ((homedir = getenv("HOME")) == NULL) 
	    homedir = getpwuid(getuid())->pw_dir;
	
	std::string home = std::string(homedir)+"/.md.snake\0";
	#endif
	return home;
}
int getBest()
{
	ifstream fp (getFile().c_str());
	int best = 0;
	std::string content;
	if(fp.is_open() && fp.good())
	{
		getline(fp, content); 
		best = strtol(content.c_str(), NULL, 10);
		fp.close();
	}
	return best;
}
void writeBest(int best)
{
	ofstream fp (getFile().c_str());
	if(fp.is_open())
	{
		fp << best;
		fp.close();
	}
}
bool writeEndAndGetInput()
{
	WINDOW* endwin = newwin(4,COLS/2, LINES/2, 2);
	box(endwin, 0, 0);
	nodelay(endwin, TRUE);
	keypad(endwin, TRUE);
	mvwprintw(endwin, 1, 1, "Press [Spacebar]/[Enter] to play again.");
	mvwprintw(endwin, 2, 1, "Press [q] to quit.");
	int c;
	do{
		c = wgetch(endwin);
	}while(c!=10 && c!=' ' && c!='q' && c!='Q');
	werase(endwin);
	wrefresh(endwin);
	delwin(endwin);
	return (c=='q' || c=='Q')?false:true;
}
void printScore(WINDOW* w, int score, int level, int best)
{
	werase(w);
	wbkgd(w, COLOR_PAIR(1));
    wattron(w, COLOR_PAIR(1));
	wborder(w, '|','|','-','-','+','+','+','+');
	 mvwprintw(w, 1, 2, "Score Board"); 
    mvwprintw(w, 4, 2, "B: Current Length / Max Length %d/%d",score,MAXlen);
    mvwprintw(w, 6, 2, "+: Number of Growth Items %d",countA ); 
    mvwprintw(w, 8, 2, "-: Number of Poison Items %d",countP); 
    mvwprintw(w, 10, 2, "G: Number of gate used %d",countG); 
	wrefresh(w);
}

void printMission(WINDOW* w, int score, int level, int best)
{
	werase(w);
	wbkgd(w, COLOR_PAIR(1));
    wattron(w, COLOR_PAIR(1));
	wborder(w, '|','|','-','-','+','+','+','+');
    mvwprintw(w, 1, 2, "Mission Board"); 

    mvwprintw(w, 4, 2, "B: 10  (%d)",score/5); 
    mvwprintw(w, 5, 2, "+: 5   (%d)",countA); 
    mvwprintw(w, 6, 2, "-: 2   (%d)",countP); 
    mvwprintw(w, 7, 2, "G: 1   (%d)",countG); 
	wrefresh(w);
}
void draw(WINDOW* win, Snake& snake, char* table, int height, int width)
{
	werase(win);
	wbkgd(win, COLOR_PAIR(1));
    wattron(win, COLOR_PAIR(1));
	wborder(win, '|','|','-','-','+','+','+','+');
	for(int i=0; i<(height*width); ++i)
	{
		if(table[i]!=' ')
		{
			int y = i/width;
			int x = i-(y*width);
			int ch;
			int d;
			switch(table[i])
			{
				case 'a':
					ch = '*';
					break;
				case 'x':
					ch = 'x';
					break;
				case 'h':
					
					ch = 'O';
				
					break;
				case 'b':
					ch = 'o';
					break;
			}
			mvwaddch(win, 1+y,1+x, ch);
		}
	}
	wrefresh(win);
}
void proccesInput(WINDOW* win, Snake& snake, int input)
{
	int d = snake.getDirection();
	switch(input)
	{
		case KEY_UP:
			if(d == 2) {
				snake.setExit(true);
				break;}
			if(d!=0 && !(d==2 && snake.getPoints() > 0))
			snake.setDirection(0);
			break;
		case KEY_DOWN:
			if(d == 0) {
				snake.setExit(true);
				break;}
			if(d!=2 && !(d==0 && snake.getPoints() > 0))
			snake.setDirection(2);
			break;
		case KEY_LEFT:
			if(d == 1) {
				snake.setExit(true);
				break;}
			if(d!=3 && !(d==1 && snake.getPoints() > 0))
			snake.setDirection(3);
			break;
		case KEY_RIGHT:
			if(d == 3) {
				snake.setExit(true);
				break;}
			if(d!=1 && !(d==3 && snake.getPoints() > 0))
			snake.setDirection(1);
			break;
		case 'Q':
		case 'q':
			snake.setExit(true);
			break;
		case 'P':
		case 'p':
			char c;
			wattron(win, A_BOLD);
			mvwprintw(win, snake.getHeight()/2, snake.getWidth()/2, "PAUSE");
			wattroff(win, A_BOLD);
			nodelay(win, FALSE);
			do{
				c = wgetch(win);
			}while(c!='p' && c!='P');
			nodelay(win, TRUE);
			break;
	}
}
