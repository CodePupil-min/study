package write;

import java.io.*;
import java.util.*;
//命令查询类
public class Inquire {

    private class Student {
        String num = "";
        String name = "";
        double bcc = 0;
        double english = 0;
        double math = 0;
        double elective = 0;
        double avg = 0;

        @Override
        public String toString() {
            return String.format("%-6s %-8.2f%-8.2f%-8.2f%-8.2f", name, bcc, math, english, elective);
        }
    }

    //建立用于查询的studentMap，其中姓名为键（方便查询），Student类为值
    private HashMap<String, Student> studentsMap = new HashMap<>();
    //建立studentList,用于排序等操作
    private List<Student> studentList = null;

    //将汇总文件读到studentMap中
    private void readFile() {
        File file = new File("汇总.csv");
        BufferedReader br = null;
        try {
            br = new BufferedReader(new InputStreamReader(new FileInputStream(file), "gbk"));
            //读走第一行
            br.readLine();
            String s = br.readLine();
            //初始化studentMap
            while (s != null) {
                String[] s1 = s.split(",");
                if (s1.length == 6) {
                    Student stu = new Student();
                    stu.num = s1[0];
                    stu.name = s1[1];
                    stu.bcc = Double.parseDouble(s1[2]);
                    stu.english = Double.parseDouble(s1[3]);
                    stu.math = Double.parseDouble(s1[4]);
                    stu.elective = Double.parseDouble(s1[5]);
                    stu.avg = (stu.bcc + stu.english + stu.math + stu.elective) / 4;
                    studentsMap.put(s1[1], stu);
                    s = br.readLine();
                }
            }
            //初始化studentList
            studentList = new ArrayList<>(studentsMap.values());
        } catch (FileNotFoundException e) {
            System.out.println("汇总文件未找到");
        } catch (IOException e) {
            System.out.println("读写错误");
        } finally {
            try {
                br.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    public void start() {
        readFile();
        Scanner cin = new Scanner(System.in);
        System.out.print("请输入命令：");
        String in = cin.nextLine();
        //截取字符串第一个为命令，并将原字符串传入各个命令对应方法中
        String cmd = in.split(" ")[0];
        while (!cmd.equals("exit")) {
            if (commandMap.containsKey(cmd))
                calculator(commandMap.get(cmd), in);
            else System.out.println("命令未找到");
            System.out.print("请输入命令：");
            in = cin.nextLine();
            cmd = in.split(" ")[0];
        }
        System.out.println("byebye!");
    }

    //存储命令的Map
    private HashMap<String, Command> commandMap = new HashMap<>();

    //加载命令
    {
        commandMap.put("avg", new AvgCommand());
        commandMap.put("get", new GetCommand());
        commandMap.put("sort", new SortCommand());
        commandMap.put("select", new SelectCommand());
    }

    //实现命令的接口
    @FunctionalInterface
    interface Command {
        void execute(String s);
    }

    //执行命令方法,s为输入的字符串
    private void calculator(Command command, String s) {
        command.execute(s);
    }

    //命令的具体实现

    // avg
    // avg 学生名（输出该学生平均分）
    // avg 课程名（输出这门课程平均分）
    class AvgCommand implements Command {
        //计算平均值
        public double getAvg(String name) {
            double sum = 0;//总分
            if (name.equals("大基")) {
                for (Student stu : studentList)
                    sum += stu.bcc;
            } else if (name.equals("英语")) {
                for (Student stu : studentList)
                    sum += stu.english;
            } else if (name.equals("高数")) {
                for (Student stu : studentList)
                    sum += stu.math;
            } else if (name.equals("选修")) {
                for (Student stu : studentList)
                    sum += stu.elective;
            } else {
                return -1;
            }
            return sum / studentList.size();
        }

        @Override
        public void execute(String s) {
            String[] para = s.split(" ");
            if (para.length == 1) {
                System.out.println("参数不足，无法查询");
                return;
            }else if(para[1].equals("help")){
                System.out.println("avg 学生名\navg 课程名");
                return;
            }
            String name = para[1];
            if (studentsMap.containsKey(name)) {
                Student stu = studentsMap.get(name);
                System.out.printf("%s的平均分是：%.2f\n", name, stu.avg);
            } else {
                double avg = getAvg(name);
                if (avg + 1 < 1e-12)
                    System.out.println("你输入的既不是课程名，也不是学生名");
                else
                    System.out.printf("%s的平均分是：%.2f\n", name, avg);
            }
        }
    }

    // get
    // get 学生姓名 课程名 （获取指定学生指定课程的分数）
    // get 课程名 学生姓名 （获取指定课程指定学生的分数）
    class GetCommand implements Command {
        //判断输入的课程是否包含在已存在课程中
        public boolean containCourse(String s) {
            switch (s) {
                case "大基":
                case "高数":
                case "英语":
                case "选修":
                    return true;
                default:
                    return false;
            }
        }

        //获取name的course成绩
        public double getScore(String name, String course) {
            switch (course) {
                case "大基":
                    return studentsMap.get(name).bcc;
                case "高数":
                    return studentsMap.get(name).math;
                case "英语":
                    return studentsMap.get(name).english;
                case "选修":
                    return studentsMap.get(name).elective;
                default:
                    return 0;
            }
        }

        @Override
        public void execute(String s) {
            String[] para = s.split(" ");
            if (para.length < 3) {
                if (para.length == 2 && para[1].equals("help"))
                    System.out.println("get 学生姓名 课程名\nget 课程名 学生姓名");
                else
                    System.out.println("参数不足，无法查询");
                return;
            }
            String name1 = para[1];
            String name2 = para[2];
            //第一个参数为学生姓名时
            if (studentsMap.containsKey(name1)) {
                //第二个参数为课程名
                if (containCourse(name2))
                    System.out.printf("%s的%s成绩是：%.2f\n", name1, name2, getScore(name1, name2));
                else
                    System.out.printf("%s没有%s这门课\n", name1, name2);
            } else if (containCourse(name1)) {
                //第二个参数是学生名
                if (studentsMap.containsKey(name2))
                    System.out.printf("%s的%s成绩是：%.2f\n", name2, name1, getScore(name2, name1));
                else
                    System.out.printf("%s没有%s这门课\n", name2, name1);
            } else {
                System.out.println("没有该学生(课程)");
            }
        }
    }

    // sort
    // sort （不带参数默认按照平均分排序）
    // sort 课程名（带参数则按照课程排序）
    class SortCommand implements Command {
        int x = 1;

        public void show() {
            System.out.println("名次\t\t姓名\t\t大基\t\t高数\t\t英语\t\t选修");
            /*Iterator it = studentList.iterator();
            while (it.hasNext()) {
                System.out.println((x++) + "\t" + it.next());
            }*/
            studentList.stream()
                    .map(s -> (x++) + "\t" + s)
                    .forEach(System.out::println);
            x = 1;
        }

        @Override
        public void execute(String s) {
            String[] para = s.split(" ");
            //无参数时按照平均分排序
            if (para.length == 1) {
                Collections.sort(studentList, (s1, s2) -> (int) (s2.avg - s1.avg));
                show();
                return;
            }else if(para[1].equals("help")){
                System.out.println("sort\nsort 课程名");
                return;
            }
            String courseName = para[1];
            switch (courseName) {
                case "大基":
                    Collections.sort(studentList, (s1, s2) -> (int) (s2.bcc - s1.bcc));
                    break;
                case "高数":
                    Collections.sort(studentList, (s1, s2) -> (int) (s2.math - s1.math));
                    break;
                case "英语":
                    Collections.sort(studentList, (s1, s2) -> (int) (s2.english - s1.english));
                    break;
                case "选修":
                    Collections.sort(studentList, (s1, s2) -> (int) (s2.elective - s1.elective));
                    break;
                default:
                    System.out.println("没有" + courseName + "这门课");
                    return;
            }
            show();
        }
    }

    //select
    //select 课程名 bigth 最低分 lesth 最高分
    class SelectCommand implements Command {
        private void show(String course, double min, double max) {
            List<Student> list = new ArrayList<>();
            for (Student stu : studentList) {
                switch (course) {
                    case "大基":
                        if (stu.bcc >= min && stu.bcc <= max)
                            list.add(stu);
                        break;
                    case "高数":
                        if (stu.math >= min && stu.math <= max)
                            list.add(stu);
                        break;
                    case "英语":
                        if (stu.english >= min && stu.english <= max)
                            list.add(stu);
                        break;
                    case "选修":
                        if (stu.elective >= min && stu.elective <= max)
                            list.add(stu);
                        break;
                    default:
                        System.out.println("不包含此课程");
                        return;
                }
            }
            System.out.println("姓名\t\t大基\t\t高数\t\t英语\t\t选修");
            list.forEach(System.out::println);
            System.out.println(course + "成绩在" + min + "~" + max + "的人数总计：" + list.size());
        }

        @Override
        public void execute(String s) {
            String[] para = s.split(" ");
            if (para.length == 1) {
                System.out.println("参数不足,无法查询");
                return;
            } else if (para[1].equals("help")) {
                System.out.println("select 课程名 bigth 最低分 lesth 最高分");
                return;
            }
            String course = para[1];
            double min = 0;
            double max = 100;
            boolean isSet = false;
            int i = 0;
            try {
                for (; i < para.length; i++) {
                    if (para[i].equals("bigth")) {
                        min = Double.parseDouble(para[i + 1]);
                        isSet = true;
                    } else if (para[i].equals("lesth")) {
                        max = Double.parseDouble(para[i + 1]);
                        isSet = true;
                    } else continue;
                }
            } catch (IndexOutOfBoundsException e) {
                System.out.println("“" + para[i] + "”缺少参数");
                return;
            } catch (NumberFormatException e) {
                System.out.println("”" + para[i] + "“后参数格式错误");
                return;
            }
            if (isSet) {
                show(course, min, max);
            } else {
                System.out.println("参数有误");
            }
        }
    }
}
