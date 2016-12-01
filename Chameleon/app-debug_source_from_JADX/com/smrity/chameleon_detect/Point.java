package com.smrity.chameleon_detect;

import java.util.ArrayList;

public class Point {
    int assigned;
    ArrayList<Double> coord;
    ArrayList<Double> tolerance;

    public Point() {
        this.coord = new ArrayList();
        this.tolerance = new ArrayList();
    }

    double distanceFrom(Point p) {
        double ans = 0.0d;
        for (int i = 0; i < this.coord.size(); i++) {
            ans += (((Double) this.coord.get(i)).doubleValue() - ((Double) p.coord.get(i)).doubleValue()) * (((Double) this.coord.get(i)).doubleValue() - ((Double) p.coord.get(i)).doubleValue());
        }
        return Math.sqrt(ans);
    }

    boolean contains(Point p) {
        int d = this.coord.size();
        int missed = 0;
        for (int i = 0; i < d; i++) {
            if (Math.abs(((Double) this.coord.get(i)).doubleValue() - ((Double) p.coord.get(i)).doubleValue()) > ((Double) this.tolerance.get(i)).doubleValue()) {
                missed++;
            }
        }
        return missed == 0;
    }
}
