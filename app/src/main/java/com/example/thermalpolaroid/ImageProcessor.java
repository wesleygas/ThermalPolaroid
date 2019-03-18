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
        System.out.println("Original Width: " + Integer.toString(oldWidth));
        System.out.println("Original Height " + Integer.toString(oldHeight));
        if(height > oldHeight){
            height = oldHeight;
        }
        if(width > oldWidth){
            width = oldWidth;
        }

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
        int width = imageToConvert.getWidth();
        int height = imageToConvert.getHeight();

        Bitmap dest = Bitmap.createBitmap(width, height,
                Bitmap.Config.RGB_565);

        Canvas canvas = new Canvas(dest);
        Paint paint = new Paint();
        ColorMatrix colorMatrix = new ColorMatrix();
        colorMatrix.setSaturation(0); //value of 0 maps the color to gray-scale
        ColorMatrixColorFilter filter = new ColorMatrixColorFilter(colorMatrix);
        paint.setColorFilter(filter);
        canvas.drawBitmap(imageToConvert, 0, 0, paint);

        return dest;
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
