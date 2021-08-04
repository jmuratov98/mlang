#include "mlang/token.h"

#include "mlang/containers/vector.h"

void test_tokenizer();
void test_vector();

int main(int argc, char **argv) {
    test_vector();
    test_tokenizer();
    return 0;
}

void test_tokenizer() {
    lexer_t lexer;

    const char *source_code = "int rand = a * (1234567890 + c); i += 2.0; j++";
    lexer_init(&lexer, source_code);
    lex(&lexer);
    
    for(int i = 0; i < vector_size(lexer.tokens); i++) {
        print_token(lexer.tokens[i]);
    }
    
    lexer_destroy(&lexer);
}

void test_vector() {
    printf("Testing Vector\n");
    int *array = vector_create(int);

    int i;

    vector_push_back(array, 10);
    vector_push_back(array, 8);
    vector_push_back(array, 4);
    vector_emplace(array, 3, 5);
    vector_push_back(array, 100);
    vector_push_back(array, 1);
    vector_pop_back(array, NULL);
    vector_remove(array, 0, &i);

    for(int i = 0; i < vector_size(array); i++) {
        printf("array[%d]: %d\n", i, array[i]);
    }

    vector_destroy(array);
}