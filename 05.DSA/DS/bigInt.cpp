typedef long long ll;

const int maxn = 10000;

struct bigInt {
	ll* data;
	int size;
	static const int BASE = 1e8;
	static const int WIDTH = 8;
	void standardize() {
		for (int i = size - 1; i >= 0; --i) {
			if (data[i]) break;
			size--;
		}
	}
	bigInt() {
		data = new ll[maxn >> 3];
		memset(data, 0, sizeof(data));
		size = 0;
	}
	bigInt(char* num) :bigInt() {//µÍÎ»ÔÚÇ°, 12345 = "54321"
		int len = strlen(num);
		size = (len - 1) / WIDTH + 1;
		int j = 0;
		for (int i = 0; i < size; i++) {
			for (int k = 1; k < BASE && j < len; k *= 10, j++) {
				data[i] += (num[j] - '0') * k;
			}
		}
	}
	bigInt operator*(const bigInt& a) {
		bigInt tmp;
		tmp.size = size + a.size;
		for (int i = 0; i <= size; ++i)
			for (int j = 0; j <= a.size; ++j)
				tmp.data[i + j] += data[i] * a.data[j];
		for (int k = 0; k < tmp.size; ++k) {
			tmp.data[k + 1] += tmp.data[k] / BASE;
			tmp.data[k] %= BASE;
		}
		tmp.standardize();
		return tmp;
	}
	void print() {
		for (int i = size - 1; i >= 0; --i)
			printf((i != size - 1) ? "%08lld" : "%lld", data[i]);
		if (!size) printf("0");
		printf("\n");
	}
};
inline void srev(char* str) {
	int len = strlen(str) - 1;
	for (int i = 0; i <= len / 2; i++)
		swap(str[i], str[len - i]);
}
