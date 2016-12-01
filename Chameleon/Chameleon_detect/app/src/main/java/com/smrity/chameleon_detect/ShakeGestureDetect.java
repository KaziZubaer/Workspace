package com.smrity.chameleon_detect;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.smrity.chameleon_detect.ShakeDetector.OnShakeListener;

public class ShakeGestureDetect extends Activity{
	
	public static File file_acc;
	public static FileOutputStream fOut_acc;
	public static OutputStreamWriter myOutWriter_acc;
	
	private ShakeDetector mShakeDetector;
	private SensorManager mSensorManager;
	private Sensor mAccelerometer;
	public static String name;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.shake_gesture);
		
		Intent intent = getIntent();
		
		name = intent.getStringExtra("Name");
		
		final EditText editTextGesture = (EditText) findViewById(R.id.editTextShakeGesture);
		Button button = (Button) findViewById(R.id.buttonSaveGesture);	
		button.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				try{
					
					
					file_acc = new File("/sdcard/chameleon_"
							+ name+ "_shake.txt");
					fOut_acc = new FileOutputStream(file_acc, true);
	
					myOutWriter_acc = new OutputStreamWriter(fOut_acc);
					myOutWriter_acc.append("***************" + editTextGesture.getText().toString()+"***************\n\n\n");
		            
					myOutWriter_acc.close();
					fOut_acc.close();
					 
		            Toast.makeText(getApplicationContext(), editTextGesture.getText().toString(), Toast.LENGTH_LONG).show();	
				}catch (Exception e) {
					// TODO: handle exception
				}
			}
		});
		
		// ShakeDetector initialization
		mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
		mAccelerometer = mSensorManager
				.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);		
		mShakeDetector = new ShakeDetector(new OnShakeListener() {
			@Override
			public void onShake() {
				Toast.makeText(getApplicationContext(), "shaking",
						Toast.LENGTH_LONG).show();
			}
		});
		
		
	}
	@Override
	protected void onResume() {
		super.onResume();
		mSensorManager.registerListener(mShakeDetector, mAccelerometer,
				SensorManager.SENSOR_DELAY_UI);
	}
	
	@Override
	protected void onPause() {
		mSensorManager.unregisterListener(mShakeDetector);
		super.onPause();
	}
}
