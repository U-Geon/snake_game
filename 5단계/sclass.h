# include "vector.h"
#include <ctime>


int countA; // apple 갯수
int countP; // poison 갯수
int countG; // gate 갯수
int countB; // MAX 도달 횟수
int countW; // 맵에서 1 갯수
char comA; //v표시 해줄거
char comP;
char comG;
char comB;
int missionA; // 미션 설정
int missionP;
int missionB;
int missionG;

time_t nowGameT;
time_t startAT;
time_t nowAT;
time_t startPT;
time_t nowPT;
int itemT = 6;



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
		int points;
		int level;
		int best;
		const int height;
		const int width;
		bool exit;
		int speed;
		char* table;
		bool end;
		
	public:
		Snake(int _height, int _width)
		: height(_height), width(_width)
        {   init(map);
			dropApple(map); 
			dropPoison(map);
            checkWall(map);
            getGate(wall1);
			body.push_back(Vector(width/2, height/2));
            body.push_back(Vector(width/2+1, height/2));
            body.push_back(Vector(width/2+2, height/2)); 
			setDirection(0); 
			exit=false; 
			speed=100000; 
			level=1;
			missionA = 1;
			missionB = 5;
			missionP = 1;
			missionG = 1;
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
		// void ChangeLevel(){
		// 	if(level != changeLevel){
		// 		changeLevel = level;
		// 		dropApple(map);
		// 		dropPoison(map);
		// 		getGate(wall1);
				
		// 		}
			
			
		// }

        void mapChange(int next_map[30][60]){ // 맵 바꿔주기
            for(int i=0; i<30; i++){
                for(int j=0; j<60; j++){
                    map[i][j] = next_map[i][j];
                }
            }
            checkWall(next_map);
			
        }
		
        void getGate(Vector wall[]) // 게이트 위치 정해주기
        {
            int A = rand()%countW;
            int B = rand()%countW;
            if(B == A) getGate(wall1);
            gate[0] = wall1[A];
            gate[1] = wall1[B];
        }
		void dropApple(int map[30][60]) // 애플 놓기
		{	
			startAT = time(NULL);

			int x = rand()%width;
			int y = rand()%height;
			apple = Vector(x,y);
			for(unsigned int i=0;i<body.size();++i)
				if(apple==body[i])dropApple(map);
            if(map[y][x]!=0) dropApple(map);
                
		}

		void getSize(){ //3이하로 뱀 크기 떨어지면 죽음
			if(body.size()<3){exit = true;}
		}

		
		void dropPoison(int map[30][60])  // poison 위치 설정
		{	
			startPT = time(NULL);
			
            int x,y;
			x = rand()%width;
			y = rand()%height;
			poison = Vector(x,y);
			for(unsigned int i=0;i<body.size();++i)
				if(poison==body[i])dropPoison(map);
            
            if(map[y][x]!=0) dropPoison(map);
                
		}

        void checkWall(int map[30][60]) //벽 위치 벡터배열로 생성
		{

            countW =0;
			for(int i=0; i<30; i++){
                for(int j=0; j<60; j++)
                    if(map[i][j]==1){
                    wall1[countW++] = Vector(j,i);            
                }
            }    
		}

		void setDirection(int d) // 방향 설정
		{
			switch(d)
			{
				case 0:direction = Vector(0, -1); break;
				case 1:direction = Vector(1, 0); break;
				case 2:direction = Vector(0, 1); break;
				case 3:direction = Vector(-1, 0); break;
			}
		}

		//맵 다음단계 되면 다시3개로 줄여줌
		void changeBody(){
			
			for(int i=0; i<body.size()-3; i++){
				reduceBodySize(body.size());
			}
			
		}
		
		void makeMove() // 이동
		{
			if(!exit)body[0]+=direction;

			for(unsigned int i = 1; i<body.size(); ++i)
				if(body[i]==body[0]){exit = true; break;}
		}
		void bodyMove()
		{
			for(unsigned int i=body.size()-1; i>0; --i)
				increaseBodySize(i);
		}
		void checkForApple()
		{

			/*아이템을 먹을때마다 startT 갱신해줌.
			메인 while문에서 계속 checkForApple을 호출해주며
			nowT를 갱신해줌. 만약 nowT - startT가 5초 이상이
			될 경우 dropApple함수를 재호출해서 새로 아이템 생성*/

			nowAT = time(NULL);
			
			if(nowAT - startAT > itemT){
				dropApple(map);
			}
			if(body[0]==apple)
			{
				countA++; //수정
				points++;
				if(points>best)best = points;
				dropApple(map);
				increaseBodySize(body.size());
			}
		}
		void checkForPoison() // 길이 줄이고 포인트 감소
		{	

			nowPT = time(NULL);
			
			if(nowPT - startPT > itemT){
				dropPoison(map);
			}

			if(body[0]==poison)
			{	countP++; //수정
				points--;
				dropPoison(map);
				reduceBodySize(body.size());
			}
		}

    
         void checkForwall() // 벽에 닿으면 죽음
		{
            for(int i=0; i<countW; i++){
                if(body[0]==wall1[i] && body[0] != gate[0] && body[0]!= gate[1]){
					exit = true;
				}
                    
            }
		}

        void checkGate() // 몸이 다 통과할때까지 머리 방향 설정
        {
            if(body[0] == gate[0]){
                body[0] = gate[1];
                countG++;
                setDirection(gateDirection(gate[1]));
            }

            else if(body[0] == gate[1]){
				countG++;
                body[0] = gate[0];
                setDirection(gateDirection(gate[0]));
            }
                
        }
		void changeGate() // 다 통과해야 게이트 바뀌도록
        {
			Vector A = direction;
			for(int i=0; i<body.size(); i++){
				A = A + direction;

			}
            if(body[0] == gate[0]+A){
				getGate(wall1);
            }

            else if(body[0] == gate[1]+A){
				getGate(wall1);
            }
                
        }
    

        int gateDirection(Vector gate){ //벽 없는 쪽으로 머리 방향 설정
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
		void mission(){ // 미션 완료시 v표시
			if(missionA <= countA){
				comA = 'v';
			}
			else comA = ' ';
			if(missionP <= countP){
				
				comP = 'v';
			}
			else comP = ' ';
			if(missionG <= countG){
		
				comG = 'v';
			}
			else comG = ' ';
			if(missionB <= body.size()){
		
				comB = 'v';
			}
			else comB = ' ';
		}
		int missionComplete(){
			if(comP == 'v' && comA == 'v' && comG == 'v' && comB == 'v' ) {
				level ++;
				return level;
			}
			return level;
		}
		bool allComplete(){
			if(level > 5){
				exit = true;
				return true;
			}
			return false;
			}

		void checkPoints() // 미션 완료시 맵 바꾸고 미션 스코어 변경
		{
			switch(missionComplete()){
				case 2: {
					missionA =2;
					missionB =4;
					missionP =2;
					missionG =2;
					mapChange(map2);
					break;}
				case 3: {
					missionA =3;
					missionB =5;
					missionP =3;
					missionG =3;
					mapChange(map3); break;}
				case 4: {
					missionA =4;
					missionB =6;
					missionP =4;
					missionG =4;
					mapChange(map4); break;}
				case 5: {
					missionA =5;
					missionB =7;
					missionP =5;
					missionG =5;
					mapChange(map5); break;}				
			}
		}

        char* checkMap(){ // 맵바꾸기
            if(level == 1){return getTable(map1);}
            if(level == 2){return getTable(map2);}
			if(level == 3){return getTable(map3);}
            if(level == 4){return getTable(map4);}
			if(level == 5){return getTable(map5);}
            return getTable(map5);
        }
		void increaseBodySize(uint i)
		{
			if(i >= body.size())
				body.push_back(body.back());
			else
				body[i] = body[i - 1];
		}

		void reduceBodySize(uint i)
		{
			if(i >= body.size())
				body.pop_back();
			else
				body[i] = body[i - 1];
		}
		int getBodySize(){return body.size();}
		int getPoints() {return points;}
		int getLevel() {return level;}
		bool getExit() {return exit;}
		bool getEnd() {return end;}
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
			if(direction.getY()==-1)
				return 0;
			else if(direction.getX()==1)
				return 1;
			else if(direction.getY()==1)
				return 2;
			else 
				return 3;
		}
};