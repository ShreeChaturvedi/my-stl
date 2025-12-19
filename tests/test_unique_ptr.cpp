#include "test.hpp"

#include "unique-ptr/unique_ptr.hpp"

struct DropCounter {
  static inline int drops = 0;
  int value = 0;
  ~DropCounter() {
    ++drops;
  }
};

TEST_CASE("unique_ptr: destroys owned object") {
  DropCounter::drops = 0;
  {
    unique_ptr<DropCounter> p(new DropCounter{});
  }
  CHECK_EQ(DropCounter::drops, 1);
}
