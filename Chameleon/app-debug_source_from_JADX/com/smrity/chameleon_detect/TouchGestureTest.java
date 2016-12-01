package com.smrity.chameleon_detect;

import android.app.Activity;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.support.v4.widget.CursorAdapter;
import android.view.MotionEvent;
import android.view.VelocityTracker;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.ArrayList;
import java.util.HashMap;

public class TouchGestureTest extends Activity {
    public static FileOutputStream fOut;
    public static File file;
    public static String fileNo;
    public static OutputStreamWriter myOutWriter;
    public static HashMap nameId;
    public static ArrayList<Person> persons;
    float new_pos_x;
    float new_pos_y;
    float new_size;
    float old_pos_x;
    float old_pos_y;
    float old_size;
    float pressure_new;
    float pressure_old;
    float time_new;
    float time_old;
    VelocityTracker velocity;

    /* renamed from: com.smrity.chameleon_detect.TouchGestureTest.1 */
    class C00631 implements SensorEventListener {
        final /* synthetic */ float[] val$mValuesAccel;
        final /* synthetic */ float[] val$mValuesMagnet;

        C00631(float[] fArr, float[] fArr2) {
            this.val$mValuesAccel = fArr;
            this.val$mValuesMagnet = fArr2;
        }

        public void onSensorChanged(SensorEvent event) {
            switch (event.sensor.getType()) {
                case CursorAdapter.FLAG_AUTO_REQUERY /*1*/:
                    System.arraycopy(event.values, 0, this.val$mValuesAccel, 0, 3);
                case CursorAdapter.FLAG_REGISTER_CONTENT_OBSERVER /*2*/:
                    System.arraycopy(event.values, 0, this.val$mValuesMagnet, 0, 3);
                default:
            }
        }

        public void onAccuracyChanged(Sensor sensor, int accuracy) {
        }
    }

    /* renamed from: com.smrity.chameleon_detect.TouchGestureTest.2 */
    class C00642 implements OnClickListener {
        final /* synthetic */ float[] val$mRotationMatrix;
        final /* synthetic */ float[] val$mValuesAccel;
        final /* synthetic */ float[] val$mValuesMagnet;
        final /* synthetic */ float[] val$mValuesOrientation;
        final /* synthetic */ String val$mimic;
        final /* synthetic */ String val$name;

        C00642(float[] fArr, float[] fArr2, float[] fArr3, float[] fArr4, String str, String str2) {
            this.val$mRotationMatrix = fArr;
            this.val$mValuesAccel = fArr2;
            this.val$mValuesMagnet = fArr3;
            this.val$mValuesOrientation = fArr4;
            this.val$name = str;
            this.val$mimic = str2;
        }

        public void onClick(View v) {
            TouchGestureTest.this.velocity.computeCurrentVelocity(1000);
            float x_velocity = TouchGestureTest.this.velocity.getXVelocity();
            float y_velocity = TouchGestureTest.this.velocity.getYVelocity();
            SensorManager.getRotationMatrix(this.val$mRotationMatrix, null, this.val$mValuesAccel, this.val$mValuesMagnet);
            SensorManager.getOrientation(this.val$mRotationMatrix, this.val$mValuesOrientation);
            float angle = (float) (((double) SensorManager.getInclination(this.val$mRotationMatrix)) * 57.2957795d);
            Point p = new Point();
            ArrayList<String> acceptedList = new ArrayList();
            try {
                int i;
                TouchGestureTest.file = new File("/sdcard/chameleon_log.txt");
                TouchGestureTest.fOut = new FileOutputStream(TouchGestureTest.file, true);
                TouchGestureTest.myOutWriter = new OutputStreamWriter(TouchGestureTest.fOut);
                p.coord.add(Double.valueOf((double) TouchGestureTest.this.old_pos_x));
                p.coord.add(Double.valueOf((double) TouchGestureTest.this.old_pos_y));
                p.coord.add(Double.valueOf((double) TouchGestureTest.this.new_pos_x));
                p.coord.add(Double.valueOf((double) TouchGestureTest.this.new_pos_y));
                p.coord.add(Double.valueOf((double) TouchGestureTest.this.old_size));
                p.coord.add(Double.valueOf((double) TouchGestureTest.this.new_size));
                p.coord.add(Double.valueOf((double) TouchGestureTest.this.pressure_old));
                p.coord.add(Double.valueOf((double) TouchGestureTest.this.pressure_new));
                p.coord.add(Double.valueOf((double) angle));
                p.coord.add(Double.valueOf((double) this.val$mValuesOrientation[0]));
                p.coord.add(Double.valueOf((double) this.val$mValuesOrientation[1]));
                p.coord.add(Double.valueOf((double) this.val$mValuesOrientation[2]));
                p.coord.add(Double.valueOf((double) x_velocity));
                p.coord.add(Double.valueOf((double) y_velocity));
                p.coord.add(Double.valueOf((double) (TouchGestureTest.this.time_new - TouchGestureTest.this.time_old)));
                TouchGestureTest.myOutWriter.append(this.val$name);
                for (i = 0; i < TouchGestureTest.persons.size(); i++) {
                    if (((Person) TouchGestureTest.persons.get(i)).mid.contains(p)) {
                        acceptedList.add(((Person) TouchGestureTest.persons.get(i)).name);
                        Toast.makeText(TouchGestureTest.this.getApplicationContext(), "Accepted as " + ((Person) TouchGestureTest.persons.get(i)).name, 0).show();
                    }
                }
                if (acceptedList.size() == 0) {
                    TouchGestureTest.myOutWriter.append(" Reject\n");
                    Toast.makeText(TouchGestureTest.this.getApplicationContext(), "Rejected", 0).show();
                } else {
                    TouchGestureTest.myOutWriter.append(" Accepted as ");
                }
                i = 0;
                while (i < acceptedList.size() - 1) {
                    TouchGestureTest.myOutWriter.append(((String) acceptedList.get(i)) + ",");
                    i++;
                }
                if (acceptedList.size() > 0) {
                    TouchGestureTest.myOutWriter.append(((String) acceptedList.get(i)) + "\n");
                }
                TouchGestureTest.myOutWriter.append("Present List: ");
                i = 0;
                while (i < TouchGestureTest.persons.size() - 1) {
                    TouchGestureTest.myOutWriter.append(((Person) TouchGestureTest.persons.get(i)).name + ",");
                    i++;
                }
                if (TouchGestureTest.persons.size() > 0) {
                    TouchGestureTest.myOutWriter.append(((Person) TouchGestureTest.persons.get(i)).name + "\n");
                }
                TouchGestureTest.myOutWriter.append("Mimic: " + this.val$mimic + "\n");
                TouchGestureTest.myOutWriter.close();
                TouchGestureTest.fOut.close();
            } catch (Exception e) {
                Toast.makeText(TouchGestureTest.this.getBaseContext(), e.getMessage(), 0).show();
            }
        }
    }

