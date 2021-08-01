#include "mlang/token.h"

int main(int argc, char **argv) {
    token_t tokens[] = { 
        { .type=IF, .value="if" },
        { .type=LEFT_PAREN, .value="(" },
        { .type=RIGHT_PAREN, .value=")" },
        { .type=MULT_ASSIGN, .value="*=" },
        { .type=INT, .value="int" }
    };
    int length = sizeof(tokens) / sizeof(tokens[0]);

    for(int i = 0; i < length; i++) {
        print_token(tokens + i); // Same thing as saying &tokens[i]
    }

    return 0;
}