
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

#define WAIT_MS 250 	// default milliseconds b/w iterations
#define CON_W	51		// width used in console
#define MAX_LIM 999		// the maximum limit that the user can input
#define ROW_AMT 12		// numbers displayed per row
#define TITLE	"ANSIEVE" // title displayed at top of output

// Text attribute ANSI escape sequences
#define COL_BORDER	"\x1b[0;34m"	// border: blue
#define COL_BLDBRDR "\x1b[1;34m"	// bold border
#define COL_C_PRIME	"\x1b[1;37m"	// current prime: bold, white
#define COL_C_MULT	"\x1b[0;30;43m"	// current multiple: black, yellow highlight
#define COL_PRIME	"\x1b[0;32m"	// confirmed prime: green
#define COL_COMP	"\x1b[0;31m"	// confirmed composite: red
#define COL_UNKNOWN "\x1b[1;34m"	// unknown: bold, blue
#define TEXT_RESET  "\x1b[0m"		// reset all attributes

int waittime; // milliseconds b/w iterations, equals
			  // WAIT_MS unless command line arg is given

/* * * * * * * * * * * * * * * * * * * * * * * *
 * drawborder
 * * * * * * * * * * * * * * * * * * * * * * * *
 * Given the length of the list of numbers
 * (including zero), create a border around the
 * display area and clear out the interior.
 * Uses value ROW_AMT to calculate how many rows
 * are needed
 * * * * * * * * * * * * * * * * * * * * * * * */
void drawborder(int len) {
	// height of display area (12 numbers per row)
	int h = len / ROW_AMT;
	if (len % ROW_AMT != 0)
		++h;

	// width of display area
	// 3 digits per number, space separated
	int w = 4 * ROW_AMT + 1;
	
	// move cursor up by 1
	printf("\x1b[1A");
	// set text colour
	printf(COL_BORDER);

	// draw top of border
	printf(" \u2554");
	for (int i = 0; i < w; i++)
		printf("\u2550");
	printf("\u2557\n");

	// overlay a title at top of border
	char* title = TITLE;
	int titleLen = strlen(title);
	// move cursor up
	printf("\x1b[1A");
	// move cursor right
	printf("\x1b[%dC", (w - titleLen)/2);
	// print left border
	printf("\u2561");
	// print title in bold
	printf(COL_BLDBRDR);
	printf(" %s ", title);
	printf(COL_BORDER);
	// print right border
	printf("\u255e\n");

	// draw sides of border
	for (int y = 0; y < h; y++)
	{
		printf(" \u2551");
		printf("\x1b[%dC", w);
		printf("\u2551\n");
	}

	// draw bottom of border
	// left corner
	printf(" \u255a");
	// one horizontal piece
	printf("\u2550");
	// "Primes: " + room for 3 digits
	printf(COL_BLDBRDR"Primes: \x1b[3C"COL_BORDER);
	// rest of the horizontal space
	for (int i = 12; i < w; i++)
		printf("\u2550");
	// right corner
	printf("\u255d\n");

	// put cursor in correct area
	// move up by h+1
	printf("\x1b[%dA", h+1);
	// reset text
	printf(TEXT_RESET);
}
 
/* * * * * * * * * * * * * * * * * * * * * * * *
 * drawlist
 * * * * * * * * * * * * * * * * * * * * * * * *
 * Draws the list of primes within the border
 * area. Uses different ANSI text attributes for
 * different items such as primes or composites.
 * * * * * * * * * * * * * * * * * * * * * * * *
 * PARAMETERS:
 * * * * * * * * * * * * * * * * * * * * * * * *
 * bool* list : list[x] == true if x is prime
 * int   p    : index of current iteration
 * int   m    : index of current multiple of p
 * int   lim  : last index of list, NOT the size
 * * * * * * * * * * * * * * * * * * * * * * * *
 * Special Cases:
 * * * * * * * * * * * * * * * * * * * * * * * *
 * p == 0 : no number being tested,
 * 			list[x] => x is unknown
 * p <  0 : no number being tested,
 * 			list[x] => x is prime
 * m <= 0 : no multiple of p being marked
 * * * * * * * * * * * * * * * * * * * * * * * */
