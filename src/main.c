#include "mlang/stream.h"
#include "mlang/token.h"
#include "mlang/parser.h"

#include "mlang/containers/vector.h"
#include "mlang/containers/ast.h"

void test_tokenizer();
void test_vector();
void test_stream_and_tokenizer(const char *input);
void test_ast();
void test_parser(const char *input);


int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Not enough arguments\n");
        return 1;
    }

    // test_vector();
    // test_tokenizer();
    // test_stream_and_tokenizer(argv[1]);
    // test_ast();
    test_parser(argv[1]);
    return 0;
}

void test_parser(const char *input) {
    stream_t stream;
    lexer_t lexer;
    parser_t parser;

    stream_init(&stream, input);
    char *source_code = read_file(&stream);

    lexer_init(&lexer, source_code);
    lex(&lexer);

    parser_init(&parser);
    parse(&parser, lexer.tokens);

    parser_destroy(&parser);
    lexer_destroy(&lexer);
}

void test_ast() {
    node_t *root = create_node("root");
    insert_node(&root, "hello");
    insert_node(&root, "goodbye");

    destroy_node(&root);
}

void test_stream_and_tokenizer(const char *input) {
    lexer_t lexer;
    stream_t stream;

    stream_init(&stream, input);
    char *source_code = read_file(&stream);
    printf("%s\n", source_code);

    lexer_init(&lexer, source_code);

    lex(&lexer);
    for(int i = 0; i < vector_size(lexer.tokens); i++) {
        print_token(lexer.tokens[i]);
    }

    lexer_destroy(&lexer);
}

void test_tokenizer() {
    lexer_t lexer;

    char *source_code = "int rand = a * (1234567890 + c); i += 2.0; j++";
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