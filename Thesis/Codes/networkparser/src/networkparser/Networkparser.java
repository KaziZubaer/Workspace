/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package networkparser;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import static java.lang.Math.cos;
import static java.lang.Math.sin;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.Vector;

/**
 *
 * @author bejon
 */
public class Networkparser {

    /**
     * @param args the command line arguments
     */
    static int R = 6389;
    Connection conn = null;
    public static int return_ways_number(Connection conn)
    {
        try
        {
            String sql = "SELECT count(id)  from ways_tags where ways_tags.k='highway' ;";
            Statement st = conn.createStatement();
            ResultSet rs = st.executeQuery(sql);
            while(rs.next())
            {
                return rs.getInt(1);
            }
            return -1;
        }
        catch(Exception e)
        {
            System.out.println(e);
            return -1;
        }
    }
    
    public static int findstartnode(String str)
    {
        int count=0;
        int start=0;
        for(int i=0;i<str.length();i++)
        {
            if(str.charAt(i)==' ')
                count++;
            if(start==0 && count ==2)
                return i;
        }
        return -1;
    }
    
    public static int findendnode(String str)
    {
        int count=0;
        int end = 0;
        for(int i=0;i<str.length();i++)
        {
            if(str.charAt(i)==' ')
                count++;
            if(end==0 && count==3)
                return i;
        }
        return -1;
    }
    
    
    public static void main(String[] args) {
        // TODO code application logic here
        try
        {
            Class.forName("org.sqlite.JDBC");
            Connection conn = DriverManager.getConnection("jdbc:sqlite:map.osm.db");
            System.out.println("connection established");
            String sql = "select * from nodes;";
            String sql1 = "SELECT ways_nodes.way_id,nodes.id,nodes.lat,nodes.lon,ways_nodes.way_pos  from ways_tags,ways_nodes,nodes where ways_tags.k='highway' and ways_tags.id = ways_nodes.way_id and nodes.id = ways_nodes.node_id;";
            Statement st = conn.createStatement();
            ResultSet re = st.executeQuery(sql);
            FileWriter fw = new FileWriter("nodes.txt");
            PrintWriter pw = new PrintWriter(fw);
            while(re.next())
            {
                String nodeid = re.getString(1);
                float lat = re.getFloat("lat");
                float lon = re.getFloat("lon");
                pw.println(""+nodeid+" "+lat+" "+lon);
                //pw.print("bejon");
                
            }
            
            pw.close();
            
            Statement st1 = conn.createStatement();
            ResultSet re1 = st.executeQuery(sql1);
            FileWriter fw1 = new FileWriter("ways_with_nodes_detail.txt");
            PrintWriter pw1 = new PrintWriter(fw1);
            int nodes =0;
            float max = 900000,x_max = 000,y_max=00,x_min=900000,y_min = 900000;
            while(re1.next())
            {
                String wayid = re1.getString(1);
                String nodeid = re1.getString(2);
                String waypos = re1.getString(5);
                float lat = re1.getFloat(3);
                float lon = re1.getFloat(4);
                float x = (float)(cos(lat*Math.PI/180) * cos(lon*Math.PI/180) * R);
                if(x<0)x=-x;
                if(x>x_max)
                    x_max = x;
                if(x<x_min)
                    x_min = x;
                
                float y = (float)(R * cos(lat*Math.PI/180) * sin(lon*Math.PI/180));
                if(y<0)y=-y;
                if(y>y_max)
                    y_max =y;
                if(y<y_min)
                    y_min = y;
                
                
                pw1.println(""+wayid+" "+nodeid+" "+waypos+" "+lat+" "+lon+" "+x+" "+y);
                nodes++;
                //pw.print("bejon");
                
            }
            
            pw1.close();
            
            Statement st2 = conn.createStatement();
            ResultSet re2 = st.executeQuery(sql1);
            FileWriter fw2 = new FileWriter("myNetwork.txt");
            PrintWriter pw2 = new PrintWriter(fw2);
            pw2.println(nodes);
            for(int i =0;i<nodes;i++)
                pw2.println(i+" 0 node"+i);
            //pw2.println(nodes-return_ways_number(conn));
            int count1 =0,c=0,count=0;
            int Start=0,end = 0;
            float []arr = new float[2];
            StringBuilder print=new StringBuilder("");
            Vector<Vector<String>> vec = new Vector<>();
            Vector <String> str = new Vector<>();
            while(re2.next())
            {
                
                    //String wayid = re2.getString(1);
                    //String nodeid = re2.getString(2);
                    String waypos = re2.getString(5);
                    if(waypos.equals("0"))
                    {
                        //pw2.print(print);
                        //print = new StringBuilder("");
                        //count=0;
                        if(str.size()!=0)
                        {
                            pw2.println(count+" 0 "+Start+" "+(count1-1)+" "+str.size());
                            for(int i = 0;i<str.size();i++)
                            {
                                pw2.println(i+" "+str.get(i)+" 7");
                            }
                            /*str.add(0,new String(count+" 0 "+Start+" "+(count1-1)+" "+(str.size()+1)));
                            vec.add(str);*/
                            Start = count1;
                            str = new Vector<>();
                            count++;
                        }
                        float lat = re2.getFloat(3);
                        float lon = re2.getFloat(4);
                        float x = (float)(cos(lat*Math.PI/180) * cos(lon*Math.PI/180) * R);
                        if(x<0)x=-x;
                        x = (x - x_min)*1250/(x_max-x_min)+20;
                        float y = (float)(R * cos(lat*Math.PI/180) * sin(lon*Math.PI/180));
                        if(y<0)y=-y;
                        y = (y-y_min)*600/(y_max-y_min)+20;
                        arr[0]= x;
                        arr[1]= y;
                        //print = print+""+count1+" 0 "+
                        //pw2.print();
                        
                        
                    
                    }
                    else
                    {
                        float lat = re2.getFloat(3);
                        float lon = re2.getFloat(4);
                        float x = (float)(cos(lat*Math.PI/180) * cos(lon*Math.PI/180) * R);
                        if(x<0)x=-x;
                        x = (x - x_min)*1250/(x_max-x_min)+20;
                        float y = (float)(R * cos(lat*Math.PI/180) * sin(lon*Math.PI/180));
                        if(y<0)y=-y;
                        y = (y-y_min)*600/(y_max-y_min)+20;
                        //pw2.println(c+" 0 "+(count1-1)+" "+count1+" 1");
                        //pw2.println("0 "+(int)arr[0]+" "+(int)arr[1]+" "+(int)x+" "+(int)y+" 20 7");
                        //System.out.println(c+arr[0]+" "+(int)arr[1]+" "+x+" "+(int)y+" 20");
                        str.add(new String((int)arr[0]+" "+(int)arr[1]+" "+(int)x+" "+(int)y+" 4"));
                        //str.add((c+1), new String(c+(int)arr[0]+" "+(int)arr[1]+" "+(int)x+" "+(int)y+" 20"));
                        arr[0]=x;
                        arr[1]=y;
                        c++;
                    }
              count1++;
                //pw2.println(""+wayid+" "+nodeid+" "+waypos+" "+lat+" "+lon+" "+x+" "+y);
                //nodes++;
                //pw.print("bejon");
                
            }
            pw2.println(count+" 0 "+Start+" "+(count1-1)+" "+str.size());
            for(int i = 0;i<str.size();i++)
                {
                    pw2.println(i+" "+str.get(i)+" 7");
                }
           /* pw2.println(vec.size());
            for(int i =0;i<vec.size();i++)
                for(int j=0;j<vec.get(i).size()-1;j++)
                    pw2.println(j+" "+vec.get(i).get(j+1)+" 7");*/
            pw2.close();
            //return conn;
            BufferedReader br = new BufferedReader(new FileReader("myNetwork.txt"));
            PrintWriter p = new PrintWriter("finalNetwork.txt");
            String read;
            Vector<String> node = new Vector<>();
            Vector<String> links = new Vector<>();
            Vector<String> linksegment = new Vector<>();
            int counthere = 0,countlink=0,countsegment = 0;
            int value = 0;
            int last=-1;
            while((read = br.readLine())!=null)
            {
                if(value==0)
                {
                    value = Integer.parseInt(read);
                    //System.out.println(value);
                }
                counthere++;
                if(counthere>value+1)
                {
                    if(countlink==0 && countsegment==0)
                    {
                        links.add(read);
                        countsegment =Integer.parseInt(read.substring(read.lastIndexOf(" ")+1));
                        last = counthere+countsegment;
                        node.add(read.substring(findstartnode(read)+1,findendnode(read)));
                        node.add(read.substring(findendnode(read)+1, (read.lastIndexOf(" "))));
                    }
                    else if(counthere == last+1)
                    {
                        links.add(read);
                        countsegment =Integer.parseInt(read.substring(read.lastIndexOf(" ")+1));
                        last = counthere+countsegment;
                        node.add(read.substring(findstartnode(read)+1,findendnode(read)));
                        node.add(read.substring(findendnode(read)+1, (read.lastIndexOf(" "))));
                    }
                    linksegment.add(read);
                }
            }
            br.close();
            
            for(int i =0;i<node.size();i++)
                System.out.println(node.get(i));
            for(int i=0;i<links.size();i++)
            {
                System.out.println(links.get(i));
            }
            p.println(node.size());
            for(int i =0;i<node.size();i++)
                p.println(node.get(i)+" 0 node"+node.get(i));
            p.println(links.size());
            for(int i =0;i< linksegment.size();i++)
            {
                p.println(linksegment.get(i));
            }
            p.close();
        }
        catch(Exception e)
        {
            System.out.println("connection failed"+e);
        }
        
    }
    
}
