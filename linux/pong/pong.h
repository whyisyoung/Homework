/*
 * Note:
 * Y axis is row
 * X axis is col
 */

#define BLANK 	     ' '
#define BALL_SYMBOL  'o'
#define WALL 	       ' '
#define GUARD	       '='
#define TOP_ROW 	    5
#define	BOT_ROW 	    20
#define	LEFT_EDGE	    10
#define	RIGHT_EDGE	  70
#define	Y_INIT		    10  /* starting row  */
#define	X_INIT		    10  /* starting col  */
#define	TICKS_PER_SEC	40 /* affects speed, bigger means faster */

#define MAX 12 /* max y_ttm and x_ttm */
#define MIN 9  /* min y_ttm and x_ttm */

int guard_pos;
int guard_len;
int score;


struct pong_ball {
	int y_pos, x_pos,
	    y_ttm, x_ttm, /* 间隔，指明两次移动之间要等待的计时信号个数   */
	    y_ttg, x_ttg, /* 当前值，在下次重画之前还要等待多少个计时信号 */
	    y_dir, x_dir; /* 控制字符串移动的速度 */
	char symbol;
}ball;

void init();
void create_window();
void init_ball_and_guard();
void draw();
void play_game();
void ball_move(int);
void quit_game();
void move_guard(int);
void bounce_or_lose(struct pong_ball *bp);
