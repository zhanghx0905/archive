import java.util.*;
import java.io.*;

class Main {
    public static void main(String[] args) throws Exception {
        Scanner stdin = new Scanner(System.in);
        HashMap<Character, Integer> res = new HashMap<>();
        Scanner filein;
        while (stdin.hasNext()) {
            try {
                String fileName = stdin.next();
                String encoding = stdin.next();
                filein = new Scanner(new File(fileName), encoding);
            } catch (Exception e) {
                continue;
            }
            while (filein.hasNextLine()) {
                String line = filein.nextLine();
                for (Character ch : line.toCharArray()) {
                    Integer val = res.get(ch);
                    res.put(ch, (val == null) ? 1 : (val + 1));
                }
            }
            filein.close();
        }
        stdin.close();
        List<Map.Entry<Character, Integer>> list = new ArrayList<>(res.entrySet());

        Collections.sort(list, (o1, o2) -> {
            if (o2.getValue().equals(o1.getValue()))
                return o1.getKey().compareTo(o2.getKey());
            return o2.getValue().compareTo(o1.getValue());
        });
        int sum = list.stream().mapToInt(it -> it.getValue()).sum();

        PrintWriter pw = new PrintWriter(new File("statistics.txt"), "UTF-8");
        StringBuilder s = new StringBuilder(sum + "\n");
        list.forEach(it -> s.append(it.getKey() + " " + it.getValue() + "\n"));
        pw.print(s);
        pw.close();
    }
}