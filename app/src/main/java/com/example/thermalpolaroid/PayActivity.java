package com.example.thermalpolaroid;

import android.app.ProgressDialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

public class PayActivity extends AppCompatActivity {

    Bitmap bitmapImage;
    String image_uri;
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
        image_uri = extras.getString("image_uri");
        bitmapImage = getPic();

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

    private Bitmap getPic() {
        // Get the dimensions of the View
        int targetW = 1200;
        //int targetH = ;

        // Get the dimensions of the bitmap
        BitmapFactory.Options bmOptions = new BitmapFactory.Options();
        bmOptions.inJustDecodeBounds = true;
        BitmapFactory.decodeFile(image_uri, bmOptions);
        int photoW = bmOptions.outWidth;
        //int photoH = bmOptions.outHeight;

        // Determine how much to scale down the image
        int scaleFactor = photoW/targetW;

        // Decode the image file into a Bitmap sized to fill the View
        bmOptions.inJustDecodeBounds = false;
        bmOptions.inSampleSize = scaleFactor;
        bmOptions.inPurgeable = true;

        return BitmapFactory.decodeFile(image_uri, bmOptions);
    }
}
