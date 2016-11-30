/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package templatematching;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.Image;
import java.awt.image.BufferedImage;
import java.awt.image.ConvolveOp;
import java.awt.image.Kernel;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

/**
 *
 * @author kazi
 */
public class TemplateMatching {

    /**
     * @param args the command line arguments
     */
    
    static final int MAXLEVEL = 2;
    static final File REF = new File("images/ref2.bmp");
    static final File TEST = new File("images/test.bmp");
    static Color refImage[][],testImage[][];
    static int refDim[] = new int[2];
    static int testDim[] = new int[2];
    
    static double Log2(double x) {
        return Math.log(x)/Math.log(2) - 1e-10;
    }
    
    static void ReadRef() throws IOException {
        int i,j;
        BufferedImage image = ImageIO.read(REF);
        refDim[0] = image.getHeight();
        refDim[1] = image.getWidth();
        refImage = new Color[refDim[0]][refDim[1]];
        for(i = 0; i < refDim[1]; i++) {
            for(j= 0; j < refDim[0]; j++) {
                refImage[j][i] = new Color(image.getRGB(i, j));
            }
        }
    }
    
    static void ReadTest() throws IOException {
        int i,j;
        BufferedImage image = ImageIO.read(TEST);
        testDim[0] = image.getHeight();
        testDim[1] = image.getWidth();
        testImage = new Color[testDim[0]][testDim[1]];
        for(i = 0; i < testDim[1]; i++) {
            for(j= 0; j < testDim[0]; j++) {
                testImage[j][i] = new Color(image.getRGB(i, j));
            }
        }
    }
    
    static double GetDistance(Color [][] image1, Color [][] image2, 
                        int sx1, int sy1, int sx2, int sy2, int h, int w) {
        int i, j;
        double ret = 0, val1, val2;
        for(i = 0; i < h; i++) {
            for(j = 0; j< w; j++) {
                val1 = image1[sx1 + i][sy1 + j].getRed();
                val1 += image1[sx1 + i][sy1 + j].getGreen();
                val1 += image1[sx1 + i][sy1 + j].getBlue();
                val1 /= 3.0;
                val2 = image2[sx2 + i][sy2 + j].getRed();
                val2 += image2[sx2 + i][sy2 + j].getGreen();
                val2 += image2[sx2 + i][sy2 + j].getBlue();
                val2 /= 3.0;
                ret += (val1 - val2)*(val1 - val2);
            }
        }
        return Math.sqrt(ret);
    }
    
    static double GetDistance2(Color [][] image1, Color [][] image2, 
                        int sx1, int sy1, int sx2, int sy2, int h, int w) {
        int i, j, ret = 0, val1, val2;
        for(i = 0; i < h; i++) {
            for(j = 0; j< w; j++) {
                val1 = image1[sx1 + i][sy1 + j].getRed();
                val1 += image1[sx1 + i][sy1 + j].getGreen();
                val1 += image1[sx1 + i][sy1 + j].getBlue();
                val1 /= 255*3;
                val2 = image2[sx2 + i][sy2 + j].getRed();
                val2 += image2[sx2 + i][sy2 + j].getGreen();
                val2 += image2[sx2 + i][sy2 + j].getBlue();
                val2 /= 255*3;
                ret += Math.abs(val1 - val2);
            }
        }
        return ret;
    }
    
    static void BruteForce() {
        int i, j, miniI = -1, miniJ = -1;
        double mini = refDim[0] * refDim[1] * 255 * 255, temp;
        
        for(i = 0; i <= testDim[0]-refDim[0]; i++) {
            for(j = 0; j <= testDim[1]-refDim[1]; j++) {
                temp = GetDistance(refImage, testImage, 0, 0, i, j, refDim[0], refDim[1]);
                if(temp < mini) {
                    mini = temp;
                    miniI= i;
                    miniJ = j;
                }
            }
            System.out.println("I = " + i);
        }
        
        System.out.println(miniI + ", " + miniJ + ", " + mini);
    }
    
    static Result LogarithmRecursion(int cx, int cy,int p) {
        int k, d = 1, i, j, offset, x, y;
        Result ret = new Result(), temp;
        k = (int)Log2(p);
        if(k == 0) {
            ret.x = Math.max(cx - refDim[0],0);
            ret.y = Math.max(cy - refDim[1],0);
            ret.value = GetDistance2(refImage, testImage, 0, 0, Math.max(cx - refDim[0], 0),
                                    Math.max(cy - refDim[1], 0), refDim[0], refDim[1]);
            return ret;
        }
        for(i = 0; i < k; i++) {
            d *= 2;
        }
        
        offset = p - d;
        
        for(i = 0; i < 3; i++) {
            x = cx - p + offset + i*d;
            for(j = 0; j < 3; j++) {
                y = cy - p + offset + j*d;
                temp = LogarithmRecursion(x, y, p/4);
                if(temp.value < ret.value) {
                    ret.Copy(temp);
                }
            }
        }
        return ret;
    }
    
