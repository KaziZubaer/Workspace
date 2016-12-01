package com.smrity.chameleon_detect;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.util.HashSet;

public class MainActivity extends Activity {
    public static FileOutputStream fOut;
    public static File file_demo;
    public static File file_dummy;
    public static File file_log;
    public static OutputStreamWriter myOutWriter;
    Button buttonEnter;
    Button buttonSignUp;
    EditText editTextAge;
    EditText editTextEducation;
    EditText editTextGender;
    EditText editTextName;
    EditText editTextNameEnter;
    EditText editTextUseExp;
    HashSet<String> listOfUser;

    /* renamed from: com.smrity.chameleon_detect.MainActivity.1 */
    class C00541 implements OnClickListener {
        C00541() {
        }

        public void onClick(View v) {
            if (MainActivity.this.listOfUser.contains(MainActivity.this.editTextNameEnter.getText().toString())) {
                Intent intent = new Intent(MainActivity.this, OptionActivity.class);
                intent.putExtra("Name", MainActivity.this.editTextNameEnter.getText().toString());
                MainActivity.this.startActivity(intent);
                return;
            }
            Toast.makeText(MainActivity.this.getApplicationContext(), "User doesn't exist", 1).show();
        }
    }

    /* renamed from: com.smrity.chameleon_detect.MainActivity.2 */
    class C00552 implements OnClickListener {
        C00552() {
        }

        public void onClick(View v) {
            try {
                MainActivity.fOut = new FileOutputStream(MainActivity.file_demo, true);
                MainActivity.myOutWriter = new OutputStreamWriter(MainActivity.fOut);
                MainActivity.myOutWriter.append(MainActivity.this.editTextName.getText().toString() + ",");
                MainActivity.myOutWriter.append(MainActivity.this.editTextAge.getText().toString() + ",");
                MainActivity.myOutWriter.append(MainActivity.this.editTextGender.getText().toString() + ",");
                MainActivity.myOutWriter.append(MainActivity.this.editTextEducation.getText().toString() + ",");
                MainActivity.myOutWriter.append(MainActivity.this.editTextUseExp.getText().toString() + "\n");
                MainActivity.myOutWriter.close();
                MainActivity.fOut.close();
                MainActivity.this.listOfUser.add(MainActivity.this.editTextName.getText().toString());
                Toast.makeText(MainActivity.this.getApplicationContext(), "Data saved", 1).show();
            } catch (Exception e) {
                Toast.makeText(MainActivity.this.getBaseContext(), e.getMessage(), 0).show();
            }
        }
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(C0059R.layout.activity_main);
        initializeFiles();
        this.editTextNameEnter = (EditText) findViewById(C0059R.id.editTextNameEnter);
        this.editTextName = (EditText) findViewById(C0059R.id.editTextName);
        this.editTextAge = (EditText) findViewById(C0059R.id.editTextAge);
        this.editTextGender = (EditText) findViewById(C0059R.id.editTextGender);
        this.editTextEducation = (EditText) findViewById(C0059R.id.editTextEducation);
        this.editTextUseExp = (EditText) findViewById(C0059R.id.editTextUseExp);
        this.buttonEnter = (Button) findViewById(C0059R.id.buttonEnter);
        this.buttonSignUp = (Button) findViewById(C0059R.id.buttonSignUp);
        this.buttonEnter.setOnClickListener(new C00541());
        this.buttonSignUp.setOnClickListener(new C00552());
    }

    private void initializeFiles() {
        file_demo = new File("/sdcard/chameleon_demo.txt");
        file_log = new File("/sdcard/chameleon_log.txt");
        file_dummy = new File("/sdcard/chameleon_dummy.txt");
        if (!file_demo.exists()) {
            try {
                file_demo.createNewFile();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        if (!file_log.exists()) {
            try {
                file_log.createNewFile();
            } catch (IOException e2) {
                e2.printStackTrace();
            }
        }
        if (!file_dummy.exists()) {
            try {
                file_dummy.createNewFile();
            } catch (IOException e22) {
                e22.printStackTrace();
            }
        }
        this.listOfUser = new HashSet();
        FileReader fileR = null;
        try {
            fileR = new FileReader(file_demo);
        } catch (FileNotFoundException e3) {
            e3.printStackTrace();
        }
        BufferedReader myReader = new BufferedReader(fileR);
        while (true) {
            try {
                String line = myReader.readLine();
                if (line != null) {
                    this.listOfUser.add(line.split(",")[0]);
                } else {
                    return;
                }
            } catch (IOException e222) {
                e222.printStackTrace();
                return;
            }
        }
    }

    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(C0059R.menu.main, menu);
        return true;
    }
}
