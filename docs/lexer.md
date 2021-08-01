# The lexer #

## Definition ##
The lexer is a black box that takes a filepath as an input and outputs a table of tokens. You may have heard it called a tokenizer or a lexical syntax

### Example ###
- Example 1:  
`The man opened the door`  
In order to lex the statement, we have to break it up into various words, such as `the`, `man`, `opened`, `door`. We can then clasify each of the words, like so: `the (adjective)`, `man (subject)`, `opened (verb)`, and `door (noun)`.

- Example 2: 
```c
printf("Hello, world\n");
```
Like we did in the example above, you can do something similar for programming languages. One we tokenize the above statement we get: `printf`, `(`, `"`, `Hello, world\n`, `)`, and finally `;`. Each of which mean something different. `printf` is the only identifier, `Hello, world\n` is a string literal, and the rest are used for the language grammar such as the `(`, `)`, and `;`.