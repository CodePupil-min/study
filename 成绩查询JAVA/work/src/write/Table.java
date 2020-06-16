package write;

import java.io.*;
import java.util.HashMap;
import java.util.Map;

//生成汇总文件
public class Table {
    private HashMap<String, String> num_name = new HashMap<>();//学号_姓名
    private HashMap<String, Double> num_bcc = new HashMap<>();//学号_大基
    private HashMap<String, Double> num_math = new HashMap<>();//学号_高数
    private HashMap<String, Double> num_english = new HashMap<>();//学号_英语
    private HashMap<String, Double> num_elective = new HashMap<>();//学号_选修
    private String path=".\\资源文件";

    //将文件读取到五个hashtable中
    private void readFile(String filename, HashMap table) {
        File file = new File(path,filename);
        BufferedReader br=null;
        try {
             br= new BufferedReader(new InputStreamReader(new FileInputStream(file), "gbk"));
            br.readLine();//读走第一行
            String s = br.readLine();
            String[] s1;
            while (s != null) {
                s1 = s.split("\t");
                if (!num_name.containsKey(s1[1])) {
                    num_name.put(s1[1], s1[0]);
                }
                table.put(s1[1], Double.parseDouble(s1[2]));
                s = br.readLine();
            }
        } catch (FileNotFoundException e) {
            System.out.println("未找到" + filename);
        } catch (IOException e) {
            System.out.println(filename + "文件读取错误");
        }finally {
            try {
                br.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    //将hashtable中数据写入汇总文件
    private void writeFile() {
        File file = new File("汇总.csv");
        BufferedWriter bw=null;
        try {
            bw = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(file), "gbk"));
            bw.write("学号,姓名,大基,英语,高数,选修");
            for (Map.Entry<String, String> entry : num_name.entrySet()) {
                bw.newLine();
                String num=entry.getKey();
                String name=entry.getValue();
                double bcc=num_bcc.get(num)==null?0:num_bcc.get(num);
                double english=num_english.get(num)==null?0:num_english.get(num);
                double math= num_math.get(num)==null?0:num_math.get(num);
                double elective=num_elective.get(num)==null?0:num_elective.get(num);
                bw.write(num+","+name+","+bcc+","+english+","+math+","+elective);
            }
        } catch (IOException e) {
            System.out.println("文件写入错误");
        }finally {
            try {
                bw.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    //创建文件的总过程
    public void create(){
        readFile("大基.txt", num_bcc);
        readFile("高数.txt", num_math);
        readFile("英语.txt", num_english);
        readFile("选修.txt", num_elective);
        writeFile();
        System.out.println("文件汇总完成");
    }
}