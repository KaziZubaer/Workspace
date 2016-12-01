package com.smrity.chameleon_detect;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import java.io.File;
import java.io.IOException;

public class OptionActivity extends Activity {

    /* renamed from: com.smrity.chameleon_detect.OptionActivity.1 */
    class C00561 implements OnClickListener {
        final /* synthetic */ String val$name;

        C00561(String str) {
            this.val$name = str;
        }

        public void onClick(View v) {
            Intent intent = new Intent(OptionActivity.this, TouchGesturePractice.class);
            intent.putExtra("Name", this.val$name);
            OptionActivity.this.startActivity(intent);
        }
    }

    /* renamed from: com.smrity.chameleon_detect.OptionActivity.2 */
    class C00572 implements OnClickListener {
        final /* synthetic */ EditText val$fileNo;
        final /* synthetic */ String val$name;

        C00572(String str, EditText editText) {
            this.val$name = str;
            this.val$fileNo = editText;
        }

        public void onClick(View v) {
            Intent intent = new Intent(OptionActivity.this, TouchGestureTrain.class);
            intent.putExtra("Name", this.val$name);
            intent.putExtra("FileNo", this.val$fileNo.getText().toString());
            File file = new File("/sdcard/chameleon_data" + this.val$fileNo.getText().toString() + ".txt");
            if (!file.exists()) {
                try {
                    file.createNewFile();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            OptionActivity.this.startActivity(intent);
        }
    }

    /* renamed from: com.smrity.chameleon_detect.OptionActivity.3 */
    class C00583 implements OnClickListener {
        final /* synthetic */ EditText val$fileNo;
        final /* synthetic */ CheckBox val$mimic;
        final /* synthetic */ String val$name;

        C00583(String str, EditText editText, CheckBox checkBox) {
            this.val$name = str;
            this.val$fileNo = editText;
            this.val$mimic = checkBox;
        }

        public void onClick(View v) {
            Intent intent = new Intent(OptionActivity.this, TouchGestureTest.class);
            intent.putExtra("Name", this.val$name);
            intent.putExtra("FileNo", this.val$fileNo.getText().toString());
            File file = new File("/sdcard/chameleon_data" + this.val$fileNo.getText().toString() + ".txt");
            if (!file.exists()) {
                try {
                    file.createNewFile();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (this.val$mimic.isChecked()) {
                intent.putExtra("Mimic", "1");
            } else {
                intent.putExtra("Mimic", "0");
            }
            OptionActivity.this.startActivity(intent);
        }
    }

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(C0059R.layout.option);
        String name = getIntent().getStringExtra("Name");
        Button buttonTrain = (Button) findViewById(C0059R.id.buttonTrain);
        Button buttonTest = (Button) findViewById(C0059R.id.buttonTest);
        CheckBox mimic = (CheckBox) findViewById(C0059R.id.checkBoxMimic);
        EditText fileNo = (EditText) findViewById(C0059R.id.editTextFileNo);
        ((Button) findViewById(C0059R.id.buttonPractice)).setOnClickListener(new C00561(name));
        buttonTrain.setOnClickListener(new C00572(name, fileNo));
        buttonTest.setOnClickListener(new C00583(name, fileNo, mimic));
    }
}