void drawlist(bool* list, int p, int m, int lim)
{
	int h = (lim + 1) / ROW_AMT;
	if ((lim + 1) % ROW_AMT != 0)
		++h;

	int count = 0;

	// for each row
	for (int y = 0; y < h; y++) {
		// move cursor right by 3
		printf("\x1b[3C");
		
		// for each number
		for (int x = 0; x < ROW_AMT; x++) {
			int index = y * ROW_AMT + x;
			
			// don't do too many numbers on last row
			if (index > lim)
				break;

			// leading spaces
			if (index < 10)
				printf("  ");
			else if (index < 100)
				printf(" ");

			// set colour and attributes for this number
			
			// current prime
			if (p > 0 && index == p) {
				// white, bold
				printf(COL_C_PRIME);
				if (list[p])
					++count;
			}

			// current multiple of prime
			else if (m > 0 && index == m) {
				// yellow, bold
				printf(COL_C_MULT);
			}
			
			// marked as prime
			else if (list[index]) {
				// confirmed prime
				if (p < 0 || index < p) {
					// green
					printf(COL_PRIME);
					++count;
				}
			
				// potential prime
				else {
					// blue
					printf(COL_UNKNOWN);
				}
			}
			
			// not prime
			else {
				// red
				printf(COL_COMP);
			}

			fflush(stdout);
			// print the number
			printf("%d", index);
			// print space, make sure it isn't highlighted
			printf(TEXT_RESET);
			printf(" ");
		
		} // end for x

		// end of row: start new line
		printf("\n");

	} // end for y

	// print the count of primes
	// move cursor right by 11 (after "Primes: ")
	printf("\x1b[11C");
	// right-justify
	if (count < 10)
		printf("  ");
	else if (count < 100)
		printf(" ");
	// print count
	printf(COL_BLDBRDR"%d\n", count);

	// reset text attributes
	printf(TEXT_RESET);
	// move cursor to the top
	printf("\x1b[%dA", h+1);

	// sleep for 1 frame
	usleep(1000 * waittime);
}

// run the sieve algorithm, draw the list at each iteration
void runsieve(int lim)
{
	bool* isprime = (bool*) malloc (sizeof(bool) * (lim+1));

	isprime[0] = false;
	isprime[1] = false;
	for (int i = 2; i <= lim; i++) {
		isprime[i] = true;
	}

	// clear screen, move cursor down 1 row
	printf("\x1b[2J\n");
	// draw border around display
	drawborder(lim + 1);
	// draw list in initial state
	drawlist(isprime, 0, -1, lim);

	// begin algorithm
	for (int prime = 2; prime <= lim; prime++) {
		drawborder(lim + 1);
		drawlist(isprime, prime, -1, lim);

		if (!isprime[prime])
			continue;

		for (int mult = prime*2; mult <= lim; mult += prime) {
			isprime[mult] = false;
			drawlist(isprime, prime, mult, lim);
		}
	}

	// draw list in final state
	drawlist(isprime, -1, -1, lim);
	// move cursor below list so output is not overwritten by terminal
	printf("\x1b[%dB\n", (lim)/ROW_AMT + 1);
}

// prompt user for limit, ensure 3-digit value is given,
// initiate sieve algorithm
int main(int argc, char* argv[])
{
	// global variable, milliseconds between each iteration of the algorithm
	waittime = WAIT_MS;
	int lim = 0; // highest value to check for primes (not the list size, the last element)

	// use command line arg for custom speed
	if (argc > 1) {
		long speed = strtol(argv[1], NULL, 0);

		if (speed > 0 && speed < INT_MAX) {
			waittime = (int)speed;
		}
		else {
			printf("Usage: ./%s [milliseconds between iterations [algorithm limit] ]\n", argv[0]);
		}
	}
	// use command line arg to input a limit from the command line
	if (argc > 2) {
		long llim = strtol(argv[2], NULL, 0);

		if (llim > 0 && llim < INT_MAX) {
			lim = (int)llim;
		}
		else {
			printf("Usage: ./%s [milliseconds between iterations [algorithm limit] ]\n", argv[0]);
		}
	}

	printf(COL_BLDBRDR);
	printf("\n\nANSIEVE.C - by Aidan Garvey\n"
			TEXT_RESET);
	if (lim <= 0) {
		printf("The algorithm will find primes up to a given number.\n"
				"Please note that entering in too large of a value will\n"
				"prevent the display from fitting on your screen.\n"
				"A good first number to try is 200.\n");

		printf(COL_PRIME);
		printf("\nFind all prime numbers up to...\n > "TEXT_RESET);
		scanf("%d", &lim);
	}

	if (lim > MAX_LIM) {
		lim = MAX_LIM;
		printf("That number is too high. Using %d\n", lim);
		// give user a second to read above output
		sleep(1);
	}

	// hide cursor while running (if supported, makes appearance a little nicer)
	printf("\x1b[?25l");

	// execute main loop
	runsieve(lim);

	// print exit message
	printf("\x1b[1;36m"
			"Thanks for checking out ANSIEVE!\n"
			"\x1b[1;31m"
			"Programmed by Aidan Garvey, 2022-03-20\n"
			"\x1b[1;34m"
			"github.com/aidan-garvey/ansieve\n"
			TEXT_RESET
			"\x1b[?25h"); // disable hidden cursor

	return 0;
}

