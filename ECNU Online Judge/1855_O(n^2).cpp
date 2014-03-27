#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

// 每次在能达到的范围内油量最大的站加油，注意每停一次后，可达到的范围是动态变化的
// 先将所有加油站按照此规则排序
// 复杂度： O(n^2)
int N, L, P;

struct stop {
	int dis;
	int fuel;
};

vector<stop> vec;

int cmp(const stop &a, const stop &b)
{
	if(a.fuel == b.fuel)
		return a.dis < b.dis; // 因为 dis 是从 town 到加油站的距离，而非起点到加油站的距离
	return a.fuel > b.fuel;
}

int get_minimal_stop()
{
	int total_fuel = P;
	vector<stop>::iterator it;

	for(int ans = 0; ans < N; ++ans) {
		if(total_fuel >= L)
			return ans;

		int tmp = total_fuel;

		for(it = vec.begin(); it != vec.end(); ++it) {
			if(total_fuel >= L - it->dis) {
				total_fuel += it->fuel;
				vec.erase(it);
				break;
			}
		}
		if(tmp == total_fuel)
			// no stop was added and before this: total_fuel < L,
			// so the town is not reachable
			return -1;
	}
	return -1;
}

int main(int argc, char const *argv[])
{

	int dis, fuel;

	while(scanf("%d", &N) != EOF) {
		stop tmp;
		for(int i = 0; i < N; ++i) {
			scanf("%d%d", &dis, &fuel);
			tmp.dis = dis;
			tmp.fuel = fuel;
			vec.push_back(tmp);
		}
		scanf("%d%d", &L, &P);

		sort(vec.begin(), vec.end(), cmp);

		cout << get_minimal_stop() << endl;
	}
	return 0;
}