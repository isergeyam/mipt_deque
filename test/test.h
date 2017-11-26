#include <gtest/gtest.h>
#include <algorithm>
#include "../src/deque.h"
#include <deque>
#include <vector>
#include <cmath>
namespace{
	class DequeTest : public testing::Test {
		protected:
			mydeque::Deque<int> d0_, d1_, d2_, d3_, d_rand ;
			std::deque<int> d_std ;
			std::vector<int> array ;
			static size_t test_max ;
			static const int PU_BACK=0 ;
			static const int PU_FRONT=1 ;
			static const int PO_BACK=2 ;
			static const int PO_FRONT=3 ;
			DequeTest() {
				d1_.push_back(1) ;
				d2_.push_back(2) ;
				d2_.push_back(3) ;
				d3_.push_back(4) ;
				d3_.push_back(5) ;
				d3_.push_back(6) ;
				d3_.push_back(7) ;
				d3_.push_back(8) ;
				d3_.push_back(9) ;
				d3_.push_back(10) ;
				array=std::vector<int>(test_max);
				std::srand(std::time(NULL)) ;
				std::generate(array.begin(), array.end(), std::rand) ;
				d_rand=mydeque::Deque<int>(array.begin(), array.end()) ;
				d_std=std::deque<int>(array.begin(), array.end()) ;
			}
		public:
			template<typename Deque>
			static void process_ops(Deque &deq, const std::vector<std::pair<int, int> > &ops) {
				for (auto it=ops.begin();it!=ops.end();++it) {
					switch(it->first) {
						case PU_BACK:
							deq.push_back(it->second) ;
							break ;
						case PU_FRONT:
							deq.push_front(it->second) ;
							break ;
						case PO_BACK:
							deq.pop_back() ;
							break ;
						case PO_FRONT:
							deq.pop_front() ;
							break ;
					}
				}
				return ;
			}
			static void generate_ops(size_t sz, size_t cur_sz, std::vector<std::pair<int, int> > &ops) {
				while(sz) {
					std::pair<int, int> cur(std::rand()%4, std::rand()) ;
					if (cur.first>2) {
						if (!cur_sz)
							continue ;
						else
							--cur_sz ;
					}
					else
						++cur_sz ;
					ops.push_back(cur) ;
					--sz ;
				}
				return ;
			}
	};
	TEST_F(DequeTest, DefaultConstructor) {
		EXPECT_EQ(0u, d0_.size()) ;
		EXPECT_TRUE(d0_.empty()) ;
	}
	TEST_F(DequeTest, MAIN_FUNCTIONALITY) {
		d1_.pop_back() ;
		EXPECT_EQ(0u, d1_.size()) ;
		EXPECT_TRUE(d1_.empty()) ;
		d2_.pop_back() ;
		EXPECT_EQ(1, d2_.size()) ;
		EXPECT_FALSE(d2_.empty()) ;
		EXPECT_EQ(2, d2_.back()) ;
		d2_.pop_back() ;
		EXPECT_TRUE(d1_.empty()) ;
		d3_.pop_back() ;
		EXPECT_EQ(9, d3_.back()) ;
		d3_.pop_front() ;
		EXPECT_EQ(5, d3_.front()) ;
		EXPECT_EQ(5, d3_.size()) ;
		std::vector<std::pair<int, int> > ops ;
		DequeTest::generate_ops(DequeTest::test_max, DequeTest::test_max, ops) ;
		DequeTest::process_ops(d_rand, ops) ;
		DequeTest::process_ops(d_std, ops) ;
		EXPECT_TRUE(d_rand.size()==d_std.size() && std::equal(d_rand.begin(), d_rand.end(), d_std.begin())) ;
	}
	TEST_F(DequeTest, Iterator) {
		EXPECT_TRUE(d0_.begin() == d0_.end()) ;
		for (auto it=d1_.begin(); it!=d1_.end();++it) {
			EXPECT_EQ(*it, 1) ;
		}
		EXPECT_EQ(d3_.begin()-d3_.end(), -7) ;
		for (auto it=d3_.begin(); it!=d3_.end();++it) {
			EXPECT_EQ(*it, 4+(it-d3_.begin())) ;
		}
		auto it=d2_.begin() ;
		*it=4 ;
		EXPECT_EQ(d2_.front(), 4) ;
	}
	TEST_F(DequeTest, ReverseIterator) {
		EXPECT_EQ(d0_.rbegin(), d0_.rend()) ;
		for (auto rit=d1_.rbegin(); rit!=d1_.rend();++rit) {
			EXPECT_EQ(*rit, 1) ;
		}
		EXPECT_EQ(d3_.rbegin()-d3_.rend(), -7) ;
		for (auto rit=d3_.rbegin(); rit!=d3_.rend();++rit) {
			EXPECT_EQ(*rit, 10-(rit-d3_.rbegin())) ;
		}
		auto rit=d2_.rbegin() ;
		*rit=4 ;
		EXPECT_EQ(d2_.back(), 4) ;
	}
	TEST(ComplexityTest, ComplexityTest) {
		const int samples_min=3, samples_max=8 ;
		std::vector<double> stumps ;
		const double error=0.01 ;
		for (int i=samples_min;i<=samples_max;++i) {
			const int cur_size=static_cast<int>(pow(10, i)) ;
			mydeque::Deque<int> deq ;
			std::vector<std::pair<int, int> > ops ;
			DequeTest::generate_ops(cur_size, 0, ops) ;
			const clock_t start_time=std::clock() ;
			DequeTest::process_ops(deq, ops) ;
			stumps.push_back(static_cast<double>(std::clock()-start_time)/static_cast<double>(cur_size)) ;
		}
		for (size_t i=0;i<stumps.size()-1;++i) {
			for(size_t j=i+1;j<stumps.size();++j) {
				EXPECT_NEAR(stumps[i], stumps[j], error) ;
			}
		}
	}
} //namespace
