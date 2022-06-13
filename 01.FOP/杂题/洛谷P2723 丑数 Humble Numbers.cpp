/*源自USACO Training Section 3.1*/
#include<algorithm>
#include<cstdio>
#define INT_MAX       2147483647    // maximum (signed) int value
using namespace std;

int a[100001] = { 1 };
int prime[101], mark[101];

int main() {
	int k, n;
	scanf("%d %d", &k, &n);
	for (int i = 1; i <= k; i++) scanf("%d", &prime[i]);

	for (int i = 1; i <= n;i++ ) {
		a[i] = INT_MAX;
		for (int j = 1; j <= k; j++) {
			while (prime[j] * a[mark[j]] <= a[i - 1] && mark[j] < i)
				mark[j]++;

			a[i] = min(a[i], prime[j] * a[mark[j]]);
		}
	}
	printf("%d", a[n]);
	return 0;
}