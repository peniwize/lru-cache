/*!
    \file "main.cpp"

    Author: Matt Ervin <matt@impsoftware.org>
    Formatting: 4 spaces/tab (spaces only; no tabs), 120 columns.
    Doc-tool: Doxygen (http://www.doxygen.com/)

    https://leetcode.com/problems/lru-cache/
*/

//!\sa https://github.com/doctest/doctest/blob/master/doc/markdown/main.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "utils.hpp"

/*
    Design a data structure that follows the constraints
    of a Least Recently Used (LRU) cache.

    Implement the LRUCache class:

        * LRUCache(int capacity) Initialize the LRU cache 
          with positive size capacity.
        
        * int get(int key)
          Return the value of the key if the key exists, otherwise return -1.

        * void put(int key, int value)
          Update the value of the key if the key exists.
          Otherwise, add the key-value pair to the cache.
          If the number of keys exceeds the capacity from this operation, 
          evict the least recently used key.
    
    The functions get and put must each run in O(1) average time complexity.

    Constraints:

        * 1 <= capacity <= 3000

        * 0 <= key <= 104

        * 0 <= value <= 105

        * At most 2 * 105 calls will be made to get and put.

    Example 1:

        Input
        ["LRUCache", "put", "put", "get", "put", "get", "put", "get", "get", "get"]
        [[2], [1, 1], [2, 2], [1], [3, 3], [2], [4, 4], [1], [3], [4]]
        Output
        [null, null, null, 1, null, -1, null, -1, 3, 4]

        Explanation
        LRUCache lRUCache = new LRUCache(2);
        lRUCache.put(1, 1); // cache is {1=1}
        lRUCache.put(2, 2); // cache is {1=1, 2=2}
        lRUCache.get(1);    // return 1
        lRUCache.put(3, 3); // LRU key was 2, evicts key 2, cache is {1=1, 3=3}
        lRUCache.get(2);    // returns -1 (not found)
        lRUCache.put(4, 4); // LRU key was 1, evicts key 1, cache is {4=4, 3=3}
        lRUCache.get(1);    // return -1 (not found)
        lRUCache.get(3);    // return 3
        lRUCache.get(4);    // return 4

    Your LRUCache object will be instantiated and called as such:
        LRUCache* obj = new LRUCache(capacity);
        int param_1 = obj->get(key);
        obj->put(key,value);
*/

/*
    Solution #1 (INVALID):
    
    NOTE: Testing proved this is NOT what the problem wants.

    Neither the problem description nor the example make it clear 
    whether or not the LRU must maintain keys in priority ORDER
    or only track the oldest key.  This solution is the latter
    of the two, which I believe may have better time complexity.
    
    Use a hash table (unordered map) to store key:value pairs,
    which provides O(1) time.  Use a vector to track all keys with 
    the least recently used key always residing at the front of the
    vector.  The vector is partitioned into two areas: the LRU area,
    composed on only one key, and a non-LRU area, composed of all 
    remaining keys.  The LRU key will always be at the back of the
    vector: index [0].  The non-LRU keys will always occupy the other 
    side of the vector: indexes [1, n].  When a new key is added, 
    swap the LRU key with any RANDOMLY selected key in the non-LRU 
    partition.  When a key is touched (retrieved) and it is the LRU
    key, swap it with any RANDOMLY selected key in the non-LRU 
    partition.  This is a less efficient solution that using a
    hash table + some linked data structure like a tree or list, but
    it's more space efficient.
*/
class LRUCache_Solution1 {
private:
    struct value_t {
        int value{};
        size_t idxInLru{};
        value_t() = default;
        value_t(int value, size_t idxInLru) : value{std::move(value)}, idxInLru{std::move(idxInLru)} {}
        bool operator==(const value_t& rhs) const noexcept { return value == rhs.value; }
        bool operator<(const value_t& rhs) const noexcept { return value < rhs.value; }
    };

public:
    LRUCache_Solution1(int capacity) : capacity_{std::move(capacity)} {}
    
    int get(int key) {
        auto const iter = table_.find(key);
        auto const found = table_.end() != iter;
        if (found) {
            auto const isOldest = lru_[0] == key;
            if (isOldest) {
                // Item is no longer oldest since it has been accessed.
                // Move oldest (least recently used item) into any
                // random position among non-oldest items, in LRU index.
                auto randIdx = 1 >= lru_.size() ? 0 : 1 + rand() % (lru_.size() - 1);
                std::swap(table_[key].idxInLru, table_[lru_[randIdx]].idxInLru);
                std::swap(lru_[0], lru_[randIdx]);
            }
            return iter->second.value;
        }
        
        return -1;
    }
    
