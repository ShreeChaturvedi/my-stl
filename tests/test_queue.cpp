#include "test.hpp"

#include "queue/queue.hpp"

TEST_CASE("Queue: push/front/pop") {
  Queue<int> q;
  CHECK(q.empty());

  q.push(1);
  q.push(2);
  q.push(3);

  CHECK_EQ(q.front(), 1);
  CHECK_EQ(q.back(), 3);

  q.pop();
  CHECK_EQ(q.front(), 2);
}

