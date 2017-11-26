#include "test.h"
size_t DequeTest::test_max=1000000 ;
int main (int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv) ;
	return RUN_ALL_TESTS() ;
}
