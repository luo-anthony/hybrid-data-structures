#include "hybridstructures/consistentHash.hpp"
#include <vector>
#include <gtest/gtest.h>

TEST(ConsistentHashTest, CheckBasics)
{
    HybridStructures::ConsistentHashRing<int> hashRing(10);
    int nNodes = 8;
    std::vector<int> expectedNodes;
    for(int i = 1; i <= nNodes; i++){
        hashRing.addNode(i);
        expectedNodes.push_back(i);
    }
    ASSERT_EQ(hashRing.size(), nNodes);
    auto allNodes = hashRing.getAllNodes();
    ASSERT_EQ(allNodes.size(), nNodes);
    std::sort(allNodes.begin(), allNodes.end());
    ASSERT_EQ(expectedNodes, allNodes);

    auto key = "abcdefg";
    auto ogNode = hashRing.locateKey(key);

    auto closestN = 2;
    std::vector<int> ogclosestN = hashRing.getClosestN(key, closestN);
    ASSERT_EQ(ogclosestN.size(), closestN);

    int nodesToRemove = 6;
    for(int i = 1; i <= nodesToRemove; i++){
        hashRing.removeNode(i);
    }
    ASSERT_EQ(hashRing.size(), nNodes - nodesToRemove);
    ASSERT_EQ(hashRing.getAllNodes().size(), nNodes - nodesToRemove);

    auto nNode = hashRing.locateKey(key);
    ASSERT_NE(ogNode, nNode); 
    // run ctest with -V flag to see the output 
    std::cerr << "Original Node: " << ogNode << "  | New Node: " << nNode << std::endl;

    std::vector<int> nclosestN = hashRing.getClosestN(key, closestN);
    ASSERT_EQ(nclosestN.size(), closestN);
    ASSERT_NE(nclosestN, ogclosestN);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
