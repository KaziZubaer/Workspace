package com.smrity.chameleon_detect;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.os.Handler;
import android.support.v4.widget.CursorAdapter;
import android.view.MotionEvent;
import android.view.VelocityTracker;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

public class TouchGesturePractice extends Activity {
    public static FileOutputStream fOut;
    public static File file;
    public static OutputStreamWriter myOutWriter;
    int cnt;
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

    /* renamed from: com.smrity.chameleon_detect.TouchGesturePractice.1 */
    class C00601 implements SensorEventListener {
        final /* synthetic */ float[] val$mValuesAccel;
        final /* synthetic */ float[] val$mValuesMagnet;

        C00601(float[] fArr, float[] fArr2) {
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

    /* renamed from: com.smrity.chameleon_detect.TouchGesturePractice.2 */
    class C00622 implements OnClickListener {
        final /* synthetic */ float[] val$mRotationMatrix;
        final /* synthetic */ float[] val$mValuesAccel;
        final /* synthetic */ float[] val$mValuesMagnet;
        final /* synthetic */ float[] val$mValuesOrientation;
        final /* synthetic */ String val$name;

        /* renamed from: com.smrity.chameleon_detect.TouchGesturePractice.2.1 */
        class C00611 implements Runnable {
            final /* synthetic */ Toast val$t;

            C00611(Toast toast) {
                this.val$t = toast;
            }

            public void run() {
                this.val$t.cancel();
            }
        }

        C00622(float[] fArr, float[] fArr2, float[] fArr3, float[] fArr4, String str) {
            this.val$mRotationMatrix = fArr;
            this.val$mValuesAccel = fArr2;
            this.val$mValuesMagnet = fArr3;
            this.val$mValuesOrientation = fArr4;
            this.val$name = str;
        }

        public void onClick(View v) {
            TouchGesturePractice.this.velocity.computeCurrentVelocity(1000);
            float x_velocity = TouchGesturePractice.this.velocity.getXVelocity();
            float y_velocity = TouchGesturePractice.this.velocity.getYVelocity();
            SensorManager.getRotationMatrix(this.val$mRotationMatrix, null, this.val$mValuesAccel, this.val$mValuesMagnet);
            SensorManager.getOrientation(this.val$mRotationMatrix, this.val$mValuesOrientation);
            float angle = (float) (((double) SensorManager.getInclination(this.val$mRotationMatrix)) * 57.2957795d);
            try {
                TouchGesturePractice.file = new File("/sdcard/chameleon_dummy.txt");
                TouchGesturePractice.fOut = new FileOutputStream(TouchGesturePractice.file, true);
                TouchGesturePractice.myOutWriter = new OutputStreamWriter(TouchGesturePractice.fOut);
                TouchGesturePractice.myOutWriter.append(TouchGesturePractice.this.old_pos_x + "," + TouchGesturePractice.this.old_pos_y + ",");
                TouchGesturePractice.myOutWriter.append(TouchGesturePractice.this.new_pos_x + "," + TouchGesturePractice.this.new_pos_y + ",");
                TouchGesturePractice.myOutWriter.append(TouchGesturePractice.this.old_size + ",");
                TouchGesturePractice.myOutWriter.append(TouchGesturePractice.this.new_size + ",");
                TouchGesturePractice.myOutWriter.append(TouchGesturePractice.this.pressure_old + ",");
                TouchGesturePractice.myOutWriter.append(TouchGesturePractice.this.pressure_new + ",");
                TouchGesturePractice.myOutWriter.append(angle + ",");
                TouchGesturePractice.myOutWriter.append(this.val$mValuesOrientation[0] + "," + this.val$mValuesOrientation[1] + "," + this.val$mValuesOrientation[2] + ",");
                TouchGesturePractice.myOutWriter.append(x_velocity + "," + y_velocity + ",");
                TouchGesturePractice.myOutWriter.append((TouchGesturePractice.this.time_new - TouchGesturePractice.this.time_old) + ",");
                TouchGesturePractice.myOutWriter.append(this.val$name + "\n");
                TouchGesturePractice.myOutWriter.close();
                TouchGesturePractice.fOut.close();
                Context applicationContext = TouchGesturePractice.this.getApplicationContext();
                StringBuilder append = new StringBuilder().append("Dummy data ");
                TouchGesturePractice touchGesturePractice = TouchGesturePractice.this;
                int i = touchGesturePractice.cnt + 1;
                touchGesturePractice.cnt = i;
                Toast t = Toast.makeText(applicationContext, append.append(i).append(" Saved").toString(), 1);
                t.show();
                new Handler().postDelayed(new C00611(t), 500);
            } catch (Exception e) {
                Toast.makeText(TouchGesturePractice.this.getBaseContext(), e.getMessage(), 0).show();
            }
        }
    }

    public TouchGesturePractice() {
        this.velocity = VelocityTracker.obtain();
        this.cnt = 0;
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(C0059R.layout.touch_practice);
        String name = getIntent().getStringExtra("Name");
        this.cnt = 0;
        float[] mValuesMagnet = new float[3];
        float[] mValuesAccel = new float[3];
        float[] mValuesOrientation = new float[3];
        float[] mRotationMatrix = new float[9];
        setListners((SensorManager) getSystemService("sensor"), new C00601(mValuesAccel, mValuesMagnet));
        ((Button) findViewById(C0059R.id.buttonStoreInfo)).setOnClickListener(new C00622(mRotationMatrix, mValuesAccel, mValuesMagnet, mValuesOrientation, name));
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
