#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>


/* Y axis is row */
/* X axis is col */

#define BLANK 	       ' '
#define BALL_SYMBOL    'o'
#define WALL 	       '+'
#define GUARD	       '='
#define TOP_ROW 	3
#define	BOT_ROW 	20
#define	LEFT_EDGE	10
#define	RIGHT_EDGE	70
#define	Y_INIT		10  /* starting row  */
#define	X_INIT		10  /* starting col  */
#define	TICKS_PER_SEC	50 /* affects speed, bigger means faster */

#define Y_TTM   8
#define X_TTM   5

int guard_pos = 36;
int guard_len = 10;

struct pong_ball {
	int y_pos, x_pos,
	    y_ttm, x_ttm, /* 间隔，指明两次移动之间要等待的计时信号个数   */
	    y_ttg, x_ttg, /* 当前值，在下次重画之前还要等待多少个计时信号 */
	    y_dir, x_dir; /* 控制字符串移动的速度 */
	char symbol;
}ball;

void init_screen();
void wrap_up();
void move_guard(int);

int main()
{
	int input, dir = 0;

	init_screen();

	while((input = getch()) != 'q') {
		if(input == 'K')      ball.y_ttm--; /* 增加小球在y轴的移动速度 */
		else if(input == 'S') ball.y_ttm++; /* 减少小球在y轴的移动速度 */
		else if(input == 'k') ball.x_ttm--; /* 增加小球在x轴的移动速度 */
		else if(input == 's') ball.x_ttm++; /* 减少小球在x轴的移动速度 */
		else if(input == KEY_LEFT) { /*左移挡板*/
			dir = -1;
			move_guard(dir);
		}
		else if(input == KEY_RIGHT) { /*右移挡板*/
			dir = 1;
			move_guard(dir);
		}
		else if(input == KEY_DOWN) { /* 下键减短挡板长度 */
			if(--guard_len <= 2)
				guard_len = 2;
			refresh();
		}
		else if(input == KEY_UP) {  /* 上键增加挡板长度 */
			if(++guard_len >= 20)
				guard_len = 20;
			refresh();
		}
	}

	wrap_up();
}

void init_screen()
{
	void ball_move(int);

	ball.y_pos = Y_INIT;
	ball.x_pos = X_INIT;
	ball.y_ttm = ball.y_ttg = Y_TTM;
	ball.x_ttm = ball.x_ttg = X_TTM;
	ball.y_dir = 1;
	ball.x_dir = 1;
	ball.symbol = BALL_SYMBOL;

	guard_pos = 36;
	guard_len = 10;

	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	crmode();

	signal(SIGINT, SIG_IGN);
	standout();
	/* draw wall */
	mvhline(TOP_ROW - 1, LEFT_EDGE - 1, WALL, RIGHT_EDGE - LEFT_EDGE + 2);
	mvvline(TOP_ROW - 1, LEFT_EDGE - 1, WALL, BOT_ROW - TOP_ROW + 2);
	mvvline(TOP_ROW - 1, RIGHT_EDGE + 1, WALL, BOT_ROW - TOP_ROW + 2);
	/* draw guard baffer */
	mvhline(BOT_ROW + 1, guard_pos, GUARD, guard_len);
	standend();

	/* draw ball */
	mvaddch(ball.y_pos, ball.x_pos, ball.symbol);

	refresh();

	signal(SIGALRM, ball_move);
	set_ticker(1000 / TICKS_PER_SEC); /* send millisecs per tick */
}

void ball_move(int signum)
{
	int y_cur, x_cur;
	bool can_move;

	signal(SIGALRM, SIG_IGN);  /* dont get caught now */
	y_cur = ball.y_pos;	   /* old spot 		  */
	x_cur = ball.x_pos;
	can_move = false;          /* wait for signal before you can move */

	if(ball.y_ttm > 0 && ball.y_ttg-- == 1) {
		ball.y_pos += ball.y_dir;  /* move  */
		ball.y_ttg = ball.y_ttm;   /* reset */
		can_move = true;
	}

	if(ball.x_ttm > 0 && ball.x_ttg-- == 1) {
		ball.x_pos += ball.x_dir;  /* move  */
		ball.x_ttg = ball.x_ttm;   /* reset */
		can_move = true;
	}

	if(can_move) {
		/* erase current ball and put it to next pos */
		mvaddch(y_cur, x_cur, BLANK);
		mvaddch(y_cur, x_cur, BLANK);
		mvaddch(ball.y_pos, ball.x_pos, ball.symbol);

		bounce_or_lose(&ball);

		move(LINES - 1, COLS - 1);
		refresh();
	}

	signal(SIGALRM, ball_move); /* for unreliable systems */
}

int bounce_or_lose(struct pong_ball *bp)
{
	int bounce = 0;
	int guard_left = guard_pos;
	int guard_right = guard_pos + guard_len - 1;
	char c;

	if(bp->y_pos == TOP_ROW) {
		/* bounce the top wall and back, y_pos should add one */
		bp->y_dir = 1;
		bounce = 1;
	}
	else if(bp->y_pos == BOT_ROW &&
		    (bp->x_pos >= guard_left && bp->x_pos <= guard_right)) {
		bp->y_dir = -1;
		bounce = 1;
	}
	else if(bp->y_pos == BOT_ROW) { /* lose */
		set_ticker(0);
		mvaddstr(13, 30, "You lose!");
		mvaddstr(14, 30, "Retry?(y/n)");
		refresh();
		while((c = getch()) != 'q') {
			if(c == 'y') {
				clear();
				init_screen();
				break;
			}
			else if(c == 'n') {
				wrap_up();
				break;
			}
		}
	}

	/*
	 * bounce the left wall and back, x_pos should add one
	 * similarly with right wall.
	 */
	if(bp->x_pos == LEFT_EDGE) {
		bp->x_dir = 1;
		bounce = 1;
	} else if(bp->x_pos == RIGHT_EDGE) {
		bp->x_dir = -1;
		bounce = 1;
	}

	return bounce;
}

set_ticker(n_msecs)
{
	struct itimerval new_timeset;
	long   n_sec, n_usecs;

	n_sec = n_msecs / 1000;
	n_usecs = (n_msecs % 1000) * 1000L;

	new_timeset.it_interval.tv_sec  = n_sec;
	new_timeset.it_interval.tv_usec = n_usecs;
	new_timeset.it_value.tv_sec 	= n_sec;
	new_timeset.it_value.tv_usec	= n_usecs;

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void wrap_up()
{
	set_ticker(0); /* turn off ticker */
	endwin();
	exit(0);
}

void move_guard(int dir)
{
	bool can_move = true;
	int guard_right = guard_pos + guard_len - 1;
	/**/
	if(guard_pos == LEFT_EDGE && dir == -1)
		can_move = false;
	else if(guard_right == RIGHT_EDGE && dir == 1)
		can_move = false;

	if(can_move) {
		mvhline(BOT_ROW + 1, guard_pos, BLANK, RIGHT_EDGE - LEFT_EDGE + 1);
		standout();
		guard_pos += dir;
		mvhline(BOT_ROW + 1, guard_pos, GUARD, guard_len);
		standend();
		refresh();
	}
}