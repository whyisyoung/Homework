#include <curses.h>
#include <signal.h>
#include <sys/time.h>
#include <stdlib.h>
#include "pong.h"

int main()
{
	init();

	play_game();

	while(update_status())
		draw();

	quit_game();
}

void init()
{
	create_window();
	init_ball_and_guard();
	draw();
}

void create_window()
{
	initscr();

	/* disable line buffering */
	cbreak();

	/*enables the reading of function keys like F1, arrow keys etc*/
	keypad(stdscr, TRUE);

	noecho();

	signal(SIGQUIT, SIG_IGN); /* ignore "ctrl+\" signal */
}

void init_ball_and_guard()
{
	int init_y_ttm, init_x_ttm;
	/* randomize x_ttm and y_ttm to range(9..12) */
	init_y_ttm = (rand() % (MAX + 1 - MIN)) + MIN;
	init_x_ttm = init_y_ttm;

	ball.y_pos = Y_INIT;
	ball.x_pos = X_INIT;
	ball.y_ttm = ball.y_ttg = init_y_ttm;
	ball.x_ttm = ball.x_ttg = init_x_ttm;
	ball.y_dir = 1;
	ball.x_dir = 1;
	ball.symbol = BALL_SYMBOL;

	guard_pos = 28;
	guard_len = 10;
	score     = 0;
}

void draw()
{
	clear();

	/* information */
	mvprintw(0, 5, "Score: %d\n     Guard operation: four arrow keys", score);
	mvprintw(2, 5, "-speed:s/S, +speed: k/K");

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

	move(LINES - 1, COLS - 1);

	refresh();
}

int update_status()
{
	int input, dir = 0;

	//play_game();

	switch(getch()) {
	case 'K':
		ball.y_ttm--; /* 增加小球在y轴的移动速度 */
		break;
	case 'S':
		ball.y_ttm++; /* 减少小球在y轴的移动速度 */
		break;
	case 'k':
		ball.x_ttm--; /* 增加小球在x轴的移动速度 */
		break;
	case 's':
		ball.x_ttm++; /* 减少小球在x轴的移动速度 */
		break;
	case KEY_LEFT:	      /*左移挡板*/
		dir = -1;
		change_guard_pos_by(dir);
		break;
	case KEY_RIGHT:       /*右移挡板*/
		dir = 1;
		change_guard_pos_by(dir);
		break;
	case KEY_UP:          /* 上键增加挡板长度 */
		if(guard_pos + guard_len - 1 > RIGHT_EDGE)
			guard_len = RIGHT_EDGE + 1 - guard_pos;
		if(++guard_len >= 25)
			guard_len = 25;
		break;
	case KEY_DOWN:	      /* 下键减短挡板长度 */
		if(--guard_len <= 2)
			guard_len = 2;
		break;
	case 'q':
		return FALSE;
	default:
		break;
	}

	return TRUE;
}

void play_game()
{
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

void bounce_or_lose(struct pong_ball *bp)
{
	int guard_left  = guard_pos;
	int guard_right = guard_pos + guard_len - 1;

	/* bounce left or right wall and back */
	if(bp->x_pos == LEFT_EDGE)
		bp->x_dir = 1;
	else if(bp->x_pos == RIGHT_EDGE)
		bp->x_dir = -1;

	/* bounce the top wall and back */
	if(bp->y_pos == TOP_ROW)
		bp->y_dir = 1;
	else if(bp->y_pos == BOT_ROW &&
		    (bp->x_pos >= guard_left && bp->x_pos <= guard_right)) {
		bp->y_dir = -1;
		mvprintw(0, 5, "Score: %d", ++score);
	}

	/* lose */
	if(bp->y_pos > BOT_ROW)
	 	game_over();
}

int game_over()
{
	char c;

	set_ticker(0);
	mvaddstr(10, 30, "You lose!");
	mvaddstr(11, 30, "Retry?(y/n)");
	refresh();
	while((c = getch()) != 'q') {
		if(c == 'y') {
			clear();
			init();
			play_game();
			return TRUE;
		}
		else if(c == 'n') {
			quit_game();
			return TRUE;
		}
	}

	return TRUE;
}

void change_guard_pos_by(int dir)
{
	bool can_move = true;
	int guard_right = guard_pos + guard_len - 1;
	/* sometimes you can not move guard. */
	if(guard_pos == LEFT_EDGE && dir == -1)
		can_move = false;
	else if(guard_right == RIGHT_EDGE && dir == 1)
		can_move = false;

	if(can_move)
		guard_pos += dir;
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

void quit_game()
{
	set_ticker(0); /* turn off ticker */
	endwin();
	exit(0);
}
