#include "test.h"
size_t DequeTest::test_max = static_cast<int>(10e5);
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
