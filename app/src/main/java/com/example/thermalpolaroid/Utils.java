package com.example.thermalpolaroid;

import android.content.Context;
import android.widget.Toast;

public class Utils {
    public static void showMessage(Context context, String message) {
        Toast toast = Toast.makeText(context, message, Toast.LENGTH_SHORT);
        toast.show();
    }
}
