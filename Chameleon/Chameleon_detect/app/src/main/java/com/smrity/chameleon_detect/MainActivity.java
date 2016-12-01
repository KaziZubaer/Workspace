package com.smrity.chameleon_detect;

import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends Activity {

	public static File file_acc, file;

	public static FileOutputStream fOut_acc, fOut;

	public static OutputStreamWriter myOutWriter_acc, myOutWriter;
	EditText editTextName;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		editTextName = (EditText) findViewById(R.id.editTextName);
		Button buttonCreate = (Button) findViewById(R.id.buttonCreate);
		buttonCreate.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {

				try {

					file = new File("/sdcard/chameleon_"
							+ editTextName.getText().toString() + ".txt");
					if (!file.exists()) {
						file.createNewFile();
						Toast.makeText(getApplicationContext(),
								editTextName.getText().toString() + " created",
								Toast.LENGTH_LONG).show();
					}

					file_acc = new File("/sdcard/chameleon_"
							+ editTextName.getText().toString() + "_shake.txt");
					if (!file_acc.exists()) {
						file_acc.createNewFile();
					}
					
					fOut_acc = new FileOutputStream(file_acc, true);
					fOut = new FileOutputStream(file, true);
					
					myOutWriter = new OutputStreamWriter(fOut);
					myOutWriter_acc = new OutputStreamWriter(fOut_acc);
					
					myOutWriter.append("***************************\n\n");
					myOutWriter_acc.append("***************************\n\n");
					
					myOutWriter.close();
					myOutWriter_acc.close();
					
					fOut.close();
					fOut_acc.close();
					
				} catch (Exception e) {
					Toast.makeText(getBaseContext(), e.getMessage(),
							Toast.LENGTH_SHORT).show();
				}
				Intent intent = new Intent(MainActivity.this,
						OptionActivity.class);
			    intent.putExtra("Name", editTextName.getText().toString());
				startActivity(intent);
			}
		});
	}
	
	

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}
