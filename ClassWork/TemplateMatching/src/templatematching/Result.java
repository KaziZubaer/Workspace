/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package templatematching;

/**
 *
 * @author kazi
 */
public class Result {
    int x, y;
    double value;
    
    public Result() {
        x = -1;
        y = -1;
        value = 1e20;
    }
    
    void Copy(Result r) {
        x = r.x;
        y = r.y;
        value = r.value;
    }
    
    void Show() {
        System.out.println(x + ", " + y + ", " + value);
    }
}
