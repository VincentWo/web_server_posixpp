#include <iostream>
#include <string>

using namespace std;
int main()
{
    std::string test1 = "test/../";
    std::string test2 = "test././";
    cout << test1 << endl;
    cout << test1.find("..") << endl;
    cout << test1.find_first_of("..") << endl;

    cout << test2 << endl;
    cout << test2.find("..") << endl;
    cout << test2.find_first_of("..") << endl;
    return 0;
}