    public TouchGestureTest() {
        this.velocity = VelocityTracker.obtain();
    }

    static {
        nameId = new HashMap();
        persons = new ArrayList();
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(C0059R.layout.touch_test);
        Intent intent = getIntent();
        String name = intent.getStringExtra("Name");
        fileNo = intent.getStringExtra("FileNo");
        String mimic = intent.getStringExtra("Mimic");
        Calculate();
        float[] mValuesMagnet = new float[3];
        float[] mValuesAccel = new float[3];
        float[] mValuesOrientation = new float[3];
        float[] mRotationMatrix = new float[9];
        setListners((SensorManager) getSystemService("sensor"), new C00631(mValuesAccel, mValuesMagnet));
        ((Button) findViewById(C0059R.id.buttonTestInfo)).setOnClickListener(new C00642(mRotationMatrix, mValuesAccel, mValuesMagnet, mValuesOrientation, name, mimic));
    }

    private void Calculate() {
        Point tempPoint;
        int i;
        int j;
        File file = new File("/sdcard/chameleon_data" + fileNo + ".txt");
        ArrayList<Point> allPoints = new ArrayList();
        persons.clear();
        nameId.clear();
        try {
            BufferedReader bufferedReader = new BufferedReader(new FileReader(file));
            while (true) {
                String line = bufferedReader.readLine();
                if (line == null) {
                    break;
                }
                tempPoint = new Point();
                String[] lineSegment = line.split(",", 14);
                for (i = 0; i < 13; i++) {
                    tempPoint.coord.add(Double.valueOf(Double.parseDouble(lineSegment[i])));
                }
                if (!nameId.containsKey(lineSegment[13])) {
                    Person person = new Person(lineSegment[13]);
                    nameId.put(lineSegment[13], Integer.valueOf(persons.size()));
                    persons.add(person);
                }
                int id = ((Integer) nameId.get(lineSegment[13])).intValue();
                ((Person) persons.get(id)).pts.add(tempPoint);
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e2) {
            e2.printStackTrace();
        }
        int k = persons.size();
        for (i = 0; i < k; i++) {
            tempPoint = new Point();
            int m = 0;
            while (true) {
                if (m >= ((Person) persons.get(i)).pts.size()) {
                    break;
                }
                allPoints.add(((Person) persons.get(i)).pts.get(m));
                m++;
            }
            for (j = 0; j < 13; j++) {
                double y;
                double x = 0.0d;
                m = 0;
                while (true) {
                    if (m >= ((Person) persons.get(i)).pts.size()) {
                        break;
                    }
                    x += ((Double) ((Point) ((Person) persons.get(i)).pts.get(m)).coord.get(j)).doubleValue();
                    m++;
                }
                x /= (double) ((Person) persons.get(i)).pts.size();
                tempPoint.coord.add(Double.valueOf(x));
                x = 0.0d;
                m = 0;
                while (true) {
                    if (m >= ((Person) persons.get(i)).pts.size()) {
                        break;
                    }
                    y = ((Double) ((Point) ((Person) persons.get(i)).pts.get(m)).coord.get(j)).doubleValue() - ((Double) tempPoint.coord.get(j)).doubleValue();
                    x += y * y;
                    m++;
                }
                x = Math.sqrt(x / ((double) ((Person) persons.get(i)).pts.size())) * 5.41d;
                y = Math.abs(((Double) tempPoint.coord.get(j)).doubleValue() * 0.025750000000000002d);
                tempPoint.tolerance.add(Double.valueOf(x + y));
            }
            ((Person) persons.get(i)).mid = tempPoint;
        }
        double maxDiff = 1.000001d;
        int n = allPoints.size();
        int it = 0;
        while (maxDiff > 1.0E-6d) {
            int it2 = it + 1;
            if (it < 1000) {
                maxDiff = 0.0d;
                for (i = 0; i < n; i++) {
                    ((Point) allPoints.get(i)).assigned = -1;
                    double minDist = 1.0E9d;
                    for (j = 0; j < k; j++) {
                        if (((Person) persons.get(j)).mid.contains((Point) allPoints.get(i))) {
                            if (((Point) allPoints.get(i)).distanceFrom(((Person) persons.get(j)).mid) < minDist) {
                                ((Point) allPoints.get(i)).assigned = j;
                                minDist = ((Point) allPoints.get(i)).distanceFrom(((Person) persons.get(j)).mid);
                            }
                        }
                    }
                }
                for (i = 0; i < k; i++) {
                    int i2;
                    tempPoint = new Point();
                    for (m = 0; m < 13; m++) {
                        tempPoint.coord.add(((Person) persons.get(i)).mid.coord.get(m));
                        ((Person) persons.get(i)).mid.coord.set(m, Double.valueOf(0.0d));
                    }
                    int cnt = 0;
                    for (j = 0; j < n; j++) {
                        i2 = ((Point) allPoints.get(j)).assigned;
                        if (r0 == i) {
                            cnt++;
                            for (m = 0; m < 13; m++) {
                                ((Person) persons.get(i)).mid.coord.set(m, Double.valueOf(((Double) ((Person) persons.get(i)).mid.coord.get(m)).doubleValue() + ((Double) ((Point) allPoints.get(j)).coord.get(m)).doubleValue()));
                            }
                        }
                    }
                    if (cnt > 0) {
                        for (m = 0; m < 13; m++) {
                            double d = (double) cnt;
                            ((Person) persons.get(i)).mid.coord.set(m, Double.valueOf(((Double) ((Person) persons.get(i)).mid.coord.get(m)).doubleValue() / r0));
                            ((Person) persons.get(i)).mid.tolerance.set(m, Double.valueOf(Math.abs(((Double) ((Person) persons.get(i)).mid.coord.get(m)).doubleValue() * 0.025750000000000002d)));
                            x = 0.0d;
                            for (j = 0; j < n; j++) {
                                i2 = ((Point) allPoints.get(j)).assigned;
                                if (r0 == i) {
                                    y = ((Double) ((Point) allPoints.get(j)).coord.get(m)).doubleValue() - ((Double) ((Person) persons.get(i)).mid.coord.get(m)).doubleValue();
                                    x += y * y;
                                }
                            }
                            x = Math.sqrt(x / ((double) cnt)) * 5.41d;
                            ((Person) persons.get(i)).mid.tolerance.set(m, Double.valueOf(((Double) ((Person) persons.get(i)).mid.tolerance.get(m)).doubleValue() + x));
                            double d2 = maxDiff;
                            maxDiff = Math.max(d2, Math.abs(((Double) ((Person) persons.get(i)).mid.coord.get(m)).doubleValue() - ((Double) tempPoint.coord.get(m)).doubleValue()));
                        }
                    }
                }
                it = it2;
            } else {
                return;
            }
        }
    }

    public boolean onTouchEvent(MotionEvent event) {
        int pointerIndex;
        if (event.getAction() == 0) {
            pointerIndex = event.findPointerIndex(event.getPointerId(0));
            this.old_pos_x = event.getX();
            this.old_pos_y = event.getY();
            this.time_old = (float) event.getEventTime();
            this.old_size = event.getSize(pointerIndex);
            this.pressure_old = event.getPressure(pointerIndex);
        } else if (event.getAction() == 1) {
            pointerIndex = event.findPointerIndex(event.getPointerId(0));
            this.new_pos_x = event.getX();
            this.new_pos_y = event.getY();
            this.time_new = (float) event.getEventTime();
            this.new_size = event.getSize(pointerIndex);
            this.pressure_new = event.getPressure(pointerIndex);
        }
        if (event.getAction() == 2) {
            this.velocity.addMovement(event);
        }
        return super.onTouchEvent(event);
    }

    public void setListners(SensorManager sensorManager, SensorEventListener mEventListener) {
        sensorManager.registerListener(mEventListener, sensorManager.getDefaultSensor(1), 3);
        sensorManager.registerListener(mEventListener, sensorManager.getDefaultSensor(2), 3);
    }
}
