#include <iostream>
#include <atomic>
#include <vector>
#include <stdbool.h>
#include <thread> 
#include <array>
#include <exception>
#include <string>
#include <math.h>

class mutex_exeption : std::exception {
public:
	mutex_exeption(std::string str) {
		std::cout << str;
	}
};

class peterson_mutex {
public:
	peterson_mutex() {
		want[0].store(false);
		want[1].store(false);
		victim.store(0);
	}
		
	void lock(int t) {
		if (t > 1 || t < 0) {
			throw new mutex_exeption("Wrong thread ID");
		}
		want[t].store(true);
		victim.store(t);
		while (want[1 - t].load() && victim.load() == t) {}
	}
		
	void unlock(int t) {
		if (t > 1 || t < 0) {
			throw new mutex_exeption("Wrong thread ID");
		}
		want[t].store(false);
	}

private:
	std::array<std::atomic<bool>, 2> want;
	std::atomic<int> victim;
};


class tree_mutex {
public:
	tree_mutex(std::size_t num_threads) {
		size = num_threads;
		tree.resize(int(pow(2,int(log(size)) + 1) -1));
	}

	int lock(size_t t) {
		if (t >= size || t < 0) {
			throw new mutex_exeption("Wrong thread ID");
		}
		size_t  i = (t / 2);
		size_t prev = t;
		while (1) {
			try {
				if (i * 2 + 1 == prev) {
					tree[i].lock(0);
				}
				else {
					tree[i].lock(1);
				}
			}
			catch(mutex_exeption) {
				throw new mutex_exeption("Internal problems");
			}
			prev = i;
			i = (i / 2);
			if (prev == 0) {
				break;
			}
		}
	}

	void unlock(size_t t) {
		if (t >= size || t < 0) {
			throw new mutex_exeption("Wrong thread ID");
		}
		size_t  i = (t / 2);
		size_t prev = t;
		while (1) {
			try {
				if (i * 2 + 1 == prev) {
					tree[i].unlock(0);
				}
				else {
					tree[i].unlock(1);
				}
			}
			catch (mutex_exeption) {
				throw new mutex_exeption("Internal problems");
			}
			prev = i;
			i = (i / 2);
			if (prev == 0) {
				break;
			}
		}
	}




private:
	size_t size = -1;
	std::vector<peterson_mutex> tree;
};


int main() {
	return 0;
}