import java.util.Scanner;

class Main {
    static boolean judgeSubMatrix(int[][] matrix, int m, int n) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                boolean[] flag = new boolean[m + 1];
                for (int ii = 0; ii < n; ii++) {
                    for (int jj = 0; jj < n; jj++) {
                        if (flag[matrix[i * n + ii][j * n + jj]]) {
                            return false;
                        }
                        flag[matrix[i * n + ii][j * n + jj]] = true;
                    }
                }
            }
        }
        return true;
    }

    static boolean judgeRow(int[][] matrix, int m) {
        for (int i = 0; i < m; i++) {
            boolean[] flag = new boolean[m + 1];
            for (int j = 0; j < m; j++) {
                if (flag[matrix[i][j]]) {
                    return false;
                }
                flag[matrix[i][j]] = true;
            }
        }
        return true;
    }

    static boolean judgeCol(int[][] matrix, int m) {
        for (int i = 0; i < m; i++) {
            boolean[] flag = new boolean[m + 1];
            for (int j = 0; j < m; j++) {
                if (flag[matrix[j][i]]) {
                    return false;
                }
                flag[matrix[j][i]] = true;
            }
        }
        return true;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt();
        int m = n * n;
        int[][] matrix = new int[m][m];
        for (int i = 0; i < m; i++)
            for (int j = 0; j < m; j++)
                matrix[i][j] = in.nextInt();

        if (judgeSubMatrix(matrix, m, n) && judgeRow(matrix, m) && judgeCol(matrix, m))
            System.out.println("yes");
        else
            System.out.println("no");

    }
}
