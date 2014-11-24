#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <vector>
#include "./ukkonen.h"
// несколько handmade тестов и стресс тестирование
TEST(find_all_occurences, hand_made_1) {
    int res[3] = {0, 1, 2};
    std::string s = "aaa", pat = "a";
    std::vector<int> vr = findAlloccurences(SuffixTree(s), pat);
    std::sort(vr.begin(), vr.end());
    EXPECT_EQ(vr, std::vector<int>(res, res + 3));
}

TEST(find_all_occurences, hand_made_2) {
    std::string s = "abbac", pat = "d";
    std::vector<int> vr = findAlloccurences(SuffixTree(s), pat);
    EXPECT_TRUE(vr.empty());
}

TEST(find_all_occurences, hand_made_3) {
    std::string s = "aacab", pat = "a";
    int res[3] = {0, 1, 3};
    std::vector<int> vr = findAlloccurences(SuffixTree(s), pat);
    std::sort(vr.begin(), vr.end());
    EXPECT_EQ(vr, std::vector<int>(res, res + 3));
}

TEST(find_all_occurences, hand_made_4) {
    std::string s = "abacabadabacaba", pat = "aba";
    int res[4] = {0, 4, 8, 12};
    std::vector<int> vr = findAlloccurences(SuffixTree(s), pat);
    std::sort(vr.begin(), vr.end());
    EXPECT_EQ(vr, std::vector<int>(res, res + 4));
}

TEST(find_all_occurences, hand_made_5) {
    int res[4] = {0, 1, 2, 3};
    std::string s = "aaa", pat = "";
    std::vector<int> vr = findAlloccurences(SuffixTree(s), pat);
    std::sort(vr.begin(), vr.end());
    EXPECT_EQ(vr, std::vector<int>(res, res + 4));
}

TEST(find_all_occurences, stress) {
    srand(123511);
    int TEST_SIZE = 1000;
    int PATTERN_SIZE = 6;
    std::string s;
    for (int i = 0; i < TEST_SIZE; i++) {
        s += 'a' + rand() % 3;
    }
    std::string p;
    for (int i = 0; i < PATTERN_SIZE; i++) {
        p += 'a' + rand() % 3;
    }
    std::vector<int> vr = findAlloccurences(SuffixTree(s), p);
    std::sort(vr.begin(), vr.end());
    std::vector<int> pp;
    for (int i = 0; i < TEST_SIZE - PATTERN_SIZE; i++) {
        bool found = true;
        for (int j = 0; j < PATTERN_SIZE; j++) {
            if (s[i + j] != p[j]) {
                found = false;
                break;
            }
        }
        if (found)
            pp.push_back(i);
    }
    EXPECT_EQ(vr, pp);
}

int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
