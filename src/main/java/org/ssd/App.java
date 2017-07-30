package org.ssd;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;

import org.yaml.snakeyaml.Yaml;

/**
 * Hello world!
 *
 */

class Struct {
    public static class Output {
        public Map<String, String> rdRows;
        public Map<String, String> sdRows;
        public String rc;
        public String rt;
    }
    public String name;
    public String subName;
    public Map<String, String>  inputRows = new LinkedHashMap<String, String>();
    public boolean flag;
    public List<Output>         outputRows = new ArrayList<Output>();
}

public class App 
{
    private static Object parserYaml(InputStream inputStream) {
        Yaml yaml =  new Yaml();
        return yaml.load(inputStream);
    }

    private static void printYamlObj(Object obj) {
        if (obj instanceof Map) {
            Map<String, Object> map = getMap(obj);
            for (Map.Entry<String, Object> entry : map.entrySet()) {
                System.out.print(entry.getKey() + "={" );
                printYamlObj(entry.getValue());
                System.out.println("}");
            }
        } else if (obj instanceof List) {
            List<?> list = getList(obj);
            System.out.print("[");
            for (Object o : list) {
                printYamlObj(o);
                System.out.print(", ");
            }
            System.out.println("]");
        } else {
            System.out.print(Objects.toString(obj));
        }
    }

    @SuppressWarnings("unchecked")
    private static Map<String, Object> getMap(Object obj) {
        return (Map<String, Object>) obj;
    }

    private static List<?> getList(Object obj) {
        return (List<?>) obj;
    }

    private static void printStruct(Object obj) {
        Map<String, Object> map = getMap(obj);
        List<?> list = getList(getMap(getMap(map.get("Main")).get("subtags")).get("Tag1"));
        printYamlObj(list);
        System.out.println("Tag1 list size: " + list.size());
        parseStruct1(list);

        list = getList(getMap(getMap(map.get("Main")).get("subtags")).get("Tag2"));
        printYamlObj(list);
        System.out.println("Tag2 list size: " + list.size());
        parseStruct2(list);
    }

    private static void parseStruct1(List<?> obj) {
        List<?> header = (List<?>) obj.get(0);
        System.out.println("header: " + header);
    }

    private static void parseStruct2(List<?> obj) {
        
    }

    public static void main( String[] args ) throws IOException {
        System.out.println( "Hello World!" );

        File file = new File("sample.yaml");
        InputStream inStream = new FileInputStream(file);
        //printYamlObj(parserYaml(inStream));
        printStruct(parserYaml(inStream));
        inStream.close();
    }
}
