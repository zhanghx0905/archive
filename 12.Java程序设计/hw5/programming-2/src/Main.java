import java.net.*;
import java.util.*;
import java.io.*;

class Main {
    private static int substringCount(String father, String son, int count) {
		int fromIndex = 0;
		while (true) {
            int index = father.indexOf(son, fromIndex);
			if (index != -1) {
				count++;
				fromIndex = index + 1;
			} else {
				break;
            }
		}
		return count;
	}
    /* // 蛮力算法
    private static int substringCount(String father, String son, int count) {
        int flen = father.length(), slen = son.length();
        for (int i = 0; i <= flen - slen; i++) {
            if (father.substring(i, i + slen).equals(son)){
                count++;
            }
        }
        return count;
    }*/

    public static void main(String[] args) throws Exception {
        URL.setURLStreamHandlerFactory(TUProxy::new);
        BufferedReader stdin = new BufferedReader(new InputStreamReader(System.in, "utf-8"));
        String url = stdin.readLine();
        LinkedHashMap<String, Integer> hash = new LinkedHashMap<>();
        String inputLine;
        while ((inputLine = stdin.readLine()) != null) {
            hash.put(inputLine, 0);

        }
        stdin.close();

        URLConnection tc = new URL(url).openConnection();
        BufferedReader in = new BufferedReader(new InputStreamReader(tc.getInputStream(), "utf-8"));

        while ((inputLine = in.readLine()) != null) {
            for (Map.Entry<String, Integer> entry : hash.entrySet()) {
                entry.setValue(substringCount(inputLine, entry.getKey(), entry.getValue()));
            }
        }
        in.close();

        List<Map.Entry<String, Integer>> list = new ArrayList<>(hash.entrySet());
        Collections.sort(list, (o1, o2) -> o2.getValue().compareTo(o1.getValue()));
        
        list.forEach(it -> {
            if (it.getValue() > 0)
                System.out.println(it.getKey() + " " + it.getValue());
        });
    }
}