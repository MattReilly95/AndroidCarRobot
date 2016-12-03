package cyber.arduinorobot;

/**
 * Created by train on 11/26/2016.
 */

import android.bluetooth.BluetoothSocket;
import android.os.Handler;
import android.util.Log;

import java.io.InputStream;
import java.io.OutputStream;

public class receive extends Thread
{
    private static final String TAG = "LandoBot";
    private final Handler btHandler;

    InputStream btInStream = null;
    OutputStream btOutStream = null;

    private BluetoothSocket btSocket = null;

    public receive(Handler handler, BluetoothSocket socket)
    {
        btHandler = handler;
        btSocket = socket;
    }

    public void run()
    {
        // Get the BluetoothSocket input and output streams
        try
        {
            btInStream = btSocket.getInputStream();
            btOutStream = btSocket.getOutputStream();
        }
        catch (Exception e)
        {
            Log.e(TAG, "Error getting streams", e);
        }

        byte[] buffer = new byte[1024];
        int bytes;

        // Keep listening to the InputStream while connected
        while (true)
        {
            try
            {
                // Read from the InputStream
                bytes = btInStream.read(buffer);

                // Send the obtained bytes to the UI Activity
                btHandler.obtainMessage(1, bytes, -1, buffer).sendToTarget();
            }
            catch (Exception e)
            {
                Log.e(TAG, "disconnected", e);
                break;
            }
        }
    }
}