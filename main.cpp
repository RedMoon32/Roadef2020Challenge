#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "cchecker.h"
#include "json.h"

using namespace std;
using namespace nlohmann;

int main(){
	cout << " Hello! " << endl;
	json res = readFile("../example1.json");
	parseJson(res);
	return 0;
}
