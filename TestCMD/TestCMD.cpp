#include <iostream>
#include <string>
#include <curses.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <unistd.h>

using namespace std;

WINDOW* win;
WINDOW* output_win;
WINDOW* input_win;
int row = 0, col = 0;
std::atomic<bool> flag(false);
string buf;

void ninit()
{
	win = initscr();
	getmaxyx(win, row, col);

	cbreak();
	noecho();

	nonl();
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
	refresh();
}

void nprintf(string str)
{
	touchwin(win);
	str += '\n';
	wprintw(output_win, str.c_str());
	wrefresh(output_win);
}

void nprintf(const char* fmt, ...)
{
	touchwin(win);

	va_list ap;
	va_start(ap, fmt);
	vw_printw(output_win, fmt, ap);
	va_end(ap);

	wrefresh(output_win);
}

void nmonitor()
{
	while (1)
	{
		char x = getch();

		if (x != '\r')
		{
			touchwin(win);
			buf += x;
			waddch(input_win, x);
		}
		else
		{
			nprintf(buf);
			touchwin(input_win);
			flag = true;
			wclear(input_win);
		}
		wrefresh(input_win);
	}
}

string nget()
{
	while (!flag)
		usleep(100);
	string cmd = buf;
	flag = false;
	buf = "";
	return cmd;
}

////////////////////////////////

void print_thread()
{
	while (1)
	{
		static int i = 0;
		nprintf("no.%d\n", i++);
		usleep(100000);
	}
}

int main()
{
	ninit();
	fflush(stdin);

	output_win = subwin(win, row - 1, col, 0, 0);
	scrollok(output_win, true);
	input_win = subwin(win, 1, col, row - 1, 0);

	std::thread pthr(print_thread);
	std::thread nthr(nmonitor);

	string cmd;
	while (1)
	{
		cmd = nget();
		if (cmd == "quit")
			break;
		else
			nprintf("[info] You input: %s\n", cmd.c_str());
	}

	getch();

	endwin();
}