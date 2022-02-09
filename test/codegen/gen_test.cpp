//
// Created by Alexandra Dima on 11/15/21.
#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>
#include <gtest/gtest.h>
#include "einsum_taco/ir/ir.h"
#include "einsum_taco/ir/cleanup.h"
#include "einsum_taco/codegen/codegen_visitor.h"
#include "einsum_taco/ir/dump_ast.h"
#include "einsum_taco/gstrt/tensor.h"
#include <einsum_taco/parser/heading.h>
#include "../utils.h"
#include <filesystem>

using namespace std;

namespace fs = std::filesystem;

class GenTest : public testing::Test {
public:
    std::stringstream oss;
    CodeGenVisitor generator;
    DumpAstVisitor printer;


    GenTest() : generator(oss, "test") {}

    static std::string readFileIntoString(const std::string& path) {
        FILE *fp = fopen(path.c_str(), "r");
        if (fp == nullptr) {
            std::cout << "Failed to open file for reading " << path << std::endl;
            std::abort();
        }
        auto size = fs::file_size(path);
        std::string contents = std::string(size, 0);
        fread(contents.data(), 1, size, fp);
        fclose(fp);
        return contents;
    }

    static std::string get_runtime_dir() {
        return {GSTACO_RUNTIME};
    }

    static std::string readDataIntoString(const std::string& path) {
        return readFileIntoString(get_test_data_dir() + path);
    }

    static std::string get_tensor_template() {
        return readFileIntoString(get_runtime_dir() + "tensor.h");
    }

    std::string tensor_template = get_tensor_template();

    void assert_generated(const std::string& input_filename, const std::string& expected_filename) {
        auto input = readDataIntoString(input_filename);
        auto expected = tensor_template + readDataIntoString(expected_filename);

        // parse
        auto mod = std::make_shared<Module>(parse(input));

        // cleanup
        auto new_module = apply_default_rewriters(mod);

        // code generation
        new_module->accept(&generator);
        auto output = oss.str();

        // check output code
        EXPECT_EQ(output, expected);
    }

    void assert_generated_defintion(const std::string& input_filename, const std::string& expected_filename, int d = 0) {
        // parse
        auto input = readDataIntoString(input_filename);
        auto expected = readDataIntoString(expected_filename);
        auto mod = std::make_shared<Module>(parse(input));

        // cleanup
        auto new_module = apply_default_rewriters(mod);

        // print IR
        new_module->accept(&printer);
        // cerr << printer.ast;

        // selective code generation
        auto def = new_module->decls[d];
        if (def->is_def()) {
            def->as_def()->accept(&generator);
        } else if (def->is_decl()) {
            auto defs = def->as_decl()->body;
            for (auto& stmt: defs) {
                stmt->accept(&generator);
            }
        }
        auto output = oss.str();

        // check output code
        EXPECT_EQ(output, expected);
    }

};

TEST_F(GenTest, Definition1) {
    assert_generated("codegen/inputs/definition1.txt", "codegen/outputs/definition1.cpp");
}

TEST_F(GenTest, Definition2) {
    assert_generated("codegen/inputs/definition2.txt", "codegen/outputs/definition2.cpp");
}

TEST_F(GenTest, Definition3) {
    assert_generated("codegen/inputs/definition3.txt", "codegen/outputs/definition3.cpp");
}

TEST_F(GenTest, Definition4) {
    assert_generated("codegen/inputs/definition4.txt", "codegen/outputs/definition4.cpp");
}

TEST_F(GenTest, DefinitionCall) {
    //assert_generated_defintion("codegen/inputs/call.txt", "codegen/outputs/call.cpp", 1);
    assert_generated("codegen/inputs/call.txt", "codegen/outputs/call.cpp");
}

TEST_F(GenTest, DefinitionCallCondition1) {
    assert_generated("codegen/inputs/call_condition1.txt", "codegen/outputs/call_condition1.cpp");
}

//TODO: remove lambda, later
TEST_F(GenTest, DefinitionCallRepeat1) {
    assert_generated("codegen/inputs/call_repeat1.txt", "codegen/outputs/call_repeat1.cpp");
}

TEST_F(GenTest, DefinitionCallRepeat2) {
    assert_generated("codegen/inputs/call_repeat2.txt", "codegen/outputs/call_repeat2.cpp");
}

TEST_F(GenTest, DefinitionCallRepeat3) {
    assert_generated("codegen/inputs/call_repeat3.txt", "codegen/outputs/call_repeat3.cpp");
}

TEST_F(GenTest, DefinitionCallRepeat4) {
    assert_generated("codegen/inputs/call_repeat4.txt", "codegen/outputs/call_repeat4.cpp");
}

TEST_F(GenTest, DefinitionCallRepeat5) {
    assert_generated("codegen/inputs/call_repeat5.txt", "codegen/outputs/call_repeat5.cpp");
}

TEST_F(GenTest, DefinitionCallCondition2) {
    assert_generated("codegen/inputs/call_condition2.txt", "codegen/outputs/call_condition2.cpp");
}

TEST_F(GenTest, BFS_Step) {
    assert_generated("codegen/inputs/bfs_step.txt", "codegen/outputs/bfs_step.cpp");
}
