#include <ncurses.h>
#include <stdio.h>   
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>

#include "editor_functions.c"

int main(int argc, char *argv[])
{
	int line = 0; // What line are you editing
	int character = 0; // What character are you editing
	int c;
	int position; 
	
	WINDOW *helpWindow;
	WINDOW *renderWindow;
	
	initscr();			/* Start curses mode 		*/
	raw();				/* Line buffering disabled	*/
	keypad(stdscr, TRUE);		/* We get F1, F2 etc..		*/
	noecho();			/* Don't echo() while we do getch */
	
	//create the fullscreen window
	renderWindow = newwin(0,0,COLS,LINES);
	wrefresh(renderWindow);
	printw("HAAA");
	wrefresh(renderWindow);

	// If no arguments, exit
	if (argc <= 0) 
	{
		return 0;	
	}

	//read file to memory
	char *file_buffer = readFile(argv[1]);
	int *max_characters_per_line = calculateBufferLines(file_buffer);
	int max_lines = (sizeof(*max_characters_per_line)/sizeof(int));

	
	//renderHelpWindow(helpWindow);
	renderFile(line, character, file_buffer, renderWindow);

	while(1)
	{
		//check inputs
		c = getch();
		switch(c)
		{	case KEY_UP:
				if(line != 0)
					--line;
				if(character >= max_characters_per_line[line])
					character = max_characters_per_line[line] - 1;
				break;
			case KEY_DOWN:
				if(line != max_lines - 1)
					++line;
				if(character >= max_characters_per_line[line])
					character = max_characters_per_line[line] - 1;
				break;
			case 3:
				eject = 1;
				break;
			case KEY_LEFT:
				if(character != 0)
					--character;
				break;
			case KEY_RIGHT:
				if(character != max_characters_per_line[line] - 1)
					++character;
				break;
			default:
				//mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", c, c);
				
				//reallocate memory
				position = lineCharToPos(line, character, file_buffer);
				insertCharacter(c, file_buffer, position);

		}

		
		if (eject) {
			break;
		}

		// Render helpwindow
		//renderHelpWindow(helpWindow);

		// Render with new info
		renderFile(line, character, file_buffer, renderWindow);

	}	

	endwin();			/* End curses mode		  */
	//close the supid file

	return 0;
} 
