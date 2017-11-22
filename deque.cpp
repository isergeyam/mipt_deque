#include "deque.h"
#include <iostream>
using namespace std ;
int main () {
	Deque<pair<int, int> > deq ;
	string s ; 
	while(!cin.eof()) {
		cin >> s ;
		if (s=="back")
			cout << deq.begin()->first << endl ;
		else if (s=="front")
			cout << deq.front().first << endl ;
		else if (s=="[]") {
			int c ;
			cin >> c ;
			cout << deq[c].first << endl ;
		}
		else if (s=="empty") 
			cout << deq.empty() << endl ;
		else if (s=="push_back") {
			pair<int, int> c ;
			cin >> c.first >> c.second ;
			deq.push_back(c) ;
		}
		else if (s=="push_front") {
			pair<int, int> c ;
			cin >> c.first >> c.second ;
			deq.push_front(c) ;
		}
		else if (s=="pop_back") {
			deq.pop_back() ;		
		}
	}
	return 0 ;
}
