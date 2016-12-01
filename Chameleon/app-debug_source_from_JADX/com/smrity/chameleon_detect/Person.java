package com.smrity.chameleon_detect;

import java.util.ArrayList;

public class Person {
    Point mid;
    String name;
    ArrayList<Point> pts;

    public Person(String _name) {
        this.pts = new ArrayList();
        this.mid = new Point();
        this.name = _name;
    }
}
