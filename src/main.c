#include "lexer.h"
#include "parser.h"
#include "containers/ast.h"

#include <stdio.h>

static const char *filecontents =
"import { printf } from \"stdio\";\n"
"\n"
"void print() {\n"
"   printf(\"Hello, world\");\n"
"}\n"
"\n"
"int main(int argc, string* argv) {\n"
"   print();\n"
"   return 0;\n"
"}";

int main(int argc, char** argv) {
    lexer_t lexer;
    lexer_init(&lexer, filecontents);
    tokenize(&lexer);

    // array(token_t) tokens = lexer_get_tokens();
    // for(size_t i = 0; i < array_size(tokens); i++) {
    //     token_print(&tokens[i]);
    // }

    ast_node_t* translation_unit = parser_parse();
    ast_print(translation_unit);
}
