#include <stdint.h> 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <queue>
#include <deque>

using namespace std;

class MaxQueue {
	queue<int> q;
	deque<int> maxq;

	public:
    	MaxQueue() {
        
    	}

	void enqueue(int x) {
		q.push(x);

		while (!maxq.empty() && (maxq.back() < x))
			maxq.pop_back();

		maxq.push_back(x);
	}
	    
	int dequeue() {
		if (q.empty())
			return -1;

		int val = q.front();
		q.pop();

		if (maxq.front() == val)
			maxq.pop_front();

		return val;
	}
	    
	int front() {
		if (q.empty())
			return -1;

		return q.front();
	}
	    
	int peekMax() {
		if (!maxq.empty())
			return maxq.front();
		else
			return -1;	
	}
};

int main(int argc, char* argv[])
{
	MaxQueue q;

	q.enqueue(3);
	q.enqueue(2);
	q.enqueue(1);

	printf("front %d max %d\n", q.front(), q.peekMax());
	q.dequeue();
	printf("front %d max %d\n", q.front(), q.peekMax());
	q.enqueue(4);
	printf("front %d max %d\n", q.front(), q.peekMax());
}
