
class BigInteger implements EasyInteger {
	private int[] num;

	/**
	 * This constructor convert String to BigInteger.
	 * 
	 * @param s the input String, only contains character 0-9
	 */
	BigInteger(final String s) {
		this.num = new int[(s.length()>0) ? s.length() : 1];
		for (int i = s.length() - 1; i >= 0; i--)
			this.num[s.length() - 1 - i] = s.charAt(i) - '0';
	}

	/**
	 * Calculate the value of (this + val). This method don't change input
	 * arguments.
	 * 
	 * @param val The value to add to this
	 * @return a new Integer whose value is (this + val)
	 */
	@Override
	public EasyInteger add(final EasyInteger val) {
		final BigInteger v = (BigInteger) val;
		int length = Math.max(this.num.length, v.num.length) + 1;
		int[] num = new int[length];
		for (int i = 0; i < this.num.length; i++) {
			num[i] = this.num[i];
		}
		for (int i = 0; i < v.num.length; i++) {
			num[i] += v.num[i];
		}
		for (int i = 0; i < length - 1; i++) {
			num[i + 1] += num[i] / 10;
			num[i] %= 10;
		}
		int newLen = length;
		for (int i = length - 1; i >= 0; i--) {
			if (num[i] != 0) break;
			else newLen = i;		
		}
		if (newLen == 0)
			newLen = 1;
		
		BigInteger result = new BigInteger("");
		result.num = new int[newLen];
		for (int i = 0; i < newLen; i++) {
			result.num[i] = num[i];
		}
		return result;
	}

	/**
	 * Convert this to String.
	 * 
	 * @return the String representation of this
	 */
	@Override
	public String toString() {
		final StringBuilder s = new StringBuilder();
		for (int i = this.num.length - 1; i >= 0; i--)
			s.append((char) ('0' + this.num[i]));
		return s.toString();
	}
}