    void put(int key, int value) {
        auto const findIter = table_.find(key);
        auto const alreadyCached = table_.end() != findIter;
        if (alreadyCached) {
            auto const isOldest = lru_.front() == key;
            auto& tableEntry = table_[key];
            if (isOldest) {
                if (1 < lru_.size()) {
                    // Move oldest (least recently used item) into any
                    // random position among non-oldest items, in LRU index.
                    auto randIdx = 1 >= lru_.size() ? 0 : 1 + rand() % (lru_.size() - 1);
                    std::swap(tableEntry.idxInLru, table_[lru_[randIdx]].idxInLru);
                    std::swap(lru_[0], lru_[randIdx]);
                }
            } else {
                // auto const cachedValue = findIter->second;
                // auto const keyIdxInLru = cachedValue.idxInLru;
                // Do nothing (key is already among most recently used items).
            }
            tableEntry.value = std::move(value);
        } else {
            if (table_.size() < capacity_) {
                // Add new item.
                table_.insert(std::make_pair(key, value_t{std::move(value), lru_.size()}));
                lru_.push_back(key);
            } else {
                // Move any randomly selected item, among most recently used items and
                // that is not key, into oldest (least recently used) item position.
                auto randIdx = 1 >= lru_.size() ? 0 : 1 + rand() % (lru_.size() - 1);
                auto const newOldestKey = lru_[randIdx];
                table_[newOldestKey].idxInLru = 0;
                table_.erase(lru_[0]);
                table_.insert(std::make_pair(key, value_t{value, randIdx}));
                lru_[0] = newOldestKey;
                lru_[randIdx] = key;
            }
        }
    }

private:
    std::unordered_map<int, value_t> table_{};
    std::vector<int> lru_{};
    int capacity_{};
};

/*
    Solution #2 (VALID):

    This solution uses a hash table to index into a doubly linked list 
    where each list node contains the value.  A list is used because it 
    maintains the order of the values and provides O(1) insertion and 
    removal.  Searching in the list would take O(1) time if it were done,
    however it is not done, rather the hash table is used to find list 
    items on O(1) time.  A hash table also has O(1) insertion and 
    removal so the cumulative effect of the two data structures is 
    overall O(1) time complexity.  Using a doubly linked list and a
    hash table is a heavy weight solution for caching a small number of 
    small objects, but the problem does state that up to 3000 objects
    may be stored in the cache.  An optimal solution would be to determine
    the threshold at which this solution out performs a simpler trivial 
    implementation with just a vector and linear search and then choose 
    which to use during construction (based on the cache capacity).

    Time: O(1)  [for all operations, except destruction == O(n)]
    Space: O(n)  [n = cache capacity]
*/
class LRUCache_Solution2 {
private:
    struct CacheNode;
    
    // Boilerplate: list node prev, next pointers.
    // This exists to prevent dummy nodes from containing anything else.
    class CacheNodeBase {
        union NodePtr { CacheNode *node; CacheNodeBase* base; };
        NodePtr prev_;
        NodePtr next_;
    public:
        CacheNodeBase() {
            this->prev_.node = nullptr;
            this->next_.node = nullptr;
        }
        CacheNodeBase(CacheNodeBase* prev, CacheNodeBase* next) {
            this->prev_.base = std::move(prev);
            this->next_.base = std::move(next);
        }
        CacheNode*& prev() noexcept { return prev_.node; }
        CacheNodeBase*& prevBase() noexcept { return prev_.base; }
        CacheNode*& next() noexcept { return next_.node; }
        CacheNodeBase*& nextBase() noexcept { return next_.base; }
    };
    
    struct CacheNode : public CacheNodeBase {
        int key{};
        int value{};
        CacheNode() = default;
        CacheNode(int key, int value, CacheNodeBase* prev, CacheNodeBase* next)
            : key{std::move(key)}, value{std::move(value)}
            , CacheNodeBase{std::move(prev), std::move(next)} {}
    };

public:
    LRUCache_Solution2(int capacity)
    : capacity_{std::move(capacity)} {
        // Preallocate memory.
        nodes_.reserve(capacity); // Defer node construction until node is added.
        index_.reserve(capacity);

        // Initialize doubly linked node list.
        lruRef_.nextBase() = &mruRef_;
        mruRef_.prevBase() = &lruRef_;
    }
    
    int get(int key) {
        auto const iter = index_.find(key);
        auto const found = index_.end() != iter;
        return found ? (setMru(iter->second), iter->second->value) : -1;
    }
    
