class NetSecure
{
	public static void check(String packet) throws NetException
	{
		if (packet.toLowerCase().endsWith(".exe"))
			throw new NetException(packet);
	}
}