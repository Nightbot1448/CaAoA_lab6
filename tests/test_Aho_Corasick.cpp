#include <gtest/gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::GTEST_FLAG(filter) = "Aho_Corasick_T.*";
    return RUN_ALL_TESTS();
}