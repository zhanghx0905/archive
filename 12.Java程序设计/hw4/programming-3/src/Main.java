import java.util.*;

class Graph {
    int size;
    ArrayList<LinkedList<Integer>> adjacencyList;

    Graph(int size) {
        this.size = size;
        this.adjacencyList = new ArrayList<>(size);
        for (int i = 0; i < size; i++) {
            adjacencyList.add(new LinkedList<>());
        }
    }

    public Boolean isConnected() { // BFS
        boolean[] visited = new boolean[size];
        ArrayDeque<Integer> q = new ArrayDeque<>();
        Integer src = 0, cnt = 0;
        q.push(src);
        while (!q.isEmpty()) {
            visited[src] = true;
            cnt++;
            for (Integer neighbor : adjacencyList.get(src)) {
                if (!visited[neighbor])
                    q.push(neighbor);
            }
            src = q.pop();
        }
        return cnt == size;
    }

    public void addEdge(int src, int dst) {
        adjacencyList.get(src).add(dst);
        adjacencyList.get(dst).add(src);
    }
}

class Main {
    public static void main(String[] args) {
        Scanner scn = new Scanner(System.in);
        int n = scn.nextInt();
        Graph g = new Graph(n);
        for (int i = 0; i < n - 1; i++) {
            int l = scn.nextInt();
            int r = scn.nextInt();
            g.addEdge(l - 1, r - 1);
        }
        scn.close();
        System.out.println(g.isConnected() ? "YES" : "NO");
    }
}