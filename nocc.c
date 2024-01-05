#include "./nocc.h"

#include <string.h>
#include <stdio.h>

const char* BINARY_PATH = "./bin"; 
const char* OBJECT_PATH = "./bin-obj";

typedef struct {
    bool build_;
    struct {
        bool help;
        char* config;
    } build;
    bool run_;
    struct {
        bool help;
    } run;
    bool help;
    bool version;
} nocc_ap_parse_result;

bool build_mlang(nocc_ap_parse_result* result);
bool run_mlang(nocc_ap_parse_result* result);

int main(int argc, char** argv) {
    nocc_ap_parse_result result = {};

    nocc_argparse_opt switch_args[] = {
        nocc_ap_opt_boolean('d', "debug", "Builds the program as a debug build", NULL, NULL),
        nocc_ap_opt_boolean('r', "release", "Builds the program as a release build", NULL, NULL),
        nocc_ap_opt_boolean('D', "dist", "Builds the program as a dist build", NULL, NULL)
    };

    nocc_argparse_opt build_options[] = {
        nocc_ap_opt_switch(switch_args, "debug", &(result.build.config)),
        nocc_ap_opt_boolean('h', "help", "Prints this message", NULL, &(result.build.help))
    };

    nocc_argparse_opt run_options[] = {
        nocc_ap_opt_boolean('h', "help", "Prints this message", NULL, &(result.run.help))
    };

    nocc_argparse_opt program_options[] = {
        nocc_ap_opt_boolean('h', "help", "Prints this message", NULL, &(result.help)),
        nocc_ap_opt_boolean('v', "version", "Prints the software version", NULL, &(result.version))
    };

    nocc_argparse_opt subcommands[] = { 
        nocc_ap_cmd("build", "Builds the project", build_options, NULL, NULL, &(result.build_)),
        nocc_ap_cmd("run", "runs the project", run_options, NULL, NULL, &(result.run_))
    };

    nocc_argparse_opt program = nocc_ap_cmd("nocc", "Building, linking, and running all your favorite code", program_options, NULL, subcommands, NULL);


    nocc_ap_parse(&program, argc, argv);

    int status = 0;

    if(result.build_) {
        if(result.build.help) {
            nocc_ap_usage(&program.commands[0]);
            goto failure;
        }

        if(!nocc_mkdir_if_not_exists(BINARY_PATH)) {
            nocc_error("Failed to create dir %s", BINARY_PATH);
            return 1;
        };

        if(!nocc_mkdir_if_not_exists(OBJECT_PATH)) {
            nocc_error("Failed to create dir %s", OBJECT_PATH);
            return 1;
        };

        if(!build_mlang(&result)) {
            nocc_error("unable to build helloworld");
            status = 1;
            goto failure;
        }

    }

    else if (result.run_) {
        if(result.run.help) {
            nocc_ap_usage(&program.commands[1]);
            goto failure;
        }

        if(!run_mlang(&result)) {
            nocc_error("unable to run helloworld");
            status = 1;
            goto failure;
        }
    }

    else if(result.help) {
        nocc_ap_usage(&program);
        goto failure;
    }

    else if(result.version) {
        printf("%s", NOCC_VERSION);
        goto failure;
    }

    else {
        nocc_ap_usage(&program);
        status = 1;
        goto failure;
    }

failure:
    return status;
}

bool build_mlang(nocc_ap_parse_result* result) {
    bool status = true;
    
    const char* TARGET_PATH = "./bin/mlang.exe";
    
    nocc_darray(const char*) array_of_source_files = nocc_da_create(const char*);
    if(!nocc_read_dir("./src", "c", &array_of_source_files)) {
        status = false;
        goto failure;
    }

    nocc_darray(nocc_string) array_of_object_files;
    nocc_generate_object_files(array_of_object_files, array_of_source_files, "%s/%n.o", OBJECT_PATH);

    for(size_t i = 0; i < nocc_da_size(array_of_source_files); i++) {
        if(!nocc_should_recompile(&array_of_source_files[i], 1, array_of_object_files[i]))
            continue;

        nocc_trace("Building %s", array_of_source_files[i]);
        
        nocc_darray(const char*) cmd = nocc_da_create(const char*);
        nocc_cmd_add(cmd, "clang");
        if(strcmp(result->build.config, "debug") == 0) {
            nocc_cmd_add(cmd, "-DMLANG_COFIG_DEBUG");
        } else if(strcmp(result->build.config, "release") == 0) {
            nocc_cmd_add(cmd, "-DMLANG_COFIG_RELEASE");
        } else if(strcmp(result->build.config, "dist") == 0) {
            nocc_cmd_add(cmd, "-DMLANG_COFIG_DIST");
        }

        nocc_cmd_add(cmd,"-I./src");
        nocc_cmd_add(cmd, "-Wall", "-Wformat=2", "-march=native");
        if(strcmp(result->build.config, "debug") == 0) {
            nocc_cmd_add(cmd, "-O0", "-g");
        } else if(strcmp(result->build.config, "release") == 0) {
            nocc_cmd_add(cmd, "-O2", "-g");
        } else if(strcmp(result->build.config, "dist") == 0) {
            nocc_cmd_add(cmd, "-O3");
        }
        nocc_cmd_add(cmd, "-c", array_of_source_files[i]);
        nocc_cmd_add(cmd, "-o", array_of_object_files[i]);

        nocc_cmd_execute(cmd);

        nocc_da_free(cmd);
    }

    if(nocc_should_recompile(array_of_object_files, nocc_da_size(array_of_object_files), TARGET_PATH)) {
        nocc_info("Linking mlang");

        nocc_darray(const char*) cmd = nocc_da_create(const char*);
        nocc_cmd_add(cmd, "clang");
        if(strcmp(result->build.config, "debug") == 0) {
            nocc_cmd_add(cmd, "-g");
        } else if(strcmp(result->build.config, "release") == 0) {
            nocc_cmd_add(cmd, "-g");
        }

        nocc_cmd_add(cmd, "-o", TARGET_PATH);
        nocc_cmd_addn(cmd, nocc_da_size(array_of_object_files), array_of_object_files);
 
        nocc_cmd_execute(cmd);
        nocc_da_free(cmd);
    }

failure:
    for(size_t i = 0; i < nocc_da_size(array_of_object_files); i++) {
        nocc_str_free(array_of_object_files[i]);
    }
    nocc_da_free(array_of_object_files);
    for(size_t i = 0; i < nocc_da_size(array_of_source_files); i++) {
        free(array_of_source_files[i]);
    }
    nocc_da_free(array_of_source_files);
    return true;
}

bool run_mlang(nocc_ap_parse_result* result) {
    printf("Running mlang.exe\n");
    nocc_darray(const char*) cmd = nocc_da_create(const char*);
    nocc_cmd_add(cmd, "./bin/mlang.exe");
    
    nocc_cmd_execute(cmd);

    nocc_da_free(cmd);

    return true;
}