package com.example.thermalpolaroid;

import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Bundle;

import android.Manifest;
import android.content.Intent;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity{

    private static final int REQUEST_CAMERA = 0;
    Context context = this;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.CAMERA)
                == PackageManager.PERMISSION_DENIED) {

            String[] permissions = new String[1];
            permissions[0] = Manifest.permission.CAMERA;

            ActivityCompat.requestPermissions(MainActivity.this, permissions, REQUEST_CAMERA);
        }


        Button photo_button = (Button) findViewById(R.id.photo_button);

        photo_button.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view) {
                if (ContextCompat.checkSelfPermission(MainActivity.this, Manifest.permission.CAMERA)
                        == PackageManager.PERMISSION_DENIED) {

                    String[] permissions = new String[1];
                    permissions[0] = Manifest.permission.CAMERA;

                    ActivityCompat.requestPermissions(MainActivity.this, permissions, REQUEST_CAMERA);
                }
                else{
                    openCameraActivity();
                }
            }

        });

    }

    private void openCameraActivity(){
        Intent intent = new Intent(this, CameraActivity.class);
        startActivity(intent);
        finish();
    }

    private void informativeNews(String message){
        Utils.showMessage(this, message);
    }

    @Override
    public void onRequestPermissionsResult(int request, String[] permissions, int[] results) {
        // Se o pedido de permissão foi para utilizar a camera....
        if(request == REQUEST_CAMERA) {
            // ...e a permissão não foi concedida, avisamos e pedimos novamente.
            if(results.length > 0 && results[0] == PackageManager.PERMISSION_DENIED) {
                informativeNews("Você precisa conceder permissão!");
            }
        }
    }
}