    void put(int key, int value) {
        auto const iter = index_.find(key);
        auto const found = index_.end() != iter;
        if (found) {
            // Update node value and make node MRU.
            iter->second->value = std::move(value);
            setMru(iter->second);
        } else {
            CacheNode* node{};

            auto const cacheIsFull = nodes_.size() == capacity_;
            if (cacheIsFull) {
                // Update LRU node value and make it MRU.
                node = lruRef_.next();
                auto const oldNodeKey = std::move(node->key);
                node->key = std::move(key);
                node->value = std::move(value);
                setMru(node);

                // Remove old node index.
                index_.erase(oldNodeKey);
            } else {
                // Add new node to cache and make it MRU.
                nodes_.push_back(
                    CacheNode{
                        std::move(key)
                        , std::move(value)
                        , mruRef_.prevBase()
                        , &mruRef_
                    }
                );
                node = &nodes_.back();
                mruRef_.prevBase()->nextBase() = node;
                mruRef_.prevBase() = node;
            }

            // Add node index.
            index_.insert(std::make_pair(std::move(key), std::move(node)));
        }
    }

private:
    void setMru(CacheNode* node) noexcept {
        // Remove node from list.
        node->prevBase()->nextBase() = node->nextBase();
        node->nextBase()->prevBase() = node->prevBase();

        // Add node at head (rightmost) list position.
        node->prevBase() = mruRef_.prevBase();
        node->nextBase() = &mruRef_;
        mruRef_.prevBase()->nextBase() = node;
        mruRef_.prevBase() = node;
    }

private:
    int capacity_{};
    CacheNodeBase mruRef_{}; // Most recently used end of list (rightmost list node).
    CacheNodeBase lruRef_{}; // Least recently used end of list (leftmost list node).
    std::vector<CacheNode> nodes_{}; // All nodes; reduces malloc while running.
    std::unordered_map<int, CacheNode*> index_{}; // Nodes index.
};


/*
    Solution #3 (VALID):

    This solution uses a hash table to index into a doubly linked list 
    where each list node contains the value.  A list is used because it 
    maintains the order of the values and provides O(1) insertion and 
    removal.  Searching in the list would take O(1) time if it were done,
    however it is not done, rather the hash table is used to find list 
    items on O(1) time.  A hash table also has O(1) insertion and 
    removal so the cumulative effect of the two data structures is 
    overall O(1) time complexity.  Using a doubly linked list and a
    hash table is a heavy weight solution for caching a small number of 
    small objects, but the problem does state that up to 3000 objects
    may be stored in the cache.  An optimal solution would be to determine
    the threshold at which this solution out performs a simpler trivial 
    implementation with just a vector and linear search and then choose 
    which to use during construction (based on the cache capacity).

    Time: O(1)  [for all operations, except destruction == O(n)]
    Space: O(n)  [n = cache capacity]
*/
class LRUCache_Solution3 {
    using list_t = std::list<std::pair<int, int>>;

    void setMru(list_t::iterator node) noexcept {
        nodes_.splice(nodes_.end(), nodes_, node);
    }

public:
    LRUCache_Solution3(int capacity) : capacity_{std::move(capacity)} {}
    
    int get(int key) {
        auto const iter = index_.find(key);
        auto const found = index_.end() != iter;
        return found ? (setMru(iter->second), iter->second->second) : -1;
    }
    
    void put(int key, int value) {
        auto const iter = index_.find(key);
        auto const found = index_.end() != iter;
        if (found) {
            // Update node value and make node MRU.
            iter->second->second = std::move(value);
            setMru(iter->second);
        } else {
            list_t::iterator node{};
            
            auto const cacheIsFull = nodes_.size() == capacity_;
            if (cacheIsFull) {
                // Update LRU node value and make it MRU.
                node = nodes_.begin();
                index_.erase(node->first); // Remove old node index.
                node->first = std::move(key);
                node->second = std::move(value);
                setMru(node);
            } else {
                // Add new node to cache and make it MRU.
                nodes_.push_back(std::make_pair(std::move(key), std::move(value)));
                node = --nodes_.end();
            }

            // Add node index.
            index_.insert(std::make_pair(std::move(key), std::move(node)));
        }
    }

private:
    int capacity_{};
    list_t nodes_{};
    std::unordered_map<int, list_t::iterator> index_{}; // Nodes index.
};

// [----------------(120 columns)---------------> Module Code Delimiter <---------------(120 columns)----------------]

//#define LRUCache LRUCache_Solution1
//#define LRUCache LRUCache_Solution2
#define LRUCache LRUCache_Solution3

void testLruCache(LRUCache& solution, vector<vector<int>> control)
{
    for (auto const& op : control)
    {
        assert(!op.empty());
        switch(static_cast<char>(op[0]))
        {
            case 'g':
                assert(3 <= op.size());
                if (3 <= op.size())
                {
                    CHECK(op[1] == solution.get(op[2]));
                }
                break;
            
            case 'p':
                assert(3 <= op.size());
                if (3 <= op.size())
                {
                    solution.put(op[1], op[2]);
                }
                break;
            
            default:
                assert(false);
                break;
        }
    }
}

