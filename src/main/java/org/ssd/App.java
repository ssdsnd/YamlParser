package org.ssd;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;
import java.util.Map;
import java.util.Objects;

import org.yaml.snakeyaml.Yaml;

/**
 * Hello world!
 *
 */
public class App 
{
	private static void parserYaml(InputStream inputStream) {
		Yaml yaml =  new Yaml();
		printYamlObj(yaml.load(inputStream));
	}

	static void printYamlObj(Object obj) {
		if (obj instanceof Map) {
			Map<String, Object> map = (Map<String, Object>) obj;
			for (Map.Entry<String, Object> entry : map.entrySet()) {
				System.out.print(entry.getKey() + "={" );
				printYamlObj(entry.getValue());
				System.out.println("}");
			}
			
		} else if (obj instanceof List) {
			List<?> list = (List<?>) obj;
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
	
    public static void main( String[] args ) throws IOException
    {
        System.out.println( "Hello World!" );
        
        File file = new File("sample.yaml");
        InputStream inStream = new FileInputStream(file);
        parserYaml(inStream);
        inStream.close();
    }
}
