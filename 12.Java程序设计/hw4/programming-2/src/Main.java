import java.util.*;

class Main {
    public static void main(String[] args) {
        TreeMap<Integer, Integer> tree = new TreeMap<>();
        Scanner stdin = new Scanner(System.in);
        int n = stdin.nextInt();
        long sum = 0;
        boolean bankrupt = false;
        for (int i = 0; i < n; i++) {
            int op = stdin.nextInt(), val = stdin.nextInt();
            switch (op) {
                case 1:
                    Integer tmp = tree.get(val);
                    tree.put(val, (tmp == null) ? 1 : (tmp + 1));
                    sum += val;
                    break;
                case 2:
                    Map.Entry<Integer, Integer> floor = tree.floorEntry(val);
                    if (floor != null) {
                        if (floor.getValue() <= 1)
                            tree.remove(floor.getKey());
                        else
                            floor.setValue(floor.getValue() - 1);
                        sum -= floor.getKey();

                    } else {
                        bankrupt = true;
                    }
                    break;
                case 3:
                    Map.Entry<Integer, Integer> ceiling = tree.ceilingEntry(val);
                    if (ceiling != null) {
                        if (ceiling.getValue() <= 1)
                            tree.remove(ceiling.getKey());
                        else
                            ceiling.setValue(ceiling.getValue() - 1);
                        sum -= ceiling.getKey();
                    } else {
                        bankrupt = true;
                    }
                    break;
            }
            if (bankrupt) {
                System.out.println(-1);
                System.exit(0);
            }
        }
        stdin.close();
        System.out.println(sum);
    }
}