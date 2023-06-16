#include <ncurses.h>
int main()
{
    //윈도우 세 개 생성
    WINDOW *win1;
    WINDOW *win2;
    WINDOW *win3;
    initscr();

    // resize_term(60, 30); -> 원래는 전체 터미널 사이즈 설정하는건데 이건 나중에 수정해야할것같아서 놔뒀어
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);      //글씨는 흰색, 배경은 파란색
    border('*','*','*','*','*','*','*','*'); mvprintw(1,1,"A default window"); refresh();   //default window 테두리 *
    getch();

    //win1은 뱀이 움직이는 공간
    win1 = newwin(22, 50, 3, 3);
    wbkgd(win1, COLOR_PAIR(1));
    wattron(win1, COLOR_PAIR(1));
    mvwprintw(win1, 1, 1, "Space for snake"); 
    wborder(win1, '@','@','@','@','@','@','@','@'); wrefresh(win1);
    getch();


    //win2는 점수를 표시하는 공간
    win2 = newwin(10, 40, 3, 56);
    wbkgd(win2, COLOR_PAIR(1));
    wattron(win2, COLOR_PAIR(1));
    mvwprintw(win2, 1, 2, "Score Board"); 
    mvwprintw(win2, 4, 2, "B: Current Length / Max Length");
    mvwprintw(win2, 5, 2, "+: Number of Growth Items"); 
    mvwprintw(win2, 6, 2, "-: Number of Poison Items"); 
    mvwprintw(win2, 7, 2, "G: Number of gate used"); 
    wborder(win2, '@','@','@','@','@','@','@','@'); wrefresh(win2);
    getch();


    //win3은 미션을 표시하는 공간
    win3 = newwin(10, 40, 15, 56);
    wbkgd(win3, COLOR_PAIR(1));
    wattron(win3, COLOR_PAIR(1));
    mvwprintw(win3, 1, 1, "Mission Board"); 
    mvwprintw(win3, 4, 1, "B: 10  ( )"); 
    mvwprintw(win3, 5, 1, "+: 5   ( )"); 
    mvwprintw(win3, 6, 1, "-: 2   ( )"); 
    mvwprintw(win3, 7, 1, "G: 1   (v)"); 
    wborder(win3, '@','@','@','@','@','@','@','@'); wrefresh(win3);
    getch();

    delwin(win1);
    endwin();
return 0; }
