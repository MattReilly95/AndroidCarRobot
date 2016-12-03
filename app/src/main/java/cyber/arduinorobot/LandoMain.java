package cyber.arduinorobot;

/**
 * Created by train on 11/26/2016.
 */
// Java imports
        import java.io.IOException;
        import java.util.*;
        import java.util.concurrent.*;

// Android imports
        import android.app.*;
        import android.os.Bundle;
        import android.os.*;
        import android.view.*;
        import android.bluetooth.*;
        import android.widget.*;
        import android.content.*;

// Main view/Process
public class LandoMain extends Activity
{
    receive btReceive;

    // Bluetooth setup
    private final static int REQUEST_ENABLE_BT = 1;
    public static BluetoothSocket socket;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        // Create the main UI
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lando_main);

        // Ask user to enable Bluetooth if it is off
        if (!BluetoothAdapter.getDefaultAdapter().isEnabled())
        {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu)
    {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.lando_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item)
    {
        //do things here
        return true;
    }

    // Gets the device address of any paired device, only works for paired devices
    public void getAddresses(View v)
    {
        try
        {
            Set<BluetoothDevice> pairedDevices = BluetoothAdapter.getDefaultAdapter().getBondedDevices();

            // If there are paired devices
            if (pairedDevices.size() > 0)
            {
                // Loop through paired devices
                for (BluetoothDevice device : pairedDevices)
                {
                    // Toast address
                    Toast toast = Toast.makeText(getBaseContext(),
                            device.getName() + "\n" + device.getAddress(), Toast.LENGTH_LONG);
                    toast.show();
                }
            }
        }
        catch(Exception e) // Toast error
        {
            e.printStackTrace();
            Toast toast = Toast.makeText(getBaseContext(),
                    "Something broke :(\nError: " + e.getMessage(), Toast.LENGTH_LONG);
            toast.show();
        }
    }

    public void startConnection()
    {
        final ExecutorService service;
        final Future<BluetoothSocket> task;
        service = Executors.newFixedThreadPool(1);

        task = service.submit(new connect());

        try
        {
            socket = task.get();
        }

        catch(final InterruptedException ex)
        {
            ex.printStackTrace();
        }
        catch(final ExecutionException ex)
        {
            ex.printStackTrace();
        }

        service.shutdownNow();

        if(socket.isConnected())
        {
            Toast toast = Toast.makeText(getBaseContext(), "Connected", Toast.LENGTH_LONG);
            toast.show();

            btReceive = new receive(btHandler, socket);

            btReceive.start();
        }
        else
        {
            Toast toast = Toast.makeText(getBaseContext(), "Couldn't Connect", Toast.LENGTH_LONG);
            toast.show();
        }
    }

    // Attempts to connect to the LandoBot
    public void connect(View v)
    {
        // Ask user to enable Bluetooth if it is off
        if (!BluetoothAdapter.getDefaultAdapter().isEnabled())
        {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        }

        startConnection();
    }

    // Serial code to go forward
    public void forward(View v) throws IOException
    {
        if(socket != null)
        {
            send signal = new send(socket, "#1700,1300$");
            Handler handler = new Handler();
            handler.post(signal);
        }
    }

    // Serial code to go backwards
    public void reverse(View v)
    {
        if(socket != null)
        {
            send signal = new send(socket, "#1300,1700$");
            Handler handler = new Handler();
            handler.post(signal);
        }
    }

    // Serial code to turn left
    public void turnLeft(View v)
    {
        if(socket != null)
        {
            send signal = new send(socket, "#1300,1500$");
            Handler handler = new Handler();
            handler.post(signal);
        }
    }

    // Serial code to turn right
    public void turnRight(View v)
    {
        if(socket != null)
        {
            send signal = new send(socket, "#1500,1700$");
            Handler handler = new Handler();
            handler.post(signal);
        }
    }

    // Serial code to stop
    public void stop(View v)
    {
        if(socket != null)
        {
            send signal = new send(socket, "#1500,1500$");
            Handler handler = new Handler();
            handler.post(signal);
        }
    }

    // Serial code to have sensor sweep
    public void sweep(View v)
    {
        if(socket != null)
        {
            send signal = new send(socket, "s");
            Handler handler = new Handler();
            handler.post(signal);
        }
    }

    //method to send to arduino to turn lights on
    public void turnLightOn(View v){
        if(socket != null){
            send signal = new send(socket, "l");
            Handler handler = new Handler();
            handler.post(signal);
        }
    }

    // The Handler that gets information back from the BluetoothChatService
    private final Handler btHandler = new Handler()
    {
        @Override
        public void handleMessage(Message msg)
        {

            byte[] readBuf = (byte[]) msg.obj;

            // construct a string from the valid bytes in the buffer
            String readMessage = new String(readBuf, 0, msg.arg1);

            Toast.makeText(getApplicationContext(), "Received " + readMessage,
                    Toast.LENGTH_SHORT).show();
        }
    };
}
