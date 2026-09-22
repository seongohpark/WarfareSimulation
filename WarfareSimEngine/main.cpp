#include <iostream>

#include "WarfareSimEngine.h"

using namespace std;

int main(void)
{
	cout << "WarfareSimEngine Start" << endl;

	WarfareSimEngine::GetInstance()->Start();
}
