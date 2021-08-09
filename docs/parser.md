# The parser #

## Definition ##
A parser is somet
hing that breaks speech (in this case code) with a specific set of grammar rules, into a form that can be used to understand the relationship between words or phrases.

## Grammar ##
<multiplicative-expression>
    : <multiplicative-expression> * expression
    : <multiplicative-expression> / expression
    : <multiplicative-expression> % expression

<additive-expression>
    : <multiplicative-expression>
    | <additive-expression> + <multiplicative-expression>
    | <additive-expression> - <multiplicative-expression>

<assignment-expression>
    : <additive-expression>
    | identifier = <assignment-expression>

<expression> 
    : <assignment-expression>
    | identifier
    | number
    ;

<expression-statement>
    : ;
    | <expression-statement> ;
    ;

<statement>
    : <expression-statement>
    ;