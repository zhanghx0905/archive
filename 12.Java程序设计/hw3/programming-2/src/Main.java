import java.io.*;
import java.util.*;

class Main {
    public static void main(String[] args) {
        Scanner stdin = new Scanner(System.in);
        String pattern = stdin.next();
        File[] files = new File("input/test/case").listFiles();
        ArrayList<String> res = new ArrayList<String>();
        for (File f : files) {
            if (f.isDirectory())
                continue;
            String fname = f.getName();
            if (fname.toLowerCase().contains(pattern.toLowerCase()))
                res.add(fname);
        }
        res.sort(String::compareTo);
        res.forEach(it -> System.out.println(it));
    }
}