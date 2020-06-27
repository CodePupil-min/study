//爬取bing首页图片
//2020.6.27
import java.io.*;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CaptureBingImage {
    public static void main(String[] args) {
        String url=getUrl();
        String filename=getName();
        create(url,filename);
    }
    //用日期对图片进行命名
    public static String getName(){
        Date date=new Date();
        SimpleDateFormat s=new SimpleDateFormat("yyyy-MM-dd");
        return s.format(date);
    }
    //创建图片
    public static void create(String url,String filename){
        File img=new File(filename+".jpg");
        try {
            img.createNewFile();
            InputStream in=new URL(url).openStream();
            FileOutputStream out=new FileOutputStream(img);
            int s=in.read();
            while(s!=-1){
                out.write(s);
                s=in.read();
            }
            in.close();
            out.close();
            System.out.println("ok");
        } catch (IOException e) {
            e.printStackTrace();
        }
    } 
    //获取图片链接
    public static String getUrl(){
        String url= null;
        String content= null;
        String rurl="\\s*(href|src)\\s*=\\s*(\"([^\"]*\")|(\'[^\']*\')|([^\'\">\\s]+))";
        try {
            BufferedReader br=new BufferedReader(new InputStreamReader(new URL("https://cn.bing.com/").openStream()));
            //图片链接在第三行
            br.readLine();
            br.readLine();
            content=br.readLine();
            Pattern pattern = Pattern.compile(rurl,
                    Pattern.CASE_INSENSITIVE);
            Matcher matcher = pattern.matcher( content );
            StringBuffer buffer = new StringBuffer();
            if(matcher.find()) {
                buffer.append("https://cn.bing.com");
                buffer.append(matcher.group(2).replaceAll("\"",""));
            }
            url=buffer.toString();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return url;
    }
}
