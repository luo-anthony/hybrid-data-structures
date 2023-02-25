#include <algorithm>
#include <string>
#include <vector>

#include "hybridstructures/consistentHash.hpp"

int main() {
  // some examples of how to use the library
  int replicationFactor = 10;
  HybridStructures::ConsistentHashRing<int> hashRing(replicationFactor);
  int nNodes = 20;
  for (int i = 1; i <= nNodes; i++) {
    hashRing.addNode(i);
  }
  int n = hashRing.size();

  std::vector<int> allNodes = hashRing.getAllNodes();

  bool c = hashRing.containsNode(8);

  auto key = "abcdefg";
  auto ogNode = hashRing.locateKey(key);

  std::vector<int> closestThreeNodes = hashRing.getClosestN(key, 3);

  bool s = hashRing.removeNode(5);

  return 0;
}