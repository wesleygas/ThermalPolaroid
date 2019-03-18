package com.example.thermalpolaroid;

import android.app.ProgressDialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

public class PayActivity extends AppCompatActivity {

    Bitmap bitmapImage;
    Bundle extras = new Bundle();

    public void onBackPressed() {
        super.onBackPressed();
        openCameraActivity();
        finish();
    }
    //
    private void openCameraActivity() {
        Intent intent = new Intent(this, CameraActivity.class);
        startActivity(intent);
        finish();
    }

    private void openBluetoothActivity(){
        Intent intent = new Intent(this, BluetoothActivity.class);
        extras.putParcelable("imagebitmap", bitmapImage);
        intent.putExtras(extras);
        startActivity(intent);
        finish();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_pay);

        Bundle extras = getIntent().getExtras();
        bitmapImage = (Bitmap) extras.getParcelable("imagebitmap");

        Button buttonFinish = (Button)findViewById(R.id.buttonPay);

        buttonFinish.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                openBluetoothActivity();
                }
            });

        ImageView img = (ImageView) findViewById(R.id.ImageViewer) ;
        img.setImageBitmap(bitmapImage);
    }
}
