package com.magabelab.magabeserver;

import org.qtproject.qt5.android.bindings.QtActivity;

import android.content.res.Configuration;
import android.content.Context;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Toast;
import android.os.Bundle;
import java.lang.Runnable;
import android.content.Intent;
import android.view.Gravity;
import android.os.PowerManager;
import android.annotation.TargetApi;
import android.os.Build;

import static android.view.KeyEvent.KEYCODE_VOLUME_DOWN;
import static android.view.KeyEvent.KEYCODE_VOLUME_MUTE;
import static android.view.KeyEvent.KEYCODE_VOLUME_UP;


public class MagabeServerActivity extends QtActivity {

    private PowerManager.WakeLock mFullWakeLock;
    public static MagabeServerActivity m_activity = null;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        m_activity = this;

        PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        mFullWakeLock = pm.newWakeLock(PowerManager.FULL_WAKE_LOCK, "Magabe Player FULL_WAKE_LOCK");//FULL_WAKE_LOCK is deplicated,but i need it, PARTIAL_WAKE_LOCK doent sute my needs
    }

    public static MagabeServerActivity getInstance() {
        return m_activity;
    }

    @Override
    protected void onPause() {
        super.onPause();
        releaseWakeLock();//ANOTHER CALL TO releaseWakeLock, A BACKUP
        onActivityPause();
    }


    @Override
    public void onDestroy() {//some times this is never called, i hv a backup in on pause
        releaseWakeLock();
        m_activity = null;
        super.onDestroy();
    }

    public void releaseWakeLock() {
        if (mFullWakeLock.isHeld()) {
            runOnUiThread(new Runnable() {
                public void run() {
                    if (mFullWakeLock.isHeld()) {
                        mFullWakeLock.release();
                    }
                }
            });
        }
    }

    private void acquireWakeLock() {
        if (!mFullWakeLock.isHeld()) {
            runOnUiThread(new Runnable() {
                public void run() {
                    if (!mFullWakeLock.isHeld()) {
                        mFullWakeLock.acquire();
                    }
                }
            });
        }
    }

    private boolean hasActiveWakeLock() {
        return mFullWakeLock.isHeld();
    }

    public void showToast(final String Message) {
        //Natakiwa ni hi run hii call katika Main thread  ie Android UI thread;
        runOnUiThread(new Runnable() {
            // this method is called on Android Ui Thread
            @Override
            public void run() {
                Toast toast = Toast.makeText(getApplicationContext(), Message, Toast.LENGTH_SHORT);
                toast.setGravity(Gravity.CENTER, 0, 0);
                toast.show();
            }
        });

    }


    public void sendSMS(final String number, final String message) {

        if (number.length() == 0) {
            showToast("Please enter phone number");
            return;
        }

        runOnUiThread(new Runnable() {
            // this method is called on Android Ui Thread
            @Override
            public void run() {
                //send sms here
                // Log.i("sms to " + number, "message send:" + message);
                Intent sendIntent = new Intent(Intent.ACTION_VIEW);
                sendIntent.putExtra("address", number);
                sendIntent.putExtra("sms_body", message);
                sendIntent.setType("vnd.android-dir/mms-sms");
                startActivity(sendIntent);
            }
        });

    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        //int ORIENTATION_LANDSCAPE = 2;
        // int ORIENTATION_PORTRAIT = 1;
        switch (newConfig.orientation) {
            case Configuration.ORIENTATION_LANDSCAPE:
                onOrientationLandscape();
                break;

            case Configuration.ORIENTATION_PORTRAIT:
                onOrientationPortrait();
                break;

            default:
                break;
        }
    }

    private boolean isPortrait() {
        return (getResources().getConfiguration().orientation == Configuration.ORIENTATION_PORTRAIT);
    }

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    private void goNormalScreen() {
        final View view = getWindow().getDecorView().getRootView();
        final int flags = View.SYSTEM_UI_FLAG_VISIBLE;

        if (view.getSystemUiVisibility() != flags) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    view.setSystemUiVisibility(flags);
                }
            });
        }
    }

    @TargetApi(Build.VERSION_CODES.JELLY_BEAN)
    private void goFullScreen() {
        final int flags = View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
        final View view = getWindow().getDecorView().getRootView();

        if (view.getSystemUiVisibility() != flags) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    view.setSystemUiVisibility(flags);
                }
            });
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KEYCODE_VOLUME_DOWN) {
            onVolumeDown();
        }
        if (keyCode == KEYCODE_VOLUME_MUTE) {
            onVolumeMute();
        }
        if (keyCode == KEYCODE_VOLUME_UP) {
            onVolumeUp();
        }
        return super.onKeyDown(keyCode, event);
    }


    ///static method

    private static native void onOrientationLandscape();

    private static native void onOrientationPortrait();

    //OTHER LIFE CYCLE EVENT CAUSE THE APP O CLASH, DO NOT IMPLETEMENT THEM
    private static native void onActivityPause();

    private static native void onVolumeDown();

    private static native void onVolumeUp();

    private static native void onVolumeMute();


}
