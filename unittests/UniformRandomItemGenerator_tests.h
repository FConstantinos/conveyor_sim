//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include <gtest/gtest.h>
#include "UniformRandomItemGenerator.h"

using namespace std;
using namespace conveyorsim;

struct UniformRandomItemGeneratorTestCase {
    const unordered_set<ItemPN> PNSet;
    const bool emptyPossible;
};

class UniformRandomItemGeneratorTestFixture : public ::testing::TestWithParam<UniformRandomItemGeneratorTestCase> {
protected:
    const size_t numTrials = 100000;
    const double abs_error = 0.01;
};

// TODO: Write a test for get_next_items(numTrials) as well
TEST_P(UniformRandomItemGeneratorTestFixture, UniformRandomItemGeneratorTest) {
    const auto testCase = GetParam();
    const auto &PNSet = testCase.PNSet;
    const auto &emptyPossible = testCase.emptyPossible;

    if (PNSet.size() == 0 && !emptyPossible) {
        ASSERT_THROW(UniformRandomItemGenerator gen(PNSet, emptyPossible), invalid_argument);
        return;
    }
    const UniformRandomItemGenerator gen(PNSet, emptyPossible);

    // Test that the sample mean is as expected
    const size_t N = PNSet.size() + (emptyPossible ? 1 : 0);
    unordered_map<optional<Item>, size_t> samples;
    for(size_t trial = 0; trial < numTrials; trial++) {
        samples[gen.get_next_item()]++;
    }
    ASSERT_EQ(samples.size(), N);
    size_t sumCounts = 0;
    for (const auto &[pn, count]: samples) {
        sumCounts += count;
    }
    for (const auto &[pn, count]: samples) {
        ASSERT_NEAR((double)count/sumCounts, (double) 1/N, abs_error);
    }
}

vector<UniformRandomItemGeneratorTestCase> urigtc = {
        {{}, false},
        {{}, true},
        {{ItemPN(0)}, false},
        {{ItemPN(0)}, true},
        {{ItemPN(0), ItemPN(1)}, false},
        {{ItemPN(0), ItemPN(1)}, true},
};

INSTANTIATE_TEST_CASE_P(
        UniformRandomItemGeneratorTest,
        UniformRandomItemGeneratorTestFixture,
        ::testing::ValuesIn(urigtc)
);
