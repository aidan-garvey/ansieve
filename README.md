# ANSIEVE
[Sieve of Eratosthenes](https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes) visualized using ANSI escape sequences in the terminal.

### Usage
```./ansieve [custom speed [limit shortcut]]```
- The custom speed is the amount of time between iterations, in milliseconds. Must be greater than 0. Default value is used if not specified.
- The limit shortcut allows the user to enter the limit which the algorithm will iterate up to from the terminal. If not given, the program will prompt the user for a value.

### Compiling
A Makefile is included which uses GCC. Use ```make ansieve``` to compile.

### Planned Features
If I decide to continue developing this project, I would like to implement other prime number sieves such as the [Sieve of Sundaram.](https://en.wikipedia.org/wiki/Sieve_of_Sundaram) It seems like the display code would require little or no adjustments, and only the code for the sieve algorithm would need to be added.
