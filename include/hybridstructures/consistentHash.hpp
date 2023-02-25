#ifndef CONSISTENTHASH_H_
#define CONSISTENTHASH_H_

#include <map>
#include <string>
#include <vector>

namespace HybridStructures {

  // TODO make spacing consistent

  template <typename T> class ConsistentHashRing {
  private:
    const int baseNum = 111;
    const int minReplicas = 5;

    // for now, use a string-based hash
    std::hash<std::string> hashFunc;

    // map is ordered in C++
    // need to worry about thread-safety?
    // This map has all the VIRTUAL NODES
    std::map<size_t, T> internalRing;

    // this map has all the original nodes (no duplicates)
    // the hash value is just the hash value of the object
    std::map<size_t, T> internalNodes;

    int replicationFactor;

    // note: no need for people to pass in custom node to string function since
    // they can just override it in their implementation of the node
    std::string nodeToString(const T& node);

    // C++20 is not universally supported yet, don't use std::format
    // const std::string hashForm = "{} - Repl:{}";
    std::string getNodeRepString(const T& node, int repNum);

  public:
    // TODO need to implement copy constructor?

    ConsistentHashRing(int nReps);

    // TODO: possibly support custom hash
    // ConsistentHashRing(int nReps, std::hash<std::string> customHash);

    // node can be a struct containing some ip address / other details
    void addNode(const T& node);

    bool removeNode(const T& node);

    bool containsNode(const T& node);

    const T& locateKey(const std::string& key);

    int size();

    // TODO: make this function more efficient
    std::vector<T> getClosestN(const std::string& key, int n);

    std::vector<T> getAllNodes();
  };

  template <typename T> ConsistentHashRing<T>::ConsistentHashRing(int nReps)
      : replicationFactor{nReps} {
    replicationFactor = std::max(minReplicas, replicationFactor);
  }

  template <typename T> std::string ConsistentHashRing<T>::nodeToString(const T& node) {
    return std::to_string(node);
  }

  // size should return the number of nodes NOT including the virtual nodes
  template <typename T> int ConsistentHashRing<T>::size() { return internalNodes.size(); }

  template <typename T> bool ConsistentHashRing<T>::containsNode(const T& node) {
    size_t nodeHash = hashFunc(nodeToString(node));
    return (internalNodes.count(nodeHash) == 1);
  }

  template <typename T> std::vector<T> ConsistentHashRing<T>::getAllNodes() {
    std::vector<T> outputNodes;
    for (auto const& [_, val] : internalNodes) {
      outputNodes.push_back(val);
    }

    return outputNodes;
  }

  template <typename T> void ConsistentHashRing<T>::addNode(const T& node) {
    if (containsNode(node)) {
      // already have the node in the hash ring
      return;
    }

    // Add the node to map of nodes
    size_t nHash = hashFunc(nodeToString(node));
    internalNodes[nHash] = node;

    // Add nReplica virtual nodes to the hashRing
    for (int i = 0; i < replicationFactor; i++) {
      auto vNodeString = getNodeRepString(node, i);
      size_t vHash = hashFunc(vNodeString);
      internalRing[vHash] = node;
    }
  }

  template <typename T> bool ConsistentHashRing<T>::removeNode(const T& node) {
    if (!containsNode(node)) {
      return false;
    }

    // Remove the node from the map of nodes
    size_t nHash = hashFunc(nodeToString(node));
    internalNodes.erase(nHash);

    // Remove the same nReplica virtual nodes from the hashRing
    for (int i = 0; i < replicationFactor; i++) {
      auto vNodeString = getNodeRepString(node, i);
      size_t vHash = hashFunc(vNodeString);
      internalRing.erase(vHash);
    }
    return true;
  }

  template <typename T> const T& ConsistentHashRing<T>::locateKey(const std::string& key) {
    if (this->size() == 0) {
      throw std::runtime_error("Error: Hash Ring is empty.");
    }

    size_t hash = hashFunc(key);

    // the node to return is the next node with hash >= hash of the value we want
    auto itr = internalRing.upper_bound(hash);

    if (itr == internalRing.end()) {
      // there is no greater hash value, wrap around to the start of the ring
      return internalRing.begin()->second;  // return the node
    }
    return itr->second;
  }

  // NOTE: this function is currently bugged, might return duplicate nodes since this only looks
  // through virtual nodes
  template <typename T>
  std::vector<T> ConsistentHashRing<T>::getClosestN(const std::string& key, int n) {
    if (this->size() < n) {
      throw std::runtime_error("Error: Hash Ring does not contain enough nodes.");
    }

    size_t hash = hashFunc(key);

    // the node(s) to return starts at the node with hash >= hash of the value we want
    auto itr = internalRing.upper_bound(hash);
    std::vector<T> outputNodes(n);
    for (auto i = 0; i < n; i++) {
      if (itr == internalRing.end()) {
        // there is no greater hash value, wrap around to the start of the ring
        itr = internalRing.begin();
      }
      outputNodes[i] = itr->second;
      itr++;
    }

    return outputNodes;
  }

  template <typename T>
  std::string ConsistentHashRing<T>::getNodeRepString(const T& node, int repNum) {
    return nodeToString(node) + " - RepN: " + std::to_string(repNum + baseNum);
    // return std::format(hashForm, nodeToString(node), std::to_string(repNum + baseNum));
  }

}  // namespace HybridStructures

#endif