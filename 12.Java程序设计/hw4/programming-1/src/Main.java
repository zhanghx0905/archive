import java.util.*;

class Main {
    public static void main(String[] args) {
        LinkedHashSet<String> hash = new LinkedHashSet<>();
        Scanner stdin = new Scanner(System.in);
        while (stdin.hasNextLine()) {
            String line = stdin.nextLine();
            hash.add(line);
        }
        System.out.println(hash.size());
        for (String line: hash){
            System.out.println(line);
        }
        stdin.close();
    }
}