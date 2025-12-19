# PriorityQueue<T, Compare, branches>

A heap-based priority queue adapter over `Heap<T, Compare, branches>`.

## Highlights

- Default is max-priority (`Compare = std::greater<T>`).
- D-ary heap via `branches`.

## API Notes

- `top()` returns the highest-priority element.
- `pop()` removes the highest-priority element.

## Complexity

- `push`, `pop`: O(log_base(branches) n)
- `top`: O(1)

## Differences vs `std::priority_queue`

- Uses the custom `Heap` implementation and exposes a `clear`.
- Header-only implementation.

## Example

```cpp
#include "priority-queue/priority_queue.hpp"

PriorityQueue<int> pq;
pq.push(10);
pq.push(3);
int best = pq.top();
pq.pop();
```
