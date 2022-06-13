import java.util.HashMap;
import java.util.Scanner;
import java.util.Map;

class Staff extends BaseStaff {
    private final int number;
    public final String info;

    enum Type {
        Teacher, Student
    }

    private final Type type;

    Staff(final String info) {
        this.info = info;
        String[] infos = info.split(" ");
        if (infos[0].charAt(0) == 'T')
            this.type = Type.Teacher;
        else
            this.type = Type.Student;
        this.number = Integer.parseInt(infos[1]);
    }

    @Override
    public String getType() {
        if (type == Type.Teacher)
            return "Teacher";
        else
            return "Student";
    }

    @Override
    public int getNumber() {
        return number;
    }
}

class Main {

    public static void main(String[] args) {

        Scanner in = new Scanner(System.in);
        int n, m;
        n = in.nextInt();
        m = in.nextInt();
        in.nextLine();
        HashMap<Staff, Integer> hash = new HashMap<Staff, Integer>();
        for (int i = 0; i < n; i++) {
            String info = in.nextLine();
            Staff staff = new Staff(info);
            hash.put(staff, 0);
        }
        for (int i = 0; i < m; i++) {
            String info = in.nextLine();
            Staff staff = new Staff(info);
            hash.replace(staff, hash.get(staff) + 1);
        }
        int punch_max = 0;
        String info_max = "";

        for (Map.Entry<Staff, Integer> iter : hash.entrySet()) {
            int tmp = iter.getValue();
            if (tmp > punch_max) {
                punch_max = tmp;
                info_max = iter.getKey().info;
            }
        }
        System.out.println(info_max);
        in.close();
    }
}