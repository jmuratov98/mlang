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
    printf("Testing Tokenizer\n");
    token_t tokens[] = {
        {.type = IF, .value = "if"},
        {.type = LEFT_PAREN, .value = "("},
        {.type = RIGHT_PAREN, .value = ")"},
        {.type = MULT_ASSIGN, .value = "*="},
        {.type = INT, .value = "int"}};
    int length = sizeof(tokens) / sizeof(tokens[0]);

    for (int i = 0; i < length; i++)
    {
        print_token(tokens + i); // Same thing as saying &tokens[i]
    }
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