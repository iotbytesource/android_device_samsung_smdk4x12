#include <stdio.h>
#include <stdlib.h>
#include "ui.h"
#include "event.h"
#include "board_test.h"
#include "console.h"

void reboot_work(void)
{
	system("su");
	system("reboot");
}

static struct win main_win =
{
	.title = "engmode",
	.cur = 0,
	.n = 5,
	.menu[0] = {"Board Test", board_test_win_work},
	.menu[1] = {"RF Test", NULL},
	.menu[2] = {"Current Test", NULL},
	.menu[3] = {"Erase Userdata", NULL},
	.menu[4] = {"Reboot", reboot_work},
};

void main_win_work(struct win* win)
{
	struct event ev;
	draw_win(win);
	while(0 == dequeue_event_locked(&ev))
	{
		switch (ev.type) {
			case KEY_VOLUMEDOWN:
			case HOST_EV_DOWN:
				update_win(win, win->cur, win->cur-1);
				continue;
			case KEY_VOLUMEUP:
			case HOST_EV_UP:
				update_win(win, win->cur, win->cur+1);
				continue;
			case KEY_POWER:
			case HOST_EV_ENTER:
				if (win->menu[win->cur].action != NULL) {
					win->menu[win->cur].action();
					/* when menu action returns, we should redraw the windown */
					draw_win(win);
				}
			default:
				draw_win(win);
				continue;
		}
	}
}

static void welcome(void)
{
	print("****************\n");
	print("engmode start up\n");
	print("****************\n");
}

int main()
{
	/*
	 * console should be the first,
	 * make sure do console_exit when system out
	 * */
	console_init();
	welcome();
	event_queue_init_need(1024);
	ui_init();
	event_init();

	main_win_work(&main_win);

	event_queue_uninit();
	console_exit();
	return 0;
}

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

/* for test only  */
int _main(int argc, char** argv)
{
	#if 0
	int fd = fopen("/dev/ttySAC2", O_WRONLY);

	if (argc < 2)
		return -1;

	write(argv[1], strlen(argv[1]), fd);

	return 0;
	#endif
	int i = 10;
	while(i --)
		system("echo engmode print > /dev/ttySAC3");

	return 0;


}
