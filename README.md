# ANSIEVE
[Sieve of Eratosthenes](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes) visualized using ANSI escape sequences in the terminal.

## Usage
```./ansieve [custom speed [limit shortcut]]```
- The custom speed is the amount of time between iterations, in milliseconds. Must be greater than 0. Default value is used if not specified.
- The limit shortcut allows the user to enter the limit which the algorithm will iterate up to from the terminal. If not given, the program will prompt the user for a value.

### Choosing a Limit
You'll be asked to provide a limit, and then the sieve will be run on a list up to that number (0, 1, 2, ..., limit). You can do this from the command line, or once the program opens. For the output to be displayed properly, you should choose an amount that fits in the terminal. Keep in mind that there are always 12 numbers shown per row. On a fullscreen terminal, the most I was able to fit was 767 (64 rows). I'd recommend starting with something like 200 to see how much space it takes.

## Compiling
A Makefile is included which uses GCC. Use ```make ansieve``` to compile.

## Planned Features
If I decide to continue developing this project, I would like to implement other prime number sieves such as the [Sieve of Sundaram.](https://en.wikipedia.org/wiki/Sieve_of_Sundaram) It seems like the display code would require little or no adjustments, and only the code for the sieve algorithm would need to be added.
