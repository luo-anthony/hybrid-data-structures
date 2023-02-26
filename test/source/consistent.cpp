#include <doctest/doctest.h>

#include <algorithm>
#include <string>
#include <vector>

#include "hybridstructures/consistentHash.hpp"
#include "hybridstructures/version.h"

TEST_CASE("version") {
  static_assert(std::string_view(HYBRIDSTRUCTURES_VERSION) == std::string_view("1.0"));
  CHECK(std::string(HYBRIDSTRUCTURES_VERSION) == std::string("1.0"));
}

TEST_CASE("Consistent Hash Ring | Get All Nodes") {
  int replicationFactor = 61;
  HybridStructures::ConsistentHashRing<int> hashRing(replicationFactor);
  std::vector<int> nodes;
  int nNodes = 21;
  for (int i = 1; i <= nNodes; i++) {
    hashRing.addNode(i);
    nodes.push_back(i);
  }

  REQUIRE(hashRing.size() == nNodes);
  REQUIRE(hashRing.containsNode(nNodes) == true);
  REQUIRE(hashRing.containsNode(1) == true);

  SUBCASE("Get All Nodes Basic") {
    auto allNodes = hashRing.getAllNodes();
    std::sort(allNodes.begin(), allNodes.end());
    CHECK(allNodes == nodes);
  }

  SUBCASE("Get All Nodes works after adding") {
    auto base = 123;
    for (int i = base; i <= nNodes + base; i++) {
      hashRing.addNode(i);
      nodes.push_back(i);
    }
    auto allNodes = hashRing.getAllNodes();
    std::sort(allNodes.begin(), allNodes.end());
    CHECK(allNodes == nodes);
  }

  SUBCASE("Get All Nodes works after removing") {
    auto removal = 17;
    nodes.erase(nodes.begin(), nodes.begin() + removal);
    for (int i = 1; i <= removal; i++) {
      hashRing.removeNode(i);
    }

    auto allNodes = hashRing.getAllNodes();
    std::sort(allNodes.begin(), allNodes.end());
    CHECK(allNodes == nodes);
  }
}

TEST_CASE("Consistent Hash Ring | Get Closest N Nodes") {
  int replicationFactor = 61;
  HybridStructures::ConsistentHashRing<int> hashRing(replicationFactor);
  std::vector<int> nodes;
  int nNodes = 21;
  for (int i = 1; i <= nNodes; i++) {
    hashRing.addNode(i);
    nodes.push_back(i);
  }

  REQUIRE(hashRing.size() == nNodes);
  REQUIRE(hashRing.containsNode(nNodes) == true);
  REQUIRE(hashRing.containsNode(1) == true);

  auto k1 = "test";

  SUBCASE("Get Closest N Nodes works in range") {
    auto one = hashRing.getClosestN(k1, 1);
    CHECK(std::find(nodes.begin(), nodes.end(), one[0]) != nodes.end());

    auto all = hashRing.getClosestN(k1, nNodes);
    CHECK(all.size() == nNodes);
    // TODO Issue #10
    // std::sort(all.begin(), all.end());
    // CHECK(all == nodes);

    auto none = hashRing.getClosestN(k1, 0);
    CHECK(none.size() == 0);
  }

  SUBCASE("Get Closest N Nodes throws runtime error out of range") {
    CHECK_THROWS_AS(hashRing.getClosestN(k1, -1), std::runtime_error);
    CHECK_THROWS_AS(hashRing.getClosestN(k1, nNodes + 1), std::runtime_error);
    for (int i = 1; i <= nNodes; i++) {
      hashRing.removeNode(i);
    }
    REQUIRE(hashRing.size() == 0);
    CHECK_THROWS_AS(hashRing.getClosestN(k1, 5), std::runtime_error);
  }
}

TEST_CASE("Consistent Hash Ring | Locate Key") {
  int replicationFactor = 61;
  HybridStructures::ConsistentHashRing<int> hashRing(replicationFactor);
  std::vector<int> nodes;
  int nNodes = 5;
  for (int i = 1; i <= nNodes; i++) {
    hashRing.addNode(i);
    nodes.push_back(i);
  }

  REQUIRE(hashRing.size() == nNodes);
  REQUIRE(hashRing.containsNode(nNodes) == true);
  REQUIRE(hashRing.containsNode(1) == true);

  auto k1 = "test";

  SUBCASE("Located Key changes after adding/removing nodes") {
    auto n1 = hashRing.locateKey(k1);
    CHECK(std::find(nodes.begin(), nodes.end(), n1) != nodes.end());

    hashRing.addNode(nNodes + 1);
    for (int i = 1; i <= nNodes; i++) {
      hashRing.removeNode(i);
    }
    CHECK(hashRing.locateKey(k1) == nNodes + 1);
    CHECK(hashRing.locateKey(k1) != n1);
  }

  // NOTE - each subcase has SEPERATE state, and state is discarded after
  SUBCASE("Located Key throws runtime error after removing all nodes, works after adding back") {
    for (int i = 1; i <= nNodes; i++) {
      hashRing.removeNode(i);
    }
    REQUIRE(hashRing.size() == 0);
    CHECK_THROWS_AS(hashRing.locateKey(k1), std::runtime_error);

    for (int i = 1; i <= nNodes; i++) {
      hashRing.addNode(i);
    }
    CHECK(std::find(nodes.begin(), nodes.end(), hashRing.locateKey(k1)) != nodes.end());
  }
}