TEST_CASE("Case 1")
{
    cerr << "Case 1" << '\n';
    { // New scope.
        auto solution = LRUCache{2};
        auto const start = std::chrono::steady_clock::now();
        testLruCache(
            solution
            , {
                {'p',1,1}
                ,{'p',2,2}
                ,{'g',1, 1}
                ,{'p',3,3}
                ,{'g',-1,2}
                ,{'p',4,4}
                ,{'g',-1,1}
                ,{'g',3,3}
                ,{'g',4,4}
            }
        );
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

/*
    Input
        ["LRUCache","put","put","get","put","put","get"]
        [[2],[2,1],[2,2],[2],[1,1],[4,1],[2]]

    Output
        [null,null,null,1,null,null,-1]
    
    Expected
        [null,null,null,2,null,null,-1]
*/
TEST_CASE("Case 2")
{
    cerr << "Case 2" << '\n';
    { // New scope.
        auto solution = LRUCache{2};
        auto const start = std::chrono::steady_clock::now();
        testLruCache(
            solution
            , {
                {'p',2,1}
                ,{'p',2,2}
                ,{'g',2, 2}
                ,{'p',1,1}
                ,{'p',4,1}
                ,{'g',-1,2}
            }
        );
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

/*
    Input
        ["LRUCache","put","put","put","put","put","get","put","get","get","put","get","put","put","put","get","put","get","get","get","get","put","put","get","get","get","put","put","get","put","get","put","get","get","get","put","put","put","get","put","get","get","put","put","get","put","put","put","put","get","put","put","get","put","put","get","put","put","put","put","put","get","put","put","get","put","get","get","get","put","get","get","put","put","put","put","get","put","put","put","put","get","get","get","put","put","put","get","put","put","put","get","put","put","put","get","get","get","put","put","put","put","get","put","put","put","put","put","put","put"]
        [[10],[10,13],[3,17],[6,11],[10,5],[9,10],[13],[2,19],[2],[3],[5,25],[8],[9,22],[5,5],[1,30],[11],[9,12],[7],[5],[8],[9],[4,30],[9,3],[9],[10],[10],[6,14],[3,1],[3],[10,11],[8],[2,14],[1],[5],[4],[11,4],[12,24],[5,18],[13],[7,23],[8],[12],[3,27],[2,12],[5],[2,9],[13,4],[8,18],[1,7],[6],[9,29],[8,21],[5],[6,30],[1,12],[10],[4,15],[7,22],[11,26],[8,17],[9,29],[5],[3,4],[11,30],[12],[4,29],[3],[9],[6],[3,4],[1],[10],[3,29],[10,28],[1,20],[11,13],[3],[3,12],[3,8],[10,9],[3,26],[8],[7],[5],[13,17],[2,27],[11,15],[12],[9,19],[2,15],[3,16],[1],[12,17],[9,1],[6,19],[4],[5],[5],[8,1],[11,7],[5,2],[9,28],[1],[2,2],[7,4],[4,22],[7,24],[9,26],[13,28],[11,26]]

    Output
        [null,null,null,null,null,null,-1,null,19,17,null,-1,null,null,null,-1,null,-1,5,-1,12,null,null,3,5,5,null,null,1,null,-1,null,30,5,30,null,null,null,-1,null,-1,24,null,null,18,null,null,null,null,-1,null,null,18,null,null,11,null,null,null,null,null,-1,null,null,24,null,4,29,-1,null,-1,11,null,null,null,null,29,null,null,null,null,17,22,-1,null,null,null,24,null,null,null,20,null,null,null,-1,-1,-1,null,null,null,null,20,null,null,null,null,null,null,null]
    
    Expected
        [null,null,null,null,null,null,-1,null,19,17,null,-1,null,null,null,-1,null,-1,5,-1,12,null,null,3,5,5,null,null,1,null,-1,null,30,5,30,null,null,null,-1,null,-1,24,null,null,18,null,null,null,null,-1,null,null,18,null,null,-1,null,null,null,null,null,18,null,null,-1,null,4,29,30,null,12,-1,null,null,null,null,29,null,null,null,null,17,22,18,null,null,null,-1,null,null,null,20,null,null,null,-1,18,18,null,null,null,null,20,null,null,null,null,null,null,null]
*/
TEST_CASE("Case 3")
{
    cerr << "Case 3" << '\n';
    { // New scope.
        auto solution = LRUCache{2};
        auto const start = std::chrono::steady_clock::now();
        //
        //!\todo TODO: >>> Under Construction <<<
        //
        cerr << "Elapsed time: " << elapsed_time_t{start} << '\n';
    }
    cerr << '\n';
}

/*
    End of "main.cpp"
*/
