class Singleton extends BaseSingleton {
    private static Singleton _instance;

    private Singleton() {
    }

    public static synchronized Singleton getInstance() {
        if (_instance == null) {
            _instance = new Singleton();
        }
        return _instance;
    }
}
