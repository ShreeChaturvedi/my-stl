#include "test.hpp"

#include "priority-queue/priority_queue.hpp"

TEST_CASE("PriorityQueue: max by default") {
  PriorityQueue<int> pq;
  pq.push(1);
  pq.push(3);
  pq.push(2);
  CHECK_EQ(pq.top(), 3);
  CHECK_EQ(pq.pop(), 3);
  CHECK_EQ(pq.pop(), 2);
  CHECK_EQ(pq.pop(), 1);
  CHECK(pq.empty());
}
