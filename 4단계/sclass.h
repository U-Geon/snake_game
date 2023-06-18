# include "vector.h"
#include <ctime>


int countA;
int countP;
int countG;

time_t startAT;
time_t nowAT;
time_t startPT;
time_t nowPT;
int itemT = 5;

#define MAXlen 10

using namespace std;
int map[30][60];

class Snake
{
	private:
		vector<Vector> body;
		Vector apple;
		Vector poison; // poison 추가
		Vector wall1[1000];
        Vector gate[2];
		Vector direction;
        int wallC;
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
        {   init(map);
			getApple(map); 
			getPoison(map);
            checkWall(map);
            getGate(wall1);
			body.push_back(Vector(width/2, height/2));
            body.push_back(Vector(width/2+1, height/2));
            body.push_back(Vector(width/2+2, height/2)); 
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

        void init(int map[30][60]){
            for(int i=0; i<30; i++){
                for(int j=0; j<60; j++){
                    map[i][j] = map1[i][j];
                }
                
            }
        }

        void mapChange(int next_map[30][60]){
            for(int i=0; i<30; i++){
                for(int j=0; j<60; j++){
                    map[i][j] = next_map[i][j];
                }
            }
            checkWall(next_map);
        }
        void getGate(Vector wall[])
        {
            int A = rand()%164;
            int B = rand()%164;
            if(B == A) getGate(wall1);
            gate[0] = wall1[A];
            gate[1] = wall1[B];
        }
		void getApple(int map[30][60])
		{	
			startAT = time(NULL);

			countA++;
			int x = rand()%width;
			int y = rand()%height;
			apple = Vector(x,y);
			for(unsigned int i=0;i<body.size();++i)
				if(apple==body[i])getApple(map);
            if(map[y][x]!=0) getApple(map);
                
		}

		
		void getPoison(int map[30][60])  // poison 위치 설정
		{	
			startPT = time(NULL);
			
			countP++;
            int x,y;
			x = rand()%width;
			y = rand()%height;
			poison = Vector(x,y);
			for(unsigned int i=0;i<body.size();++i)
				if(poison==body[i])getPoison(map);
            
            if(map[y][x]!=0) getPoison(map);
                
		}

        void checkWall(int map[30][60]) //벽 위치 벡터배열로 생성
		{

            wallC =0;
			for(int i=0; i<30; i++){
                for(int j=0; j<60; j++)
                    if(map[i][j]==1){
                    wall1[wallC++] = Vector(j,i);            
                }
            }    
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

			/*아이템을 먹을때마다 startT 갱신해줌.
			메인 while문에서 계속 checkForApple을 호출해주며
			nowT를 갱신해줌. 만약 nowT - startT가 5초 이상이
			될 경우 getApple함수를 재호출해서 새로 아이템 생성*/

			nowAT = time(NULL);
			
			if(nowAT - startAT > itemT){
				getApple(map);
			}
			if(body[0]==apple)
			{
				points++;
				if(points>best)best = points;
				getApple(map);
				snakeSwap(body.size());
			}
		}
		void checkForPoison() // 길이 줄이고 포인트 감소
		{	

			nowPT = time(NULL);
			
			if(nowPT - startPT > itemT){
				getPoison(map);
			}

			if(body[0]==poison)
			{
				points--;
				getPoison(map);
				snakeSwap2(body.size());
			}
		}

    
         void checkForwall()
		{
            for(int i=0; i<wallC; i++){
                if(body[0]==wall1[i] && body[0] != gate[0] && body[0]!= gate[1])
                    exit = true;
            }
		}

        void checkGate()
        {
            if(body[0] == gate[0]){
                body[0] = gate[1];
                
                setDirection(gateDirection(gate[1]));
                
                getGate(wall1);
            }

            else if(body[0] == gate[1]){
                body[0] = gate[0];
                setDirection(gateDirection(gate[0]));
                getGate(wall1);
            }
                
        }
    

        int gateDirection(Vector gate){
            Vector A = gate + Vector(0,-1) ;
            if(map[A.getY()][A.getX()]==0) return 0;
            Vector B = gate + Vector(1,0) ;
            if(map[B.getY()][B.getX()]==0) return 1;
            Vector C = gate + Vector(0,1) ;
            if(map[C.getY()][C.getX()]==0) return 2;
            Vector D = gate + Vector(-1,0) ;
            if(map[D.getY()][D.getX()]==0) return 3;

            return -1;
        }
		void checkPoints()
		{
			switch(points){
				case 1: {
                    level = 2; 
                    mapChange(map2);
                    break;
                    }
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

        char* checkMap(){
            if(level == 1){return getTable(map1);}
            if(level == 2){return getTable(map2);}
            return 0;
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
		char* getTable(int map[30][60])
		{
			std::memset(table, ' ', height*width);

			table[body[0].getY()*width+body[0].getX()] = 'h';
			for(unsigned int i=1; i<body.size(); ++i)
				table[body[i].getY()*width+body[i].getX()] = 'b';
            
			table[apple.getY()*width+apple.getX()] = 'a';
			table[poison.getY()*width+poison.getX()] = 'x';  //poison
           
            for(unsigned int i=0; i<30; i++)
            {
                for(int j=0; j<60; j++){
                    if(map[i][j]==1) table[i*width+j] = '1' ;
                    else if(map[i][j]==2) table[i*width+j] = '2' ;
                    else if (map[i][j]==3) table[i*width+j] = '3';
                }
                
            }
             table[gate[0].getY()*width+ gate[0].getX()] = 'G';
            table[gate[1].getY()*width+ gate[1].getX()] = 'G';

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
