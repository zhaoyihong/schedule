#include <iostream>
#include <vector>
#include    "deny.h"
using namespace std;


int main(void)
{
    swap_pair s1(1,2,3,4),s2(3,4,1,2),s3(1,4,3,2),s4(3,2,1,4);

    cout << s1.cmp(s1) << endl;
    cout << s1.cmp(s2) << endl;
    cout << s1.cmp(s3) << endl;
    cout << s1.cmp(s4) << endl;

    cout << s3.cmp(s4) << endl;

    cout << s1 << endl;

    s1.reverse();

    cout << s1 << endl;
}

