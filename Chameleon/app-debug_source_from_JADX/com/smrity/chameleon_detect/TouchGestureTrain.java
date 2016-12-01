package com.smrity.chameleon_detect;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
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

public class TouchGestureTrain extends Activity {
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

    /* renamed from: com.smrity.chameleon_detect.TouchGestureTrain.1 */
    class C00651 implements SensorEventListener {
        final /* synthetic */ float[] val$mValuesAccel;
        final /* synthetic */ float[] val$mValuesMagnet;

        C00651(float[] fArr, float[] fArr2) {
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

    /* renamed from: com.smrity.chameleon_detect.TouchGestureTrain.2 */
    class C00672 implements OnClickListener {
        final /* synthetic */ String val$fileNo;
        final /* synthetic */ float[] val$mRotationMatrix;
        final /* synthetic */ float[] val$mValuesAccel;
        final /* synthetic */ float[] val$mValuesMagnet;
        final /* synthetic */ float[] val$mValuesOrientation;
        final /* synthetic */ String val$name;

        /* renamed from: com.smrity.chameleon_detect.TouchGestureTrain.2.1 */
        class C00661 implements Runnable {
            final /* synthetic */ Toast val$t;

            C00661(Toast toast) {
                this.val$t = toast;
            }

            public void run() {
                this.val$t.cancel();
            }
        }

        C00672(float[] fArr, float[] fArr2, float[] fArr3, float[] fArr4, String str, String str2) {
            this.val$mRotationMatrix = fArr;
            this.val$mValuesAccel = fArr2;
            this.val$mValuesMagnet = fArr3;
            this.val$mValuesOrientation = fArr4;
            this.val$fileNo = str;
            this.val$name = str2;
        }

        public void onClick(View v) {
            TouchGestureTrain.this.velocity.computeCurrentVelocity(1000);
            float x_velocity = TouchGestureTrain.this.velocity.getXVelocity();
            float y_velocity = TouchGestureTrain.this.velocity.getYVelocity();
            SensorManager.getRotationMatrix(this.val$mRotationMatrix, null, this.val$mValuesAccel, this.val$mValuesMagnet);
            SensorManager.getOrientation(this.val$mRotationMatrix, this.val$mValuesOrientation);
            float angle = (float) (((double) SensorManager.getInclination(this.val$mRotationMatrix)) * 57.2957795d);
            try {
                TouchGestureTrain.file = new File("/sdcard/chameleon_data" + this.val$fileNo + ".txt");
                TouchGestureTrain.fOut = new FileOutputStream(TouchGestureTrain.file, true);
                TouchGestureTrain.myOutWriter = new OutputStreamWriter(TouchGestureTrain.fOut);
                TouchGestureTrain.myOutWriter.append(TouchGestureTrain.this.old_pos_x + "," + TouchGestureTrain.this.old_pos_y + ",");
                TouchGestureTrain.myOutWriter.append(TouchGestureTrain.this.new_pos_x + "," + TouchGestureTrain.this.new_pos_y + ",");
                TouchGestureTrain.myOutWriter.append(TouchGestureTrain.this.old_size + ",");
                TouchGestureTrain.myOutWriter.append(TouchGestureTrain.this.new_size + ",");
                TouchGestureTrain.myOutWriter.append(angle + ",");
                TouchGestureTrain.myOutWriter.append(this.val$mValuesOrientation[0] + "," + this.val$mValuesOrientation[1] + "," + this.val$mValuesOrientation[2] + ",");
                TouchGestureTrain.myOutWriter.append(x_velocity + "," + y_velocity + ",");
                TouchGestureTrain.myOutWriter.append((TouchGestureTrain.this.time_new - TouchGestureTrain.this.time_old) + ",");
                TouchGestureTrain.myOutWriter.append(this.val$name + "\n");
                TouchGestureTrain.myOutWriter.close();
                TouchGestureTrain.fOut.close();
                Context applicationContext = TouchGestureTrain.this.getApplicationContext();
                StringBuilder append = new StringBuilder().append("Data ");
                TouchGestureTrain touchGestureTrain = TouchGestureTrain.this;
                int i = touchGestureTrain.cnt + 1;
                touchGestureTrain.cnt = i;
                Toast t = Toast.makeText(applicationContext, append.append(i).append(" Saved").toString(), 1);
                t.show();
                new Handler().postDelayed(new C00661(t), 500);
            } catch (Exception e) {
                Toast.makeText(TouchGestureTrain.this.getBaseContext(), e.getMessage(), 0).show();
            }
        }
    }

    public TouchGestureTrain() {
        this.velocity = VelocityTracker.obtain();
        this.cnt = 0;
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(C0059R.layout.touch_train);
        Intent intent = getIntent();
        String name = intent.getStringExtra("Name");
        String fileNo = intent.getStringExtra("FileNo");
        this.cnt = 0;
        float[] mValuesMagnet = new float[3];
        float[] mValuesAccel = new float[3];
        float[] mValuesOrientation = new float[3];
        float[] mRotationMatrix = new float[9];
        setListners((SensorManager) getSystemService("sensor"), new C00651(mValuesAccel, mValuesMagnet));
        ((Button) findViewById(C0059R.id.buttonStoreInfo)).setOnClickListener(new C00672(mRotationMatrix, mValuesAccel, mValuesMagnet, mValuesOrientation, fileNo, name));
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
