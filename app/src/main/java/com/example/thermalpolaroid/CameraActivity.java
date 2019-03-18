package com.example.thermalpolaroid;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;


public class CameraActivity extends AppCompatActivity {

    Context context = this;

    static final int REQUEST_IMAGE_CAPTURE = 1;
    Bundle extras = new Bundle();
    Bitmap imageBitmap;
    ImageButton buttonPhotoImage;

    public void onBackPressed() {
        super.onBackPressed();
        openMainActivity();
        finish();
    }
//
    private void openMainActivity() {
        Intent intent = new Intent(this, MainActivity.class);
        startActivity(intent);
        finish();
    }

    private void openPayActivity(){
        Intent intent = new Intent(this, PayActivity.class);
        extras.putParcelable("imagebitmap", imageBitmap);
        intent.putExtras(extras);
        startActivity(intent);
        finish();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_camera);

        Button buttonPhoto = (Button)findViewById(R.id.button_photo);
        buttonPhotoImage = (ImageButton) findViewById(R.id.image_button);
        Button buttonFinish = (Button)findViewById(R.id.button_finish);

        buttonFinish.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (imageBitmap == null){
                    showWarning("É necessário uma foto para poder prosseguir");
                }
                else{
                    final ProgressDialog progressDialog = ProgressDialog.show(context,"Processando Imagem", "Por favor, aguarde!",true, true);
                    Handler handler = new Handler();
                    handler.postDelayed(new Runnable() {
                        @Override
                        public void run() {
                            progressDialog.cancel();
//                            imageBitmap = ImageProcessor.resizedImage(imageBitmap,320,320);
//                            imageBitmap = ImageProcessor.colorToGray(imageBitmap);
//                            imageBitmap = ImageProcessor.ditchingImage(imageBitmap);
                            openPayActivity();
                        }
                    },3000);
                }
            }
        });
    }

    public void dispatchTakePictureIntent(View view) {
        Intent takePictureIntent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
        if (takePictureIntent.resolveActivity(getPackageManager()) != null) {
            startActivityForResult(takePictureIntent, REQUEST_IMAGE_CAPTURE);
        }
    }

    private void showWarning(String message){
        Utils.showMessage(this, message);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_IMAGE_CAPTURE && resultCode == RESULT_OK) {
            extras = data.getExtras();
            imageBitmap = (Bitmap) extras.get("data");
            BitmapDrawable bdrawable = new BitmapDrawable(getResources(),imageBitmap);
            buttonPhotoImage.setBackground(bdrawable);
        }
    }
}
