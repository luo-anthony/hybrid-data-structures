#include <doctest/doctest.h>

#include <algorithm>
#include <string>
#include <vector>

#include "hybridstructures/consistentHash.hpp"
#include "hybridstructures/version.h"

TEST_CASE("Consistent Hash - Integration?") {
  HybridStructures::ConsistentHashRing<int> hashRing(10);
  int nNodes = 8;
  std::vector<int> expectedNodes;
  for (int i = 1; i <= nNodes; i++) {
    hashRing.addNode(i);
    expectedNodes.push_back(i);
  }

  CHECK(hashRing.size() == nNodes);
  auto allNodes = hashRing.getAllNodes();
  CHECK(allNodes.size() == nNodes);
  std::sort(allNodes.begin(), allNodes.end());
  CHECK(expectedNodes == allNodes);

  auto key = "abcdefg";
  auto ogNode = hashRing.locateKey(key);

  auto closestN = 2;
  std::vector<int> ogclosestN = hashRing.getClosestN(key, closestN);
  CHECK(ogclosestN.size() == closestN);

  int nodesToRemove = 6;
  for (int i = 1; i <= nodesToRemove; i++) {
    hashRing.removeNode(i);
  }
  CHECK(hashRing.size() == nNodes - nodesToRemove);
  CHECK(hashRing.getAllNodes().size() == nNodes - nodesToRemove);

  auto nNode = hashRing.locateKey(key);
  CHECK(ogNode != nNode);
  // // run ctest with -V flag to see the output
  // std::cerr << "Original Node: " << ogNode << "  | New Node: " << nNode << std::endl;

  std::vector<int> nclosestN = hashRing.getClosestN(key, closestN);
  CHECK(nclosestN.size() == closestN);
  CHECK(nclosestN != ogclosestN);
}

TEST_CASE("version") {
  static_assert(std::string_view(HYBRIDSTRUCTURES_VERSION) == std::string_view("1.0"));
  CHECK(std::string(HYBRIDSTRUCTURES_VERSION) == std::string("1.0"));
}