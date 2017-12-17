#include "test.h"
size_t DequeTest::test_max = static_cast<int>(10e5);
int DequeTest::samples_min = 3;
int DequeTest::samples_max = 8;
int main(int argc, char **argv) {
  DequeTest::init(argc, argv);
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
