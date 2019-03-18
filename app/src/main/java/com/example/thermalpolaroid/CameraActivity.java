package com.example.thermalpolaroid;

import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.provider.MediaStore;
import android.support.v4.content.FileProvider;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;


public class CameraActivity extends AppCompatActivity {

    Context context = this;

    static final int REQUEST_IMAGE_CAPTURE = 1;
    static final int REQUEST_TAKE_PHOTO = 1;
    String currentPhotoPath;
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
        extras.putString("image_uri", currentPhotoPath);
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
                            //imageBitmap = ImageProcessor.resizedImage(imageBitmap,800,800);
                            progressDialog.cancel();
//
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
            // Create the File where the photo should go
            File photoFile = null;
            try {
                photoFile = createImageFile();
            } catch (IOException ex) {
                // Error occurred while creating the File
            }
            // Continue only if the File was successfully created
            if (photoFile != null) {
                Uri photoURI = FileProvider.getUriForFile(this,"com.example.android.fileprovider",photoFile);
                takePictureIntent.putExtra(MediaStore.EXTRA_OUTPUT, photoURI);
                startActivityForResult(takePictureIntent, REQUEST_TAKE_PHOTO);
            }
        }
    }

    private void showWarning(String message){
        Utils.showMessage(this, message);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == REQUEST_IMAGE_CAPTURE && resultCode == RESULT_OK) {
            extras = data.getExtras();
            imageBitmap = getPic();
            BitmapDrawable bdrawable = new BitmapDrawable(getResources(),imageBitmap);
            buttonPhotoImage.setBackground(bdrawable);
        }
    }



    private File createImageFile() throws IOException {
        // Create an image file name
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        String imageFileName = "JPEG_" + timeStamp + "_";
        File storageDir = getExternalFilesDir(Environment.DIRECTORY_PICTURES);
        File image = File.createTempFile(
                imageFileName,  /* prefix */
                ".jpg",         /* suffix */
                storageDir      /* directory */
        );

        // Save a file: path for use with ACTION_VIEW intents
        currentPhotoPath = image.getAbsolutePath();
        return image;
    }

    private Bitmap getPic() {
        // Get the dimensions of the View
        int targetW = 384;
        //int targetH = ;

        // Get the dimensions of the bitmap
        BitmapFactory.Options bmOptions = new BitmapFactory.Options();
        bmOptions.inJustDecodeBounds = true;
        BitmapFactory.decodeFile(currentPhotoPath, bmOptions);
        int photoW = bmOptions.outWidth;
        //int photoH = bmOptions.outHeight;

        // Determine how much to scale down the image
        int scaleFactor = targetW/photoW;

        // Decode the image file into a Bitmap sized to fill the View
        bmOptions.inJustDecodeBounds = false;
        bmOptions.inSampleSize = scaleFactor;
        bmOptions.inPurgeable = true;

        return BitmapFactory.decodeFile(currentPhotoPath, bmOptions);
    }
}
