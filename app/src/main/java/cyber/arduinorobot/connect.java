package cyber.arduinorobot;

/**
 * Created by train on 11/26/2016.
 */


import java.io.IOException;
import java.util.UUID;
import java.util.concurrent.Callable;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

public class connect implements Callable<BluetoothSocket>
{
    public BluetoothSocket call()
    {
        BluetoothAdapter bt = BluetoothAdapter.getDefaultAdapter();

        BluetoothDevice landoBot = bt.getRemoteDevice("00:06:66:6B:B1:49");
        //00:06:66:61:5F:50

        UUID uuid = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

        BluetoothSocket socket = null;

        try
        {

            socket=landoBot.createRfcommSocketToServiceRecord(uuid);

            socket.connect();
        }
        catch(Exception e)
        {

            e.printStackTrace();
        }

        return socket;
    }
}

