/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package shapeparser;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author kazi
 */
public class ShapeParser {

    /**
     * @param args the command line arguments
     */
//    final static int WORKINGROAD = 2893;
//    final static int WORKINGROAD = 2916;
//    final static int WORKINGROAD = 1340;
//    final static int WORKINGROAD = 3656;
//    final static int WORKINGROAD = 2904;
    final static int WORKINGROAD = 2902;    
    final static int WORKINGLINK = 1;
    final static int RESOLUTION = 2;
    final static int ANGRESOLUTION = 3;
    final static double TESTX = 540132;
    final static double TESTY = 627469;
    final static double MULTI = 1.4;
    final static int QUANTILE = 75;
    final static double LEASTDIST = 4;
    final static double DIVIDERWIDTH = 2;
    final static double CORNERAREAFACTOR = 12;
    final static boolean TESTING = false;
    final static double PI = Math.acos(-1);
    final static double EPS = 1e-10;
    
    /*
    * Checks if any point in the range of x+-RESOLUTION/2, y+-RESOLUTION/2 is 
    * int the HashSet
    */
    static boolean DoesContain(double x, double y, HashSet<Point> pts) {
        double lowX = Math.floor(x), lowY = Math.floor(y);
        int i,j;
        for(i=-RESOLUTION/2; i<=(RESOLUTION+1)/2; i++) {
            for(j=-RESOLUTION/2; j<=(RESOLUTION+1)/2; j++) {
                if(pts.contains(new Point(lowX+i,lowY+j))) {
                    return true;
                }
            }
        }
        return false;
    }
    
    /*
    * Checks if this polygon is a divider. If the average width of the polygon
    * is less than DIVIDERWIDTH will will identify it as a divider
    */
    static boolean Removable (ArrayList<Point>poly) {
        int i, j, n = poly.size();
        double mostDist = 0,area = 0;
        for(i=0; i<poly.size(); i++) {
            for(j=i+1; j<poly.size(); j++) {
                mostDist = Math.max(mostDist, poly.get(i).distFrom(poly.get(j)));
            }
            area += poly.get(i).x*poly.get((i+1)%n).y - poly.get(i).y*poly.get((i+1)%n).x;
        }
        area = Math.abs(area);
        area *= 0.5;
        return area/mostDist <= DIVIDERWIDTH;
    }
    
    /*
    * Finds if lines parallel
    */
    static boolean LinesParallel(Point a, Point b, Point c, Point d) {
        return Math.abs((b.minus(a)).cross(c.minus(d))) < EPS;
    }
    
    /*
    * Finds if lines collinear
    */
    static boolean LinesCollinear(Point a, Point b, Point c, Point d) {
        return Math.abs((b.minus(a)).cross(c.minus(d))) < EPS
                && Math.abs((a.minus(b)).cross(a.minus(c))) < EPS
                && Math.abs((c.minus(d)).cross(c.minus(a))) < EPS;
    }
    
    /*
    * Finds if Segments intersect
    */
    static boolean SegmentsIntersect(Point a, Point b, Point c, Point d) {
        if(LinesCollinear(a,b,c,d)) {
            if(a.distFrom(c) < EPS || a.distFrom(d) < EPS ||
                b.distFrom(c) < EPS || b.distFrom(d) < EPS) 
                return true;
            return !((c.minus(a)).dot(c.minus(b)) > 0 
                    && (d.minus(a)).dot(d.minus(b)) > 0
                    && (c.minus(b)).dot(d.minus(b)) > 0);
        }
        if((d.minus(a)).cross(b.minus(a))*(c.minus(a)).cross(b.minus(a)) > 0) return false;
        return (a.minus(c)).cross(d.minus(c))*(b.minus(c)).cross(d.minus(c)) <= 0;
    }
    
    /*
    * Finds intersection of two lines
    */
    static Point ComputeLineIntersection(Point a, Point b, Point c, Point d) {
        b = b.minus(a); 
        d = c.minus(d); 
        c = c.minus(a);
        assert(b.magnitude() > EPS && d.magnitude() > EPS);
        Point ret = a.add(b.mult(c.cross(d)/b.cross(d)));
        return ret;
    }
    
