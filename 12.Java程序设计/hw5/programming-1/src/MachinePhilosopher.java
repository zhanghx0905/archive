class MachinePhilosopher implements Runnable {
	public final int id;

	MachinePhilosopher(final int id) {
		this.id = id;
	}

	@Override
	public void run() {
		Com com = Com.getInstance();
		int number = com.getNumber();
		synchronized (com) {
			while (true) {
				try {
					com.wait();
				} catch (InterruptedException e) {
				}
				int tmp = com.getNumber();
				if (number == tmp)
					continue;
				number = tmp;
				com.star(number, id);
			}
		}
	}
}
