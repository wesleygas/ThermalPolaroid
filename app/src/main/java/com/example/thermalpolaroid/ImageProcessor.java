package com.example.thermalpolaroid;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.ColorMatrix;
import android.graphics.ColorMatrixColorFilter;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.util.Base64;

import java.io.ByteArrayOutputStream;

public class ImageProcessor {

    public static Bitmap resizedImage(Bitmap imageToConvert, int height, int width){
        int oldWidth = imageToConvert.getWidth();

        int oldHeight = imageToConvert.getHeight();

        float scaleWidth = ((float) width) / oldWidth;

        float scaleHeight = ((float) height) / oldHeight;

        // CREATE A MATRIX FOR THE MANIPULATION
        Matrix matrix = new Matrix();

        // RESIZE THE BIT MAP
        matrix.postScale(scaleWidth, scaleHeight);

        // RECREATE THE NEW BITMAP
        Bitmap resizeImage = Bitmap.createBitmap(imageToConvert, 0, 0, width, height, matrix, false);

        return resizeImage;
    }

    public static Bitmap colorToGray(Bitmap imageToConvert){
        int width, height;
        height = imageToConvert.getHeight();
        width = imageToConvert.getWidth();

        Bitmap grayImage = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        Canvas c = new Canvas(grayImage);
        Paint paint = new Paint();
        ColorMatrix cm = new ColorMatrix();
        cm.setSaturation(0);
        ColorMatrixColorFilter f = new ColorMatrixColorFilter(cm);
        paint.setColorFilter(f);
        c.drawBitmap(grayImage, 0, 0, paint);
        return grayImage;
    }

    public static Bitmap ditchingImage(Bitmap imageToConvert){
        Bitmap ditchedImage = imageToConvert;
        System.out.println("DITCHING IMAHE@@@@@@@@@@@@@@@@@@@@");
        return ditchedImage;
    }

    //Code from: http://androidapplicationdeveloper.weebly.com/android-tutorial/how-to-convert-bitmap-to-string-and-string-to-bitmap
    //Last access june-07-2018
    public static String BitmapToString(Bitmap bitmap){
        ByteArrayOutputStream baos = new  ByteArrayOutputStream();
        bitmap.compress(Bitmap.CompressFormat.PNG,100, baos);
        byte [] arr=baos.toByteArray();
        return Base64.encodeToString(arr, Base64.DEFAULT);
    }
}