    public static void main(String[] args) throws IOException {
        // TODO code application logic here
        
//        int WORKINGROAD = Integer.parseInt(args[0]);
        String fileNameIn = "Nodes.csv", line;
        String [] vals;
        int i, j, n, k, m, roadId, cnt, angIdx, cornerIdx, tempIdx;
        int[] idx = new int[5];
        int[] angCnt = new int[2];
        int[][] pointCnt;
        double[][] roadWidth;
        int[][] dirArray = new int[2][4];
        double id, x, y, endX, endY, miniX, maxiX, miniY, maxiY, ang, 
                threshDist, r, tempWidth, biasX, biasY, scaling, polySizeSum,
                timeTracker;
        double[] dists = new double[360/ANGRESOLUTION];
        double[] avgAng = new double[2];
        Point p, pp, pp1, pp2;
        Road link;
        ArrayList<Point> pts = new ArrayList<>(), poly, temp;
        ArrayList< ArrayList<Point> > polys = new ArrayList<>();
        ArrayList< Corner > corners = new ArrayList<>();
        ArrayList< Road > links = new ArrayList<>();
        HashSet<Point> pixels = new HashSet<>();
        HashSet<Point> cornerRaw = new HashSet<>();
        HashSet<Point> cornerToDel = new HashSet<>();
        HashSet<Point> processedCorner = new HashSet<>();
        HashSet<Point> processedPoint = new HashSet<>();
        Map cornersMap = new HashMap();
        Map roadCornerMap = new HashMap();
        Map pointWidth = new HashMap();
        boolean angExist[] = new boolean[360];
        boolean prev;
        FileWriter fileWriter;
        BufferedWriter bufferedWriter;
        
        /***Get border points from the file***/
        try {
            
            FileReader fileReader = new FileReader(fileNameIn);
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            line = bufferedReader.readLine();
            while((line = bufferedReader.readLine()) != null) {
                vals = line.split(",");
                id = Double.parseDouble(vals[0]);
                roadId = (int) Math.round(id);
                if(roadId != WORKINGROAD) {
                    continue;
                }
                x = Double.parseDouble(vals[3]);
                y = Double.parseDouble(vals[4]);
                pts.add(new Point(x,y));
            }
            bufferedReader.close();
            
        } catch (FileNotFoundException ex) {
            Logger.getLogger(ShapeParser.class.getName()).log(Level.SEVERE, null, ex);
        }
        miniX = Float.MAX_VALUE;
        maxiX = 0;
        miniY = Float.MAX_VALUE;
        maxiY = 0;
        j=0;
        
        timeTracker = System.currentTimeMillis();
        /***Get the polygons***/
        while(pts.size() > 0) {
            endX = pts.get(0).x;
            endY = pts.get(0).y;
            pts.remove(0);
            
            /***Adding points in 3 unit intervals***/
            for(i=0 ; pts.get(i).x != endX || pts.get(i).y != endY ; i++) {
                if(pts.get(i).distFrom(pts.get(i+1))>3) {
                    p = pts.get(i).add((pts.get(i+1).minus(pts.get(i))).scaledP(3));
                    pts.add(i+1,p);
                }
            }
            while(true) {
                if(pts.get(i).distFrom(pts.get(0))>3) {
                    p = pts.get(i).add((pts.get(0).minus(pts.get(i))).scaledP(3));
                    pts.add(i+1,p);
                    i++;
                }
                else {
                    break;
                }
            }
            endX = pts.get(i).x;
            endY = pts.get(i).y;
            
            /***Removing too close points***/
            for(i=0 ; pts.get(i+1).x != endX || pts.get(i+1).y != endY ; i++) {
                if(pts.get(i).distFrom(pts.get(i+1))<2) {
                    pts.remove(i+1);
                    i--;
                }
            }
            
            /***Creating the polygon***/
            n = i+2;
            poly = new ArrayList<>();
            for(i=0 ; i<n ; i++) {
                miniX = Math.min(miniX,pts.get(i).x);
                maxiX = Math.max(maxiX,pts.get(i).x);
                miniY = Math.min(miniY,pts.get(i).y);
                maxiY = Math.max(maxiY,pts.get(i).y);
                poly.add(pts.get(i));
            }
            polys.add(poly);
            pts.subList(0, n).clear();
            j++;
        }
        
        polySizeSum = 0;
        /***Eliminate dividers***/
        if(polys.size() > 1) {
            for(i=0; i<polys.size(); i++) {
                if(Removable(polys.get(i))) {
                    polys.remove(i);
                    i--;
                }
                else {
                    polySizeSum += polys.get(i).size();
                }
            }
        }
        
        
        miniX = Math.floor(miniX);
        maxiX = Math.ceil(maxiX);
        miniY = Math.floor(miniY);
        maxiY = Math.ceil(maxiY);
        System.out.println("Checkpoint 1 "+(maxiX-miniX)+" "+(maxiY-miniY)+" "+polySizeSum+" "+j);
        System.out.println("Time passed: "+(System.currentTimeMillis() - timeTracker));
        timeTracker = System.currentTimeMillis();
        fileWriter = new FileWriter("AllPoints"+WORKINGROAD+".csv");
        bufferedWriter = new BufferedWriter(fileWriter);
        line =  "ID,X,Y\n";
        bufferedWriter.write(line);
        
        /***Get pixel points. The Logic is: if any point around the current
         point is inside a polygon then it's inside that polygon***/
        for(x=miniX; x<=maxiX; x+=RESOLUTION) {
            for(y=miniY; y<=maxiY; y+=RESOLUTION) {
                for(j=0; j<5; j++) {
                    idx[j]=0;
                }
                p = new Point(x,y);
                for(j=0; j<polys.size(); j++) {
                    if(p.isInside(polys.get(j))) {
                        idx[0]++;
                    }
                }
                for(j=0; j<polys.size(); j++) {
                    if((p.add(new Point(-1,0))).isInside(polys.get(j))) {
                        idx[1]++;
                    }
                }
                for(j=0; j<polys.size(); j++) {
                    if((p.add(new Point(1,0))).isInside(polys.get(j))) {
                        idx[2]++;
                    }
                }
                for(j=0; j<polys.size(); j++) {
                    if((p.add(new Point(0,-1))).isInside(polys.get(j))) {
                        idx[3]++;
                    }
                }
                for(j=0; j<polys.size(); j++) {
                    if((p.add(new Point(0,1))).isInside(polys.get(j))) {
                        idx[4]++;
                    }
                }
                for(j=0; j<5; j++) {
                    if(idx[j]%2 == 1) {
                        pixels.add(p);
                        line =  "0,"+x+","+y+"\n";
                        bufferedWriter.write(line);
                        break;
                    }
                }
            }
        }
             
        bufferedWriter.close();
        System.out.println("Checkpoint 2 "+pixels.size());
        System.out.println("Time passed: "+(System.currentTimeMillis() - timeTracker));
        timeTracker = System.currentTimeMillis();
        j = 0;
        fileWriter = new FileWriter("CornerPointsRaw"+WORKINGROAD+".csv");
        bufferedWriter = new BufferedWriter(fileWriter);
        line =  "ID,X,Y\n";
        bufferedWriter.write(line);
        
        dirArray[0] = new int[] {-RESOLUTION, RESOLUTION, 0, 0};
        dirArray[1] = new int[] {0, 0, -RESOLUTION, RESOLUTION};
        
        /***Detect raw corner points***/
        for(Point ip : pixels) {
//            if(j%100==0) System.out.println(j);
            j++;
            if(TESTING && (ip.x != TESTX || ip.y != TESTY))
                continue;
            
            /***Check distance around 360 angle***/
            for(i=0; i<360 ;i+=ANGRESOLUTION) {
                ang = (i*PI)/180;
                for(r=LEASTDIST; ; r++) {
                    x = r*Math.cos(ang);
                    y = r*Math.sin(ang);   
                    if(!DoesContain(ip.x+x, ip.y+y, pixels) || r>100) {
                        dists[i/ANGRESOLUTION] = r-1;
                        if(TESTING && Math.abs(ip.x-TESTX)<EPS && Math.abs(ip.y-TESTY)<EPS)
                            System.out.println("Dist "+i+" "+(r-1));
                        break;
                    }
                }
            }
            
            /***TempWidth will be the minimum distance around the point***/
            tempWidth = 10000;
            for(i=0; i<360 ;i+=ANGRESOLUTION) {
                tempWidth = Math.min(tempWidth, dists[i/ANGRESOLUTION]+dists[((i+180)%360)/ANGRESOLUTION]);
            }
            pointWidth.put(new Point(ip), tempWidth);
            Arrays.sort(dists);
            if(TESTING && Math.abs(ip.x-TESTX)<EPS && Math.abs(ip.y-TESTY)<EPS)
                for(i=0; i<360; i+=ANGRESOLUTION)
                    System.out.println(Math.floor((i*100.0)/360)+" "+dists[i/ANGRESOLUTION]);
            
            /***Calculating the threshold distance***/
            threshDist = dists[(360*QUANTILE)/(100*ANGRESOLUTION)]*MULTI;
            if(TESTING && Math.abs(ip.x-TESTX)<EPS && Math.abs(ip.y-TESTY)<EPS)
                System.out.println("Threshold "+threshDist);
            
            /***Check which angles exists***/
            for(i=0; i<360 ;i+=ANGRESOLUTION) {
                ang = (i*PI)/180;
                for(r=LEASTDIST; r<=threshDist; r++) {
                    x = r*Math.cos(ang);
                    y = r*Math.sin(ang);
                    if(!DoesContain(ip.x+x, ip.y+y, pixels)) {
                        break;
                    }
                }
                angExist[i] = r>threshDist;
            }
            
            /***Adding missed angles***/
            for(i=ANGRESOLUTION; i<360-ANGRESOLUTION; i+=ANGRESOLUTION) {
                if(angExist[i-ANGRESOLUTION]&&angExist[i+ANGRESOLUTION]) {
                    angExist[i] = true;
                }
            }
            if(angExist[360-ANGRESOLUTION]&&angExist[ANGRESOLUTION]) {
                angExist[0] = true;
            }
            if(angExist[360-2*ANGRESOLUTION]&&angExist[0]) {
                angExist[360-ANGRESOLUTION] = true;
            }
            
            /***Count continuous angle portions***/
            prev = false;
            cnt = 0;
            angCnt[0] = angCnt[1] = 0;
            avgAng[0] = avgAng[1] = 0;
            angIdx = 0;
            for(i=0; i<360; i+=ANGRESOLUTION) {
                if(angExist[i]&&!prev) {
                    angIdx = (angIdx+1)%2;
                    cnt++;
                }
                if(angExist[i]) {
                    if(TESTING && Math.abs(ip.x-TESTX)<EPS && Math.abs(ip.y-TESTY)<EPS)
                        System.out.println("Ang exists "+i);
                    avgAng[angIdx] += i;
                    angCnt[angIdx] ++;
                }
                prev = angExist[i];
            }
            if(angExist[360-ANGRESOLUTION]&&angExist[0]) {
                cnt--;
            }
            if(TESTING && Math.abs(ip.x-TESTX)<EPS && Math.abs(ip.y-TESTY)<EPS) 
                System.out.println("CNT "+cnt);
            if(cnt!=2) {
                cornerRaw.add(new Point(ip.x,ip.y));
                line =  "0,"+ip.x+","+ip.y+"\n";
                bufferedWriter.write(line);
                line = "For road "+WORKINGROAD+", point "+ip.x+","+ip.y+" has count "+cnt+"\n";
                Files.write(Paths.get("log.txt"), line.getBytes(), StandardOpenOption.APPEND);
            }
        }
        /***Expanding corner points***/
        for(Point ip : pixels) {
            if(cornerRaw.contains(ip)) {
                continue;
            }
            cnt = 0;
            for(i=0; i<4; i++) {
                if(cornerRaw.contains(ip.add(new Point(dirArray[0][i], dirArray[1][i])))) {
                    cnt++;
                }
            }
            if(cnt>1) {
                cornerRaw.add(new Point(ip.x,ip.y));
                line =  "0,"+ip.x+","+ip.y+"\n";
                bufferedWriter.write(line);
            }
        }
        bufferedWriter.close();
        System.out.println("Checkpoint 3 "+cornerRaw.size());
        System.out.println("Time passed: "+(System.currentTimeMillis() - timeTracker));
        timeTracker = System.currentTimeMillis();
        fileWriter = new FileWriter("CornerPoints"+WORKINGROAD+".csv");
        bufferedWriter = new BufferedWriter(fileWriter);
        line =  "ID,X,Y\n";
        bufferedWriter.write(line);
        j = 0;
        cornerIdx = 0;
        
        /***Detect final corner points. The average ones***/
        for(Point ip : cornerRaw) {
//            if(j%100==0) System.out.println(j);
            j++;
            if(processedCorner.contains(ip)) {
                continue;
            }
            x = 0;
            y = 0;
            cnt = 0;
            temp = new ArrayList<>();
            temp.add(new Point(ip.x,ip.y));
            processedCorner.add(new Point(ip.x,ip.y));
            while(temp.size()>0) {
                p = temp.get(0);
                x += p.x;
                y += p.y;
                cnt++;
                cornersMap.put(new Point(p.x, p.y), cornerIdx);
                for(i=0; i<4; i++) {
                    pp = p.add(new Point(dirArray[0][i], dirArray[1][i]));
                    if(cornerRaw.contains(pp) 
                            && !processedCorner.contains(pp)) {
                        temp.add(pp);
                        processedCorner.add(pp);
                    }
                }
                temp.remove(0);
            }
            if(cnt>1) {
                line =  cornerIdx+","+x/cnt+","+y/cnt+"\n";
                corners.add(new Corner(cornerIdx, x/cnt, y/cnt, cnt*CORNERAREAFACTOR));
                bufferedWriter.write(line);
                cornerIdx++;
            }
            else {
                cornersMap.remove(new Point(x, y));
                cornerToDel.add(new Point(ip));
            }
        }
        cornerToDel.stream().forEach(cornerRaw::remove);
        n = cornerIdx;
        pointCnt = new int[n][n];
        roadWidth = new double[n][n];
        for(i=0; i<n; i++) {
            for(j=0; j<n; j++) {
                pointCnt[i][j] = 0;
                roadWidth[i][j] = 10000;
            }
        }
        bufferedWriter.close();
        
        /***Detect Connections. Corner id calculated in previous stage.
         Run BFS***/
        j = 0;
        System.out.println("Checkpoint 4 "+pixels.size());
        System.out.println("Time passed: "+(System.currentTimeMillis() - timeTracker));
        timeTracker = System.currentTimeMillis();
        processedPoint.clear();
        for(Point ip : cornerRaw) {
            if(processedPoint.contains(ip)) {
                continue;
            }
            cornerIdx = (int) cornersMap.get(ip);
            temp = new ArrayList<>();
            temp.add(new Point(ip.x,ip.y));
            processedPoint.add(new Point(ip.x,ip.y));
            while(temp.size()>0) {
                p = temp.get(0);
//                if(j%100==0) System.out.println(j);
                j++;
                for(i=0; i<4; i++) {
                    pp = p.add(new Point(dirArray[0][i], dirArray[1][i]));
                    if(pixels.contains(pp)
                            && !processedPoint.contains(pp)) {
                        if(cornersMap.containsKey(pp)) {
                            tempIdx = (int) cornersMap.get(pp);
                            if (cornerIdx == tempIdx) {
                                temp.add(pp);
                                processedPoint.add(pp);
                            }
                            else {
                                corners.get(cornerIdx).adjacent.add(tempIdx);
                                corners.get(tempIdx).adjacent.add(cornerIdx);
                            }
                        }
                        else {
                            if(roadCornerMap.containsKey(pp)) {
                                if(cornerIdx == (int) roadCornerMap.get(pp)) {
                                    continue;
                                }
                            }
                            temp.add(pp);
                            if(roadCornerMap.containsKey(pp)) {
                                pointCnt[(int) roadCornerMap.get(pp)][cornerIdx]++;
                                pointCnt[cornerIdx][(int) roadCornerMap.get(pp)]++;
                                tempWidth = Math.min(roadWidth[cornerIdx][(int) roadCornerMap.get(pp)],
                                                    (double) pointWidth.get(pp));
                                roadWidth[cornerIdx][(int) roadCornerMap.get(pp)] = tempWidth;
                                roadWidth[(int) roadCornerMap.get(pp)][cornerIdx] = tempWidth;
                                processedPoint.add(pp);
                            }
                            else {
                                roadCornerMap.put(pp, cornerIdx);
                            }
                        }
                    }
                }
                temp.remove(0);
            }
        }
        
        biasX = Float.MAX_VALUE;
        biasY = Float.MAX_VALUE;
        
        m = 0;
        for(i=0; i<n; i++) {
            biasX = Math.min(biasX, corners.get(i).x);
            biasY = Math.min(biasY, corners.get(i).y);
            
//            System.out.println(i+" ("+corners.get(i).x+", "+corners.get(i).y+"):");
            for(Integer it : corners.get(i).adjacent) {
//                System.out.print(it+" ");
                y = roadWidth[i][it]-1;
                x = pointCnt[i][it]*4;
                x += corners.get(i).area/corners.get(i).adjacent.size();
                x += corners.get(it).area/corners.get(it).adjacent.size();
                x /= y;
//                System.out.print(" (Length = "+x+", Width = "+y+") ");
//                System.out.println();
                
                if(it>i) {
//                    System.out.println("Link between "+i+" "+it+" "+m);
                    link = new Road(m, i, it, corners.get(i), corners.get(it), y);
                    corners.get(i).linkId.add(m);
                    corners.get(it).linkId.add(m);
                    links.add(link);
                    m++;
                }
            }
        }
        
        biasX -= 10;
        biasY -= 10;
        
        
        for(i=0; i<m; i++) {
            cornerIdx = links.get(i).cornerId1;
            for(j=0; j<corners.get(cornerIdx).linkId.size(); j++) {
                k = corners.get(cornerIdx).linkId.get(j);
                if(i == k) continue;
                pp1 = ComputeLineIntersection(links.get(i).line00,
                                            links.get(i).line01,
                                            links.get(k).line00,
                                            links.get(k).line01);
                pp2 = ComputeLineIntersection(links.get(i).line10,
                                            links.get(i).line11,
                                            links.get(k).line10,
                                            links.get(k).line11);                            
                if(links.get(i).line01.distFrom(pp1) < links.get(i).line01.distFrom(links.get(i).line00)
                   && corners.get(cornerIdx).distFrom(pp1) <= corners.get(cornerIdx).r ) {
                    links.get(i).line00 = pp1;
                }
                if(links.get(i).line11.distFrom(pp2) < links.get(i).line11.distFrom(links.get(i).line10)
                    && corners.get(cornerIdx).distFrom(pp2) <= corners.get(cornerIdx).r ) {
                    links.get(i).line10 = pp2;
                }
                pp1 = ComputeLineIntersection(links.get(i).line00,
                                            links.get(i).line01,
                                            links.get(k).line10,
                                            links.get(k).line11);
                pp2 = ComputeLineIntersection(links.get(i).line10,
                                            links.get(i).line11,
                                            links.get(k).line00,
                                            links.get(k).line01);                            
                if(links.get(i).line01.distFrom(pp1) < links.get(i).line01.distFrom(links.get(i).line00)
                    && corners.get(cornerIdx).distFrom(pp1) <= corners.get(cornerIdx).r ) {
                    links.get(i).line00 = pp1;
                }
                if(links.get(i).line11.distFrom(pp2) < links.get(i).line11.distFrom(links.get(i).line10)
                    && corners.get(cornerIdx).distFrom(pp2) <= corners.get(cornerIdx).r ) {
                    links.get(i).line10 = pp2;
                }
            }
            if(links.get(i).line01.distFrom(links.get(i).line00) > links.get(i).line11.distFrom(links.get(i).line10)) {
                scaling = links.get(i).line11.distFrom(links.get(i).line10);
                pp =  (links.get(i).line00.minus(links.get(i).line01)).scaledP(scaling);
                links.get(i).line00 = links.get(i).line01.add(pp);
            }
            
            else {
                scaling = links.get(i).line01.distFrom(links.get(i).line00);
                pp =  (links.get(i).line10.minus(links.get(i).line11)).scaledP(scaling);
                links.get(i).line10 = links.get(i).line11.add(pp);
            }
            
            cornerIdx = links.get(i).cornerId2;
            for(j=0; j<corners.get(cornerIdx).linkId.size(); j++) {
                k = corners.get(cornerIdx).linkId.get(j);
                if(i == k) continue;
                pp1 = ComputeLineIntersection(links.get(i).line00,
                                            links.get(i).line01,
                                            links.get(k).line00,
                                            links.get(k).line01);
                pp2 = ComputeLineIntersection(links.get(i).line10,
                                            links.get(i).line11,
                                            links.get(k).line10,
                                            links.get(k).line11);  
                if(links.get(i).line00.distFrom(pp1) < links.get(i).line00.distFrom(links.get(i).line01)
                    && corners.get(cornerIdx).distFrom(pp1) <= corners.get(cornerIdx).r ) {
                    links.get(i).line01 = pp1;
                }
                if(links.get(i).line10.distFrom(pp2) < links.get(i).line10.distFrom(links.get(i).line11)
                    && corners.get(cornerIdx).distFrom(pp2) <= corners.get(cornerIdx).r ) {
                    links.get(i).line11 = pp2;
                }
                pp1 = ComputeLineIntersection(links.get(i).line00,
                                            links.get(i).line01,
                                            links.get(k).line10,
                                            links.get(k).line11);
                pp2 = ComputeLineIntersection(links.get(i).line10,
                                            links.get(i).line11,
                                            links.get(k).line00,
                                            links.get(k).line01);                            
                if(links.get(i).line00.distFrom(pp1) < links.get(i).line00.distFrom(links.get(i).line01)
                    && corners.get(cornerIdx).distFrom(pp1) <= corners.get(cornerIdx).r ) {
                    links.get(i).line01 = pp1;
                }
                if(links.get(i).line10.distFrom(pp2) < links.get(i).line10.distFrom(links.get(i).line11)
                    && corners.get(cornerIdx).distFrom(pp2) <= corners.get(cornerIdx).r ) {
                    links.get(i).line11 = pp2;
                }
            }
                
            if(links.get(i).line00.distFrom(links.get(i).line01) > links.get(i).line10.distFrom(links.get(i).line11)) {
                scaling = links.get(i).line10.distFrom(links.get(i).line11);
                pp =  (links.get(i).line01.minus(links.get(i).line00)).scaledP(scaling);
                links.get(i).line01 = links.get(i).line00.add(pp);
            }
            
            else {
                scaling = links.get(i).line00.distFrom(links.get(i).line01);
                pp =  (links.get(i).line11.minus(links.get(i).line10)).scaledP(scaling);
                links.get(i).line11 = links.get(i).line10.add(pp);
            }
        }
        
        
        /***Formatted Output For Simulator***/
        fileWriter = new FileWriter("junctions.txt");
        bufferedWriter = new BufferedWriter(fileWriter);
        line =  n + "\n";
        bufferedWriter.write(line);
        
        for(i=0; i<n; i++) {
            line = "" + i;
            for(j=0; j<corners.get(i).linkId.size(); j++) {
                line += " " + corners.get(i).linkId.get(j);
            }
            line += "\n";
            bufferedWriter.write(line);
        }
        bufferedWriter.close();
        
        fileWriter = new FileWriter("links.csv");
        bufferedWriter = new BufferedWriter(fileWriter);
        line = "ID,X,Y\n";
        bufferedWriter.write(line);
        
        for(i=0; i<m; i++) {
            bufferedWriter.write(links.get(i).Show2());
        }
        bufferedWriter.close();
        
        fileWriter = new FileWriter("links.txt");
        bufferedWriter = new BufferedWriter(fileWriter);
        line =  m + "\n";
        bufferedWriter.write(line);
        
        for(i=0; i<m; i++) {
            line = i + " " + links.get(i).cornerId1 + " " + links.get(i).cornerId2 + " 1\n";
            bufferedWriter.write(line);
            line = "0 " + (links.get(i).line00.x - biasX) + " " + (-1)*(links.get(i).line00.y - biasY)
                  + " " + (links.get(i).line01.x - biasX) + " " + (-1)*(links.get(i).line01.y - biasY)
                  + " " + links.get(i).width + "\n";
            bufferedWriter.write(line);
        }
        bufferedWriter.close();
        
        fileWriter = new FileWriter("glInput.txt");
        bufferedWriter = new BufferedWriter(fileWriter);
        
        line = corners.size() + "\n";
        bufferedWriter.write(line);
        
        for(Corner cr : corners) {
            line = (cr.x - biasX) + " " + (cr.y - biasY) + " " + cr.r + "\n";
            bufferedWriter.write(line);
        }
        
        line =  (2*m) + "\n";
        bufferedWriter.write(line);
        
        for(i=0; i<m; i++) {
            line = i + " " + (links.get(i).line00.x - biasX) + " " + (links.get(i).line00.y - biasY)
                  + " " + (links.get(i).line01.x - biasX) + " " + (links.get(i).line01.y - biasY)
                  + "\n";
            line += i + " " + (links.get(i).line10.x - biasX) + " " + (links.get(i).line10.y - biasY)
                  + " " + (links.get(i).line11.x - biasX) + " " + (links.get(i).line11.y - biasY)
                  + "\n";
            bufferedWriter.write(line);
        }
        bufferedWriter.close();
        System.out.println("Time passed: "+(System.currentTimeMillis() - timeTracker));
    }
}