TEST_CASE("Consistent Hash Ring | Empty Ring") {
  int replicationFactor = 61;
  HybridStructures::ConsistentHashRing<int> hashRing(replicationFactor);
  CHECK(hashRing.size() == 0);
  CHECK(hashRing.containsNode(replicationFactor) == false);
  CHECK(hashRing.removeNode(replicationFactor) == false);
  CHECK(hashRing.getAllNodes().size() == 0);

  CHECK_THROWS_AS(hashRing.locateKey("blahblah"), std::runtime_error);
  CHECK_THROWS_AS(hashRing.getClosestN("blahblah", 4), std::runtime_error);
}

TEST_CASE("Consistent Hash Ring | Min Replication Factor") {
  const int minReplicationFactor = 5;
  HybridStructures::ConsistentHashRing<int> hashRing(-24);
  CHECK(hashRing.getReplicationFactor() == minReplicationFactor);
}

TEST_CASE("Consistent Hash Ring | Add Nodes & Contains Node") {
  int replicationFactor = 61;
  HybridStructures::ConsistentHashRing<int> hashRing(replicationFactor);
  int nNodes = 10;
  for (int i = 1; i <= nNodes; i++) {
    hashRing.addNode(i);
  }

  REQUIRE(hashRing.size() == nNodes);
  REQUIRE(hashRing.containsNode(nNodes) == true);
  REQUIRE(hashRing.containsNode(1) == true);

  SUBCASE("Add duplicate node(s) should not change size") {
    hashRing.addNode(1);
    hashRing.addNode(nNodes);
    hashRing.addNode(4);
    CHECK(hashRing.size() == nNodes);
  }

  SUBCASE("Add new nodes should increase size") {
    auto newNode = 15;
    hashRing.addNode(newNode);
    CHECK(hashRing.size() == nNodes + 1);
    CHECK(hashRing.containsNode(newNode) == true);
  }
}

TEST_CASE("Consistent Hash Ring | Remove Nodes & Contains Node") {
  int replicationFactor = 61;
  HybridStructures::ConsistentHashRing<int> hashRing(replicationFactor);
  int nNodes = 10;
  for (int i = 1; i <= nNodes; i++) {
    hashRing.addNode(i);
  }

  REQUIRE(hashRing.size() == nNodes);
  REQUIRE(hashRing.containsNode(nNodes) == true);
  REQUIRE(hashRing.containsNode(1) == true);

  SUBCASE("Removing invalid node should return false") {
    auto invalid = nNodes + 27;
    CHECK(hashRing.containsNode(invalid) == false);
    CHECK(hashRing.removeNode(invalid) == false);
    CHECK(hashRing.size() == nNodes);
  }

  SUBCASE("Remove valid nodes should return true") {
    CHECK(hashRing.removeNode(nNodes) == true);
    CHECK(hashRing.removeNode(1) == true);
    CHECK(hashRing.size() == nNodes - 2);
    CHECK(hashRing.containsNode(nNodes) == false);
    CHECK(hashRing.containsNode(1) == false);
  }
}

TEST_CASE("Consistent Hash | Integration Test") {
  // simulate typical usage and test
  int replicationFactor = 61;
  HybridStructures::ConsistentHashRing<int> hashRing(replicationFactor);

  int nNodes = 47;
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

  for (int i = 0; i < 15; i++) {
    auto key = "testtest" + std::to_string(i);
    CHECK(std::find(expectedNodes.begin(), expectedNodes.end(), hashRing.locateKey(key))
          != expectedNodes.end());
    CHECK(hashRing.getClosestN(key, 5).size() == 5);
  }

  auto key = "test12345";
  auto ogNode = hashRing.locateKey(key);

  int nodesToRemove = nNodes - 1;
  for (int i = 1; i <= nodesToRemove; i++) {
    hashRing.removeNode(i);
  }
  CHECK(hashRing.size() == nNodes - nodesToRemove);
  CHECK(hashRing.getAllNodes().size() == nNodes - nodesToRemove);

  auto nNode = hashRing.locateKey(key);
  CHECK(ogNode != nNode);
}

// TODO test performance with many nodes

// struct node_t {
//   std::string name;
//   int val;

//   std::string toString() {
//     return "N: " + name + " | Val: " + std::to_string(val);
//   }
// };
// see https://www.fluentcpp.com/2017/06/06/using-tostring-custom-types-cpp/ for supporting custom
// types

// TEST_CASE("Consistent Hash Ring | Complex Type") {
//   int replicationFactor = 61;
//   HybridStructures::ConsistentHashRing<node_t> hashRing(replicationFactor);
//   node_t n1 = {"apple", 5};
//   node_t n2 = {"orange", 10};
//   hashRing.addNode(n1);
//   hashRing.addNode(n2);
//   CHECK(hashRing.size() == 2);
//   CHECK(hashRing.containsNode(n2));
//   hashRing.removeNode(n2);
//   CHECK(hashRing.size() == 1);
//   auto n = hashRing.locateKey("test");
//   CHECK(n == n1);
// }