package com.smrity.chameleon_detect;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.Toast;

public class OptionActivity extends Activity{
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.option);
		
		Intent intent = getIntent();
		
		final String name = intent.getStringExtra("Name");
		
		Button btnShake = (Button) findViewById(R.id.btnShake);
		Button buttonTouch = (Button) findViewById(R.id.buttonTouch);
		
		btnShake.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				Toast.makeText(getApplicationContext(), "shake", Toast.LENGTH_LONG).show();
				Intent intent = new Intent(OptionActivity.this, ShakeGestureDetect.class);
				intent.putExtra("Name", name);
				startActivity(intent);
			}
		});
		
		buttonTouch.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				//Toast.makeText(getApplicationContext(), "touch", Toast.LENGTH_LONG).show();
				Intent intent = new Intent(OptionActivity.this, TouchGestureDetect.class);
				intent.putExtra("Name", name);
				startActivity(intent);
			}
		});
	}
}
