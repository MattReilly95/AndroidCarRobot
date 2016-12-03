package cyber.arduinorobot;

/**
 * Created by train on 11/26/2016.
 */

import java.io.IOException;
import java.io.OutputStream;
import java.nio.charset.Charset;

import android.bluetooth.BluetoothSocket;
import android.os.Handler;

public class send implements Runnable
{
    private BluetoothSocket socket;
    String message;

    public send(BluetoothSocket s, String m)
    {
        socket = s;
        message = m;
    }
    public void run()
    {
        try
        {
            OutputStream out = socket.getOutputStream();
            out.write(message.getBytes(Charset.forName("US-ASCII")));
        }
        catch(IOException e)
        {

        }
    }
}