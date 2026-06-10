// Copyright 2026 J. Carruthers <jbc@bu.edu>
// SPDX-License-Identifier: MIT
// Purpose: singly-linked list owned by unique_ptr<Node> -- ownership chain + recursion.
// Topic: linked-structures
// Category: demo
// Lecture: L12

// L12, part 1. A singly-linked list where each node OWNS the next via
// std::unique_ptr<Node>. This is the first data structure where you
// genuinely NEED an owning pointer (L11): the list has a variable number
// of nodes, created at runtime, each outliving the function that made it.
//
// Two ideas land here:
//
//   * The ownership chain. head owns node0, node0 owns node1, ... and the
//     LAST node's `next` is nullptr -- the tail marker. (Tony Hoare's
//     "billion-dollar mistake", now tamed: nullptr means "end of list,"
//     and unique_ptr makes a leaked or dangling node impossible to write.)
//     Destroy `head` and the WHOLE chain frees automatically -- no
//     `delete` in sight. RAII, generalized from L6/L11.
//
//   * Recursion mirrors the structure. A list is either empty (nullptr)
//     or a node followed by a smaller list. Every traversal is "handle
//     this node, then recurse on next; base case is nullptr." The code
//     shape mirrors the data shape.
//
// Build-to-learn, use-to-ship: we write this to UNDERSTAND pointers +
// recursion. In production you would reach for std::forward_list /
// std::list -- and far more often std::vector, whose contiguous elements
// are dramatically friendlier to the cache. See the closing note.

#include <format>
#include <iostream>
#include <memory>
#include <utility>

namespace {

// A self-referential type: a Node holds a unique_ptr to the NEXT Node.
// `next == nullptr` marks the tail. unique_ptr<Node> means each node owns
// exactly the rest of the list that hangs off it.
struct Node {
  int value;
  std::unique_ptr<Node> next;

  explicit Node(int v, std::unique_ptr<Node> nxt = nullptr)
      : value(v), next(std::move(nxt)) {}
};

// push_front: the new node becomes the head and OWNS the old head.
// unique_ptr is move-only (L11), so we std::move the old head in --
// ownership transfers, it is never copied.
std::unique_ptr<Node> push_front(std::unique_ptr<Node> head, int value) {
  return std::make_unique<Node>(value, std::move(head));
}

// ---- Recursive traversals. The base case is always `n == nullptr`. ----
// We BORROW the nodes through a non-owning raw `const Node*` (L11: .get()
// hands you a borrow that must never delete). The list still owns them.

int length(const Node* n) {
  if (n == nullptr) {
    return 0;  // empty list
  }
  return 1 + length(n->next.get());  // this node, plus the rest
}

int sum(const Node* n) {
  if (n == nullptr) {
    return 0;
  }
  return n->value + sum(n->next.get());
}

bool contains(const Node* n, int target) {
  if (n == nullptr) {
    return false;
  }
  return n->value == target or contains(n->next.get(), target);
}

// The iterative twin of a traversal: walk a borrowed pointer to nullptr.
void print(const Node* head) {
  std::cout << '[';
  const char* sep = "";
  for (const Node* p = head; p != nullptr; p = p->next.get()) {
    std::cout << sep << p->value;
    sep = " -> ";
  }
  std::cout << " -> nullptr]\n";
}

}  // namespace

int main() {
  std::unique_ptr<Node> head;  // an empty list IS nullptr

  for (const int v : {3, 2, 1}) {
    head = push_front(std::move(head), v);  // builds 1 -> 2 -> 3
  }

  print(head.get());
  std::cout << std::format("length = {}, sum = {}\n", length(head.get()),
                           sum(head.get()));
  std::cout << std::format("contains 2? {}\n", contains(head.get(), 2));
  std::cout << std::format("contains 9? {}\n", contains(head.get(), 9));

  // No delete anywhere. When `head` leaves scope here, its destructor frees
  // node 1, whose destructor frees node 2, whose destructor frees node 3 --
  // the ownership chain unwinds automatically. RAII.
  //
  // Footgun worth naming: for a VERY long list this recursive teardown can
  // overflow the call stack (one frame per node). std::list sidesteps it
  // with an iterative destructor -- one more reason "use to ship."
  return 0;
}