    static void Logarithmic() {
        Result res = LogarithmRecursion(512, 512, testDim[0]/2);
        res.Show();
    }
    
    static BufferedImage GetBlurImage(BufferedImage image) {
        int size = 5;
        float weight = 1.0f / (size * size);
        float[] data = new float[size * size];

        for (int i = 0; i < data.length; i++) {
            data[i] = weight;
        }

        Kernel kernel = new Kernel(size, size, data);
        ConvolveOp op = new ConvolveOp(kernel, ConvolveOp.EDGE_NO_OP, null);
        
        BufferedImage ret = op.filter(image, null);
        Image img = ret.getScaledInstance(ret.getWidth()/2, ret.getHeight()/2, Image.SCALE_SMOOTH);
        if (img instanceof BufferedImage) {
            return (BufferedImage) img;
        }

        BufferedImage finalRet = new BufferedImage(img.getWidth(null), img.getHeight(null), BufferedImage.TYPE_INT_RGB);
        Graphics2D bGr = finalRet.createGraphics();
        bGr.drawImage(img, 0, 0, null);
        bGr.dispose();

        return finalRet;
    }
    
    static Result HierarchialRecursion( BufferedImage refImageBuffer,
                                        BufferedImage testImageBuffer, int level) {
        
        int testH, testW, refH, refW, i, j, sx, sy;
        Result ret = new Result();
        double val;
        
        testH = testImageBuffer.getHeight();
        testW = testImageBuffer.getWidth();
        refH = refImageBuffer.getHeight();
        refW = refImageBuffer.getWidth();
        
        Color[][] ref = new Color[refH][refW];
        Color[][] test = new Color[testH][testW];
        
        for(i = 0; i < refW; i++) {
            for(j= 0; j < refH; j++) {
                ref[j][i] = new Color(refImageBuffer.getRGB(i, j));
            }
        }
        
        for(i = 0; i < testW; i++) {
            for(j= 0; j < testH; j++) {
                test[j][i] = new Color(testImageBuffer.getRGB(i, j));
            }
        }
                
        if(level == MAXLEVEL) {
            
            for(i = 0; i <= testH - refH; i++) {
                for(j = 0; j <= testW - refW; j++) {
                    val = GetDistance(ref, test, 0, 0, i, j, refH, refW);
                    if(val < ret.value) {
                        ret.value = val;
                        ret.x= i;
                        ret.y = j;
                    }
                }
            }
            
            return ret;
        }
        
        BufferedImage blurRef = GetBlurImage(refImageBuffer);
        BufferedImage blurTest = GetBlurImage(testImageBuffer);
        
        ret = HierarchialRecursion(blurRef, blurTest, level+1);
        
        sx = 2*ret.y - refImageBuffer.getWidth();
        sy = 2*ret.x - refImageBuffer.getHeight();
        
        BufferedImage croppedTest = testImageBuffer.getSubimage(sx, sy, 
                                                                refImageBuffer.getWidth()*3, 
                                                                refImageBuffer.getHeight()*3);
        
        testH = croppedTest.getHeight();
        testW = croppedTest.getWidth();
        
        Color[][] crop = new Color[testH][testW];
        
        for(i = 0; i < testW; i++) {
            for(j= 0; j < testH; j++) {
                crop[j][i] = new Color(croppedTest.getRGB(i, j));
            }
        }
        
        ret.value = 1e20;
                
        for(i = 0; i <= testH - refH; i++) {
            for(j = 0; j <= testW - refW; j++) {
                val = GetDistance(ref, crop, 0, 0, i, j, refH, refW);
                if(val < ret.value) {
                    ret.value = val;
                    ret.x= i;
                    ret.y = j;
                }
            }
        }
        
        ret.y += sx;
        ret.x += sy;
        
        return ret;
    }
    
    static void Hierarchial() throws IOException {
        Result res = HierarchialRecursion(ImageIO.read(REF),ImageIO.read(TEST),0);
        res.Show();
    }
    
    public static void main(String[] args) throws IOException {
        // TODO code application logic here
        
        ReadRef();
        ReadTest();
        
        //BruteForce();       
        Logarithmic();
        //Hierarchial();
                
    }
    
}
