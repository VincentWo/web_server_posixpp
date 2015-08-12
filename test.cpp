#include <iostream>

using namespace std;
int main()
{
	std::string test{"field-name:field-value"};

	auto index = test.find(':');
	auto value = test.substr(index+1);
	test.erase(index);
	std::cout << value << '\n';
	std::cout << test << '\n';
}
