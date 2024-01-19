/*!
    \file "utils.hpp"

    Author: Matt Ervin <matt@impsoftware.org>
    Formatting: 4 spaces/tab (spaces only; no tabs), 120 columns.
    Doc-tool: Doxygen (http://www.doxygen.com/)

    Utilities for leetcode problems.
*/

#ifndef UTILS_HPP_4F58C358_FF95_42DE_A545_7DEA5504DE2C
#define UTILS_HPP_4F58C358_FF95_42DE_A545_7DEA5504DE2C
#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <deque>
#include <doctest/doctest.h> //!\sa https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#include <functional>
#include <iterator>
#include <list>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <set>
#include <span>
#include <stack>

using namespace std;

// [----------------(120 columns)---------------> Module Code Delimiter <---------------(120 columns)----------------]

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
    virtual ~TreeNode() {
        if (left) {
            delete left;
        }
        if (right) {
            delete right;
        }
    }
};

// [----------------(120 columns)---------------> Module Code Delimiter <---------------(120 columns)----------------]

constexpr auto const null = (std::numeric_limits<int>::min)();

struct elapsed_time_t
{
    std::chrono::steady_clock::time_point start{};
    std::chrono::steady_clock::time_point end{};
    
    elapsed_time_t(
        std::chrono::steady_clock::time_point start
        , std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now()
    ) : start{std::move(start)}, end{std::move(end)} {}
};

std::ostream&
operator<<(std::ostream& os, elapsed_time_t const& et);

TreeNode*
createBst(std::vector<int> values);

std::vector<int>
inorder(TreeNode const* root);

std::vector<int>
preorder(TreeNode const* root);

std::unique_ptr<TreeNode>
createLevelOrderBT(std::vector<int> const& levelOrderNodes);

std::vector<int>
btToLevelOrder(TreeNode* root);

#endif // #ifndef UTILS_HPP_4F58C358_FF95_42DE_A545_7DEA5504DE2C

/*
    End of "utils.hpp"
*/
