import java.util.*;

class Main {
    public static void main(String[] args) {
        Scanner stdin = new Scanner(System.in);
        int size = stdin.nextInt();
        Integer[] mem = new Integer[size];
        Integer a, b, c;
        while (stdin.hasNext()) {
            try {
                String op = stdin.next();
                switch (op) {
                    case "=":
                        a = stdin.nextInt();
                        b = stdin.nextInt();
                        c = stdin.nextInt();
                        for (int i = a; i < b; i++)
                            mem[i] = c;
                        break;
                    case "+":
                        a = stdin.nextInt();
                        b = stdin.nextInt();
                        c = stdin.nextInt();
                        mem[c] = mem[a] + mem[b];
                        break;
                    case "-":
                        a = stdin.nextInt();
                        b = stdin.nextInt();
                        c = stdin.nextInt();
                        mem[c] = mem[a] - mem[b];
                        break;
                    case "*":
                        a = stdin.nextInt();
                        b = stdin.nextInt();
                        c = stdin.nextInt();
                        mem[c] = mem[a] * mem[b];
                        break;
                    case "/":
                        a = stdin.nextInt();
                        b = stdin.nextInt();
                        c = stdin.nextInt();
                        mem[c] = mem[a] / mem[b];
                        break;
                    case "?":
                        a = mem[stdin.nextInt()];
                        if (a == null) {
                            System.out.println("null");
                        } else {
                            System.out.println(a);
                        }
                        break;
                    default:
                        break;
                }
            } catch (NullPointerException e) {
                System.out.println("Null Number");
            } catch (ArithmeticException e) {
                System.out.println("Divided By Zero");
            } catch (ArrayIndexOutOfBoundsException e) {
                System.out.println("Illegal Address");
            }
        }
    }
}