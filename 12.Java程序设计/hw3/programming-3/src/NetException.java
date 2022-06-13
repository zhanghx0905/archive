@SuppressWarnings("serial")
class NetException extends Exception
{
	public NetException(String message) {
		super("network attack: " + message); 
	}
}