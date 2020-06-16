package write;

import java.io.File;

public class Main {
    public static void main(String[] args) {
        File file = new File("汇总.csv");
        if (!file.exists()) {
            Table t = new Table();
            t.create();
        }
        Inquire inquire = new Inquire();
        inquire.start();
    }
}
