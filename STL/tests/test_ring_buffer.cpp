#include "test.hpp"

#include "../ring-buffer/buffer.h"

TEST_CASE("RingBuffer: push/pop/peek") {
  RingBuffer<int, 4> b;
  CHECK(b.empty());
  CHECK_EQ(b.push(1), 1u);
  CHECK_EQ(b.push(2), 2u);
  CHECK_EQ(b.peek(), 1);
  CHECK_EQ(b.pop(), 1);
  CHECK_EQ(b.pop(), 2);
  CHECK(b.empty());
  CHECK_THROWS(b.pop());
}

TEST_CASE("RingBuffer: full") {
  RingBuffer<int, 3> b;
  b.push(10);
  b.push(20);
  b.push(30);
  CHECK(b.full());
  CHECK_THROWS(b.push(40));
}

