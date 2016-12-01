package com.smrity.chameleon_detect;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

import android.app.Activity;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.VelocityTracker;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

public class TouchGestureDetect extends Activity {

	public static File file;

	public static FileOutputStream fOut;

	public static OutputStreamWriter  myOutWriter;

	VelocityTracker velocity = VelocityTracker.obtain();
	float old_pos_x, old_pos_y, new_pos_x, new_pos_y;
	float time_old, time_new;
	float pressure_old, pressure_new;
	float old_size, new_size;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.touch_gesture);
		
		Intent intent = getIntent();
		
		final String name = intent.getStringExtra("Name");
		
		SensorManager sensorManager = (SensorManager) this.getSystemService(SENSOR_SERVICE);        
		
		final float[] mValuesMagnet      = new float[3];
        final float[] mValuesAccel       = new float[3];
        final float[] mValuesOrientation = new float[3];
        final float[] mRotationMatrix    = new float[9];
		
        SensorEventListener mEventListener = new SensorEventListener() {
			
			@Override
			public void onSensorChanged(SensorEvent event) {
				// Handle the events for which we registered
                switch (event.sensor.getType()) {
                    case Sensor.TYPE_ACCELEROMETER:
                        System.arraycopy(event.values, 0, mValuesAccel, 0, 3);
                        break;

                    case Sensor.TYPE_MAGNETIC_FIELD:
                        System.arraycopy(event.values, 0, mValuesMagnet, 0, 3);
                        break;
                }
			}
			
			@Override
			public void onAccuracyChanged(Sensor sensor, int accuracy) {
				// TODO Auto-generated method stub
				
			}
		};
		// You have set the event lisetner up, now just need to register this with the
        // sensor manager along with the sensor wanted.
        setListners(sensorManager, mEventListener);
		
		
		Button buttonSave = (Button) findViewById(R.id.buttonSaveInfo);

		buttonSave.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				velocity.computeCurrentVelocity(1000);
				float x_velocity = velocity.getXVelocity();
				float y_velocity = velocity.getYVelocity();
				SensorManager.getRotationMatrix(mRotationMatrix, null, mValuesAccel, mValuesMagnet);
                SensorManager.getOrientation(mRotationMatrix, mValuesOrientation);
                float angle = SensorManager.getInclination(mRotationMatrix);
                angle = (float) (angle * 57.2957795);
				try {
					
					file = new File("/sdcard/chameleon_"
							+ name + ".txt");
					
					fOut = new FileOutputStream(file, true);

					myOutWriter = new OutputStreamWriter(fOut);
					
					myOutWriter.append("old pos: "+ old_pos_x + " " + old_pos_y +"\n");
					myOutWriter.append("new pos: "+ new_pos_x + " " + new_pos_y +"\n");
					myOutWriter.append("old size: "+ old_size + "\n");
					myOutWriter.append("new size: "+ new_size + "\n");
					myOutWriter.append("old pressure: "+ pressure_old + "\n");
					myOutWriter.append("new pressure: "+ pressure_new + "\n");
					myOutWriter.append("angle: "+ angle + "\n");
					myOutWriter.append("incline matrix: "+ mValuesOrientation[0] +" "+mValuesOrientation[1]+ " "+ mValuesOrientation[2] + "\n");
					//MainActivity.myOutWriter.append("Velocity old: "+ x_velocity_old + " " + y_velocity_old +"\n");
					myOutWriter.append("Velocity: "+ x_velocity + " " + y_velocity +"\n");
					//MainActivity.myOutWriter.append("Velocity new: "+ x_velocity_new + " " + y_velocity_new +"\n");
					myOutWriter.append("time : "+ (time_new - time_old)  +"\n");
					myOutWriter.append("  "+"\n\n\n\n");
					
					
					myOutWriter.close();
					fOut.close();
					
					Toast.makeText(getApplicationContext(), "" + "plz do it again", Toast.LENGTH_LONG).show();
					
				} catch (Exception e) {
					Toast.makeText(getBaseContext(), e.getMessage(), Toast.LENGTH_SHORT)
					.show();
				}
				
			}
		});
	}

	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int mActivePointerId;

		if (event.getAction() == MotionEvent.ACTION_DOWN) {
			//velocity_old.addMovement(event);
			
			// Get the pointer ID
		    mActivePointerId = event.getPointerId(0);

		    // ... Many touch events later...

		    // Use the pointer ID to find the index of the active pointer 
		    // and fetch its position
		    int pointerIndex = event.findPointerIndex(mActivePointerId);

			
			old_pos_x = event.getX();
			old_pos_y = event.getY();
		    time_old = event.getEventTime();
		    old_size = event.getSize(pointerIndex);
		    pressure_old = event.getPressure(pointerIndex);

		} else if (event.getAction() == MotionEvent.ACTION_UP) {
			//velocity_new.addMovement(event);
			// Get the pointer ID
		    mActivePointerId = event.getPointerId(0);

		    // ... Many touch events later...

		    // Use the pointer ID to find the index of the active pointer 
		    // and fetch its position
		    int pointerIndex = event.findPointerIndex(mActivePointerId);

			
			
			
			new_pos_x = event.getX();
			new_pos_y = event.getY();
			time_new = event.getEventTime();
			new_size = event.getSize(pointerIndex);
			pressure_new = event.getPressure(pointerIndex);

		}

		if (event.getAction() == MotionEvent.ACTION_MOVE) {
			velocity.addMovement(event);
		}
		return super.onTouchEvent(event);
	}

	// Register the event listener and sensor type.
    public void setListners(SensorManager sensorManager, SensorEventListener mEventListener)
    {
        sensorManager.registerListener(mEventListener, sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER), 
                SensorManager.SENSOR_DELAY_NORMAL);
        sensorManager.registerListener(mEventListener, sensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD), 
                SensorManager.SENSOR_DELAY_NORMAL);
    }
}
