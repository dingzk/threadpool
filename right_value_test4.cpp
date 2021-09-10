#include <iostream>
#include <utility>

#include <queue>

class A {
public:
    A(int a = 10) : m_a(a) { std::cout << "construct a :" << a << std::endl; }
    A(const A &a) {
        std::cout << "copy construct a :" << a.m_a << std::endl;
        this->m_a = a.m_a;
    }
    A &operator=(const A &a) {
        std::cout << "copy assign a :" << a.m_a << std::endl;
        this->m_a = a.m_a;
        return *this;
    }

    A(const A &&a) {
        std::cout << "move construct a :" << a.m_a << std::endl;
        this->m_a = a.m_a;
    }
    A &operator=(const A &&a) {
        std::cout << "move assign a :" << a.m_a << std::endl;
        this->m_a = a.m_a;
        return *this;
    }

    ~A() { std::cout << "destruct a :" << m_a << std::endl; }
    void run() { std::cout << "run a : " << m_a << std::endl; }

	int echo() {
		return m_a;
	}

public:
    int m_a;
};

A pop(std::queue<A> &q)
{
	std::cout << "a" << std::endl;
	//A &&a = std::move(q.front());
	A a = std::move(q.front());
	std::cout << "b" << std::endl;
	q.pop();
	std::cout << "a 的地址为： " << &a << std::endl;

	return a; 
}


int main()
{
	std::queue<A> q;
	q.push(std::move(A(1)));

	std::cout << "c" << std::endl;
	//A a = std::move(pop(q));
	A a = pop(q);
	std::cout << "a' 的地址为： " << &a << std::endl;
	std::cout << "d" << a.echo()  << std::endl;

	return 0;
}
