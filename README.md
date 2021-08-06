# MLANG: A simple compiler for a c-like lang #

A programming language written in C, mainly a way for me to learn how a compiler actually works.

If anyone wants to contribute, in the code, or the grammar, feel free to contribute.

## Getting Started ##

For this project, I am using:
- Makefile (so make sure your either on linux, macos, or mingw allows you to install make command for windows)
- I use git bash for my terminal so the following commands will not work on windows command line.

```bash
git clone https://github.com/jmuratov98/mlang.git
cd mlang
make init # Will create the ./bin/ and ./bin-obj/ directories where compiled files go. Otherwise the build will fail.
make      # .o files will go to the ./bin-obj/ and the executable will be in ./bin/ directory.
```

## Features ##
- [x] Lexer
    - [x] literal, reserved and operator lexing
- [ ] Parser
- [ ] Assembler

### Don't know if I need
- [ ] symbol tables
- [ ] string tables

## Programming language features ##
Nothing so far

## Usage ##
Add later

## What I want the language to look like ##
```c
// main.l
import fib from './fib';

int main(int argc, char **argv) {
    fib(5);
    return 0;
}

// fib.l
import printf from 'stdio';

int fib_internal(int n) {
    if (n == 1) return 1;
    if (n == 2) return 1;
    return fib_internal(n - 1) + fib_internal(n - 2);
}

export int fib(int n) {
    return fib_internal(5);
}
```

## FAQ ##
### Why C? ###
The short answer, I honestly have no idea.

The long answer is, while everything that you get in C++ is very nice, I want to get into C more, so I thought the best way to do that is to make a project with it.

### What does mlang mean? ###
- M for my, so my language
- M for my last name 'Muratov' so muratov language

Yes that was why :|