import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

class Hacker implements Runnable {
	@Override
	public void run() {
		int key, data;
		while (true) {
			try {
				synchronized (Cloud.class) {
					key = Cloud.getKey();
					data = Cloud.getData();
					Socket socket = new Socket(InetAddress.getLoopbackAddress(), 11111);
					PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
					out.println(key ^ data);
					Cloud.class.wait();
				}
			} catch (Exception e) {
			}
		}
	}
}
