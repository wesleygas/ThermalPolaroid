package com.example.thermalpolaroid;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.GridView;

public class GalleryActivity extends AppCompatActivity{
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gallery);

        GridView gridView = (GridView) findViewById(R.id.gridview);

        gridView.setAdapter(new ImageAdapter(this));

        gridView.setOnItemClickListener(
            new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                    openPayActivity(position);
                }
            }
        );
    }

    private void openPayActivity(Integer photoPos) {
        Intent intent = new Intent(this, PayActivity.class);
        intent.putExtra("image_position", photoPos);
        startActivity(intent);
        finish();
    }


